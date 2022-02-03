package main

import (
	"bufio"
	"fmt"
	"io"
	"os"
	"regexp"
)

// class for tokens
type Token struct {
	category string
	value    string
}

// class for nodes
type Node struct {
	token Token
	left  *Node
	right *Node
}

// main functie runs program
func main() {
	if len(os.Args) < 2 { // check for command-line argument
		errorMsg("No command-line argument given")
	}
	f, err := os.Open(os.Args[1])
	if err != nil { // check if file opened
		errorMsg("Opening file")
	}
	str := leesIn(f)
	f.Close()
	parse(lex(str))
	println()
	os.Exit(0)
} // main

// lexical analyser defines tokens
func lex(str string) []Token {
	tokens := []Token{
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
		{
			category: "newline",
			value:    "^\\n$",
		},
	}
	return makeTokens(str, tokens)
}

// Turn input string into token slice
func makeTokens(str string, tokens []Token) []Token {
	var tokenstream []Token
	var newToken Token
	for i := 0; i < len(str); i++ {
		flag := true
		for _, token := range tokens {
			match, _ := regexp.MatchString(token.value, newToken.value+string(str[i]))
			if match {
				newToken.category = token.category
				newToken.value += string(str[i])
				flag = false
				break // proceed to next char
			}
		}
		if flag {
			if newToken.category != "" { // check if not invalid token
				tokenstream = append(tokenstream, newToken)
				newToken.value = ""
				newToken.category = ""
				i--
				continue // proceed with the same char in new token
			} else if string(str[i]) != " " {
				errorMsg("illegal input")
			}
		}
	}
	if newToken.category != "" { // last token
		tokenstream = append(tokenstream, newToken)
	}
	newToken = Token{ // add end of file token
		category: "eof",
		value:    "eof",
	}
	tokenstream = append(tokenstream, newToken)
	return tokenstream
}

// reduction loop function, max 50 loops
func reduce(root *Node) *Node {
	if root == nil {
		return root
	}
	found := true
	for i := 0; found && i < 50; i++ {
		root, _, _ = findLambda(root, true)      // alpha conversion
		root, _, found = findLambda(root, false) // beta reduction
		if i == 49 {
			println("reached limit")
			os.Exit(2)
		}
	}
	return root
}

// find a new variable that is not used for alpha conversion
func findNewVal(val []string) string {
	var used bool
	for i := 97; i < 123; i++ {
		for _, char := range val {
			if string(i) == char {
				used = true
			}
		}
		if !used {
			return string(i)
		}
	}
	errorMsg("No unused variable found for alpha reduction")
	return "X"
}

// perform recursive alpha conversion
func alpha(root *Node, val []string) *Node {
	var used bool
	if root.token.category == "variable" {
		for _, char := range val {
			if root.token.value == char {
				used = true
			}
		}
		if used {
			root.token.value = findNewVal(val)
		}
	}
	if root.left != nil {
		root.left = alpha(root.left, val)
	}
	if root.right != nil {
		root.right = alpha(root.right, val)
	}
	return root
}

// find used variables in lambda argument
func findUsedVal(root *Node, val []string) []string {
	if root.token.category == "variable" {
		val = append(val, root.token.value)
	}
	if root.left != nil {
		val = findUsedVal(root.left, val)
	}
	if root.right != nil {
		val = findUsedVal(root.right, val)
	}
	return val
}

// perform beta reduction
func betaReduc(root *Node) (*Node, bool) {
	found := false
	if root.right.token.category == "multiply" { // if multiply arguments
		root, found = beta(root, root.right.left, root.left.left.token.value)
	} else { // if single argument for lambda
		root, found = beta(root, root.right, root.left.left.token.value)
	}
	return root, found
}

// perform beta reduction
func beta(root *Node, arg *Node, sub string) (*Node, bool) {
	found := false
	root.left.right, found = betaSub(root.left.right, arg, sub)
	if found && root.right.token.category == "multiply" {
		root.left = root.left.right
		root.right = root.right.right
	} else {
		root = root.left.right
	}
	return root, found
}

// beta reduction recursive find
func betaSub(root *Node, arg *Node, sub string) (*Node, bool) {
	found := false
	found2 := false
	found3 := false
	if root.token.category == "variable" && root.token.value == sub {
		root = copyArg(root, arg)
		found = true
	}
	if root.left != nil {
		root.left, found2 = betaSub(root.left, arg, sub)
	}
	if root.right != nil {
		root.right, found3 = betaSub(root.right, arg, sub)
	}
	if found || found2 || found3 {
		return root, true
	}
	return root, false
}

// make copy to insert in beta reduction
func copyArg(root *Node, arg *Node) *Node {
	root = addNode(arg.token.category, arg.token.value)
	if arg.left != nil {
		root.left = copyArg(root.left, arg.left)
	}
	if arg.right != nil {
		root.right = copyArg(root.right, arg.right)
	}
	return root
}

// find highest lambda in tree to perform alpha or beta on
func findLambda(root *Node, alph bool) (*Node, bool, bool) {
	lambda := false
	beta := false
	val := make([]string, 0)
	if root.token.category == "multiply" && root.left.token.category == "lambda" {
		if alph {
			val = findUsedVal(root.right, val)
			root.left = alpha(root.left, val)
		} else {
			root, beta = betaReduc(root)
		}
		return root, true, beta
	}
	if root.left != nil {
		root.left, lambda, beta = findLambda(root.left, alph)
		if lambda {
			return root, lambda, beta
		}
	}
	if root.right != nil {
		root.right, lambda, beta = findLambda(root.right, alph)
		if lambda {
			return root, lambda, beta
		}
	}
	return root, false, false
}

// add a node to the tree
func addNode(tokenCat string, tokenVal string) *Node {
	n := Node{}
	n.token.category = tokenCat
	n.token.value = tokenVal
	return &n
}

// print the tree recursively
func printTree(point *Node) {
	if point.token.category == "lambda" {
		print(point.token.value)
		print(point.left.token.value)
		print("(")
		printTree(point.right)
		print(")")
	} else if point.token.category == "multiply" {
		if point.left.token.category == "variable" &&
			point.right.token.category == "variable" {
			printTree(point.left)
			print(" ")
			printTree(point.right)
		} else {
			if point.left.token.category == "variable" {
				printTree(point.left)
			} else {
				print("(")
				printTree(point.left)
				print(")")
			}
			if point.right.token.category == "variable" {
				printTree(point.right)
			} else {
				print("(")
				printTree(point.right)
				print(")")
			}
		}
	} else {
		print(point.token.value)
	}
}

// Parse the tokenstream, and print per line
func parse(tokenstream []Token) {
	var rootNode *Node
	flag := false
	for len(tokenstream) > 0 {
		if flag {
			println()
		}
		printTree(reduce(expr(rootNode, &tokenstream, 1)))

		for tokenstream[0].category == "newline" {
			getToken(&tokenstream)
		}

		if tokenstream[0].category == "eof" {
			getToken(&tokenstream)
		}
		flag = true
	}
}

//  LL grammer for both expr and expr1 function depending on mode. mode = 1 -> expr. mode = 0 -> expr1.
func expr(point *Node, tokenstream *[]Token, mode int) *Node {
	tokenCat := peekToken(tokenstream).category
	if ((tokenCat == "variable" || tokenCat == "lambda" || tokenCat == "lbracket") && mode == 0) || mode == 1 {
		point = addNode("multiply", "*") //  remove if expr is not used
		point.left = lexpr(point.left, tokenstream)
		point.right = expr(point.right, tokenstream, 0)
		if point.right == nil {
			point = point.left
		}
	}
	// can choose to do nothing
	return point
}

// LL grammer lexpr function
func lexpr(point *Node, tokenstream *[]Token) *Node {
	tokenCat := peekToken(tokenstream).category
	if tokenCat == "lambda" {
		point = addNode(peekToken(tokenstream).category, getToken(tokenstream).value)
		if peekToken(tokenstream).category == "variable" {
			point.left = addNode(peekToken(tokenstream).category, getToken(tokenstream).value)
		} else {
			errorMsg("No lambda variable")
		}
		if peekToken(tokenstream).category == "dot" {
			getToken(tokenstream)
		}
		point.right = expr(point.right, tokenstream, 1)
	} else {
		point = pexpr(point, tokenstream)
	}
	return point
}

// LL grammer pexpr function
func pexpr(point *Node, tokenstream *[]Token) *Node {
	if peekToken(tokenstream).category == "variable" {
		if peek2Tokens(tokenstream).category == "variable" { // check for left recursion
			point = addNode("multiply", "*")
			point.left = addNode(peekToken(tokenstream).category, getToken(tokenstream).value)
			point.right = addNode(peekToken(tokenstream).category, getToken(tokenstream).value)
			for peekToken(tokenstream).category == "variable" {
				hulp := point
				point = addNode("multiply", "*")
				point.left = hulp
				point.right = addNode(peekToken(tokenstream).category, getToken(tokenstream).value)
			}
		} else {
			point = addNode(peekToken(tokenstream).category, getToken(tokenstream).value)
		}
	} else if peekToken(tokenstream).category == "lbracket" {
		getToken(tokenstream)
		point = expr(point, tokenstream, 1)
		if peekToken(tokenstream).category == "rbracket" {
			getToken(tokenstream)
		} else {
			errorMsg("invalid grammar, no right bracket")
		}
	} else {
		errorMsg("invalid grammar, pexpr")
	}
	return point
}

// take a token
func getToken(tokenstream *[]Token) Token {
	if len(*tokenstream) <= 0 {
		errorMsg("no more tokens")
	}
	tokenElement := (*tokenstream)[0]
	(*tokenstream) = (*tokenstream)[1:]
	return tokenElement
}

// look at token
func peekToken(tokenstream *[]Token) Token {
	if len(*tokenstream) <= 0 {
		errorMsg("no more tokens")
	}
	return (*tokenstream)[0]
}

// look at token
func peek2Tokens(tokenstream *[]Token) Token {
	if len(*tokenstream) <= 1 {
		errorMsg("no 2 more tokens")
	}
	return (*tokenstream)[1]
}

// print error message and exit with status 1
func errorMsg(msg string) {
	fmt.Fprintln(os.Stderr, "Error: "+msg)
	os.Exit(1)
}

// reads file, puts into string, checks for printable characters and newlines
func leesIn(f *os.File) string {
	var str string
	fstream := bufio.NewReader(f)
	rp, _ := regexp.Compile("[[:print:]]|[\n]")
	for {
		byteIn, eof := fstream.ReadByte()
		if eof != nil {
			if eof == io.EOF {
				break
			} // if
			println(eof)
			os.Exit(1)
		} // if
		if !rp.MatchString(string(byteIn)) {
			println("Non printable character in input")
			os.Exit(1)
		} // if
		str += string(byteIn)
	} // for
	return str
} // leesIn
