package main

import (
	"bufio"
	"fmt"
	"io"
	"os"
	"regexp"
)

// main functie runs program
func main() {
	if len(os.Args) < 2 { // check for command-line argument
		errorMsg("Error: No command-line argument given")
	}
	f, errr := os.Open(os.Args[1])
	if errr != nil { // check if file opened
		errorMsg("Error: Opening file")
	}
	str := leesIn(f)
	f.Close()
	outputStr := parse(lex(parse(lex(str))))
	fw, errw := os.Create("output.txt")
	if errw != nil { // check if file opened
		errorMsg("Error: creating write file")
	}
	_, err := fw.WriteString(outputStr)
	if err != nil { // check if file opened
		errorMsg("Error: Writing to file")
	}
	os.Exit(0)
} // main

// class for tokens
type token struct {
	category string
	value    string
}

// lexical analyser defines tokens
func lex(str string) []token {
	tokens := []token{
		{
			category: "lbracket",
			value:    "^\\($",
		},
		{
			category: "rbracket",
			value:    "^\\)$",
		},
		{
			category: "lambda",
			value:    "^\u03bb$|^\\\\$",
		},
		{
			category: "dot",
			value:    "^\\.$",
		},
		{
			category: "variable",
			value:    "^[a-zA-Z][a-zA-Z0-9]*$",
		},
	}
	return makeTokens(str, tokens)
}

// Turn input string into token slice
func makeTokens(str string, tokens []token) []token {
	var tokenstream []token
	var newToken token
OUTER:
	for _, char := range str {
	INNER:
		for _, token := range tokens {
			match, _ := regexp.MatchString(token.value, newToken.value+string(char))
			if match {
				newToken.category = token.category
				newToken.value += string(char)
				continue OUTER // proceed to next char
			}
		}
		if newToken.category != "" { // check if not invalid token
			tokenstream = append(tokenstream, newToken)
			newToken.value = ""
			newToken.category = ""
			goto INNER // proceed with the same char in new token
		} else if string(char) != " " {
			errorMsg("Error: illegal input")
		}
	}
	if newToken.category != "" { // last token
		tokenstream = append(tokenstream, newToken)
	}
	newToken = token{ // add end of file token
		category: "eof",
		value:    "eof",
	}
	tokenstream = append(tokenstream, newToken)
	return tokenstream
}

// get rid of first and last parentheses if they belong together
func flpar(tokenStr string) string {
	parCount := 0
	if string(tokenStr[0]) == "(" {
		for i := 0; i < len(tokenStr)-1; i++ {
			if string(tokenStr[i]) == "(" {
				parCount++
			} else if string(tokenStr[i]) == ")" {
				parCount--
			}
			if parCount == 0 && i != len(tokenStr)-2 { // if at some point the
				return tokenStr // counter returns to 0 the first parentheses is closed
			}
		}
		tokenStr = (tokenStr)[1 : len(tokenStr)-1]
	}
	return tokenStr
}

// Parse the tokenstream, return string
func parse(tokenstream []token) string {
	var tokenStr string
	expr(&tokenstream, &tokenStr)
	tokenStr = flpar(tokenStr)
	return tokenStr
}

// LL grammer expr function
func expr(tokenstream *[]token, tokenStr *string) {
	lexpr(tokenstream, tokenStr)
	expr1(tokenstream, tokenStr)
}

// LL grammer expr' function
func expr1(tokenstream *[]token, tokenStr *string) {
	tokenCat := peekToken(tokenstream).category
	if tokenCat == "variable" || tokenCat == "lambda" || tokenCat == "lbracket" {
		lexpr(tokenstream, tokenStr)
		expr1(tokenstream, tokenStr)
	}
	// can choose to do nothing
}

// LL grammer lexpr function
func lexpr(tokenstream *[]token, tokenStr *string) {
	tokenCat := peekToken(tokenstream).category
	if tokenCat == "lambda" {
		*tokenStr += getToken(tokenstream).value
		if peekToken(tokenstream).category == "variable" {
			*tokenStr += getToken(tokenstream).value
		} else {
			errorMsg("Error: No lambda variable")
		}
		if peekToken(tokenstream).category == "dot" {
			*tokenStr += getToken(tokenstream).value
		} else {
			errorMsg("Error: No lambda dot")
		}
		lexpr(tokenstream, tokenStr)
		placePar(tokenstream, tokenStr, true)
		return
	}
	pexpr(tokenstream, tokenStr)
}

// LL grammer pexpr function
func pexpr(tokenstream *[]token, tokenStr *string) {
	if peekToken(tokenstream).category == "variable" {
		*tokenStr += getToken(tokenstream).value
		placePar(tokenstream, tokenStr, false)
	} else if peekToken(tokenstream).category == "lbracket" {
		*tokenStr += getToken(tokenstream).value
		expr(tokenstream, tokenStr)
		if peekToken(tokenstream).category == "rbracket" {
			*tokenStr += getToken(tokenstream).value
		} else {
			errorMsg("Error: invalid grammar, no right bracket")
		}
	} else {
		errorMsg("Error: invalid grammar, pexpr")
	}
}

// places the parentheses for variables or lambda expressions
func placePar(tokenstream *[]token, tokenStr *string, lambda bool) {
	if peekToken(tokenstream).category == "rbracket" {
		return
	}
	if lambda { // put parentheses around lambda expression
		for i := len(*tokenStr) - 1; i >= 0; i-- {
			if (string((*tokenStr)[i]) == "\\") || (string((*tokenStr)[i]) == "\u03bb") {
				*tokenStr = (*tokenStr)[:i] + "(" + (*tokenStr)[i:] + ")"
				return
			}
		}
	}
	pos := 0       // position of end of closed lambda expression
	closed := true // if there is an open lambda expression
	brcount := 0   // count to check of parentheses are open or closed
	if !lambda {   // put parentheses around variable
		for i := 0; i < len(*tokenStr); i++ { // check for open lambda expression
			if string((*tokenStr)[i]) == "(" && string((*tokenStr)[i+1]) == "\\" {
				closed = false
			}
			if !closed {
				if string((*tokenStr)[i]) == "(" {
					brcount++
				} else if string((*tokenStr)[i]) == ")" {
					brcount--
				}
				if brcount == 0 {
					closed = true
					pos = i
				}
			}
		}
		for i := len(*tokenStr) - 1; i >= pos; i-- { // place parentheses
			if (string((*tokenStr)[i]) == "(") || (string((*tokenStr)[i]) == ".") {
				*tokenStr = (*tokenStr)[:i+1] + "(" + (*tokenStr)[i+1:] + ")"
				return
			}
		}
		if pos > 0 { // place parentheses at closing position of lambda expression
			*tokenStr = (*tokenStr)[:pos+1] + "(" + (*tokenStr)[pos+1:] + ")"
		} else {
			*tokenStr = "(" + (*tokenStr)[0:] + ")" // first expression or var
		}
	}
}

// take a token
func getToken(tokenstream *[]token) token {
	if len(*tokenstream) <= 0 {
		errorMsg("Error: no more tokens")
	}
	tokenElement := (*tokenstream)[0]
	(*tokenstream) = (*tokenstream)[1:]
	return tokenElement
}

// look at token
func peekToken(tokenstream *[]token) token {
	if len(*tokenstream) <= 0 {
		errorMsg("Error: no more tokens")
	}
	return (*tokenstream)[0]
}

// print error message and exit with status 1
func errorMsg(msg string) {
	fmt.Println(msg)
	os.Exit(1)
}

// reads file, puts into string, checks for printable characters and newlines
func leesIn(f *os.File) string {
	var str string
	fstream := bufio.NewReader(f)
	rp, _ := regexp.Compile("[[:print:]]")
	for {
		byteIn, eof := fstream.ReadByte()
		if eof != nil {
			if eof == io.EOF {
				break
			}
			errorMsg("Error: reading file")
		}
		if !rp.MatchString(string(byteIn)) { // check for ascii characters
			errorMsg("Error: Non printable character in input")
		}
		str += string(byteIn)
	}
	if len(str) == 0 {
		errorMsg("Error: geen input")
	}
	return str
}
