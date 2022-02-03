#include "../include/functions.h"

//Replace the token inside all nodes of a tree where the token inside node matches variable
void replaceVariable(std::string variable, std::string number, Node* node){
    if(*node == variable)
        node->setToken(Token(TYPE::NUMBER, number));
    
    if(node->left){
        replaceVariable(variable, number, node->left);
    }
    if(node->right){
        replaceVariable(variable, number, node->right);
    }
}

//Find a type inside a tree of which the parent node contains an operator and is equal to the string inside token
//TYPE::INVALID will instead match the string inside token
Node* findNode(Node* node, std::string operator_, TYPE type, std::string match){
    Node* tmp;
    if(node){
        if(operator_ == ""){
            if(*node == match){
                return node;
            }
        }else{
            if(match != "" && *node == match){
                return node;
            }else if(match == "" && *node == type){
                return node;
            //Operators are not the same so no valid number can be found in subtree
            }else if(node->getToken().getValue() != operator_){
                return nullptr;
            }
        }
        if(node->left){
            tmp = findNode(node->left, operator_, type, match);
            if(tmp){
                return tmp;
            }
        }
        if(node->right){
            tmp = findNode(node->right, operator_, type, match);
            if(tmp){
                return tmp;
            }
        }
    }
    return nullptr;
}

//Convert the string inside a token inside a node to a long double
long double ntold(Node* node) {
    return std::stold(node->getToken().getValue());
}

//Remove trailing zeroes in a string
std::string removeZeroes(std::string value){
    value.erase(value.find_last_not_of('0') + 1, std::string::npos );
    if(value[value.size() - 1] == '.'){
        value = value.substr(0,value.size() - 1);
    }
    return value;
}

//mode = 0 deletes left node, mode = 1 deletes right node,
//mode = 2 deletes children nodes, mode = 3 deletes current node
void deleteTree(Node* node, int mode){
    if(node){
        if(mode == 0 || mode == 2 || mode == 3){
            if(node->left){
                deleteTree(node->left, 2);
                delete node->left;
                node->left = nullptr;
            }
        }
        if(mode == 1 || mode == 2 || mode == 3 ){
            if(node->right){
                deleteTree(node->right, 2);
                delete node->right;
                node->right = nullptr;
            }
        }
        if(mode == 3){
            deleteTree(node, 2);
            delete node;
        }
    }
}

//Convert the expression to a syntactically correct mathematic expression string
std::string stringify(Expression expr){
    std::string str;
    bool flagOperator = false;
    bool flagTerm = false;

    if(expr.token != TYPE::OPERATOR){
        str += expr.token.getValue();
        if(expr.token == TYPE::FUNCTION){
            str += "(";
        }
    }

    while(expr.expressions.size() >= 1){
        if(flagOperator){                   //Add operator in between terms
            str += expr.token.getValue();
        }
        //Expression has more than 1 term, therefore needs parentheses
        if((expr.token == TYPE::OPERATOR) && expr.expressions.front().expressions.size() > 1){
            if(!(expr.expressions.front().token == "/" || expr.token == "+")){
                str += "(";
                flagTerm = true;
            }
        }
        str += stringify(expr.expressions.front());

        //Expression has more than 1 term, therefore needs parentheses
        if(flagTerm){
            str += ")";
            flagTerm = false;
        }
        expr.expressions.erase(expr.expressions.begin());
        flagOperator = true;
    }
 
    if(expr.token == TYPE::FUNCTION){
        str += ")";
    }
    return str;
}

//Make tokens for parsing
std::vector<Token> makeTokens(std::string str){
    std::vector<Token> tokens;
    char c;
    for(size_t pos = 0; pos < str.size(); pos++){
        std::string value = "";
        std::string error = "";
        TYPE type;
        if(pos < str.size()){
            c = str[pos];
            if(c == '-' || (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || (c >= '0' && c <= '9')){
                if(c == '-'){
                    value += c;
                    pos++;
                }
                c = str[pos];
                if((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z')){
                    value += c;
                    pos++;
                    c = str[pos];
                    while((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z')){
                        value += c;
                        pos++;
                        c = str[pos];
                    }
                    pos--;
                    type = TYPE::VARIABLE;
                }else if(c >= '0' && c <= '9'){
                    value += c;
                    pos++;
                    c = str[pos];
                    type = TYPE::NUMBER;
                    if((c >= '0' && c <= '9') || c == '.'){
                        while((c >= '0' && c <= '9') || c == '.'){
                            value += c;
                            pos++;
                            c = str[pos];
                        }
                    }
                    pos--;
                }else if(c == '\n' || c == '\t' || c == ' ' || c == '\v' || c == '('){
                    type = TYPE::OPERATOR;
                    pos--;
                }else{
                    type = TYPE::INVALID;
                    error = "INVALID CHARACTER AFTER -";
                }
            }else if(c == '+' || c == '/' || c == '*' || c == '^'){
                value += c;
                type = TYPE::OPERATOR;
            }else if(c == ' ' || c == '\t' || c == '\v' || c == '\r'){
                value += " ";
                pos++;
                c = str[pos];
                while(c == ' ' || c == '\t' || c == '\v' || c == '\r'){
                    pos++;
                    c = str[pos];
                }
                pos--;
                type = TYPE::WHITESPACE;
                continue;
            }else if(c == '\n'){
                type = TYPE::NEWLINE;
                value += c;
                continue;
            }else if(c == '('){
                type = TYPE::LBRACE;
                value += c;
            }else if(c == ')'){
                type = TYPE::RBRACE;
                value += c;
            }else{
                type = TYPE::INVALID;
                error = "Invalid start character detected";
                value += c;
            }
            tokens.push_back(Token(type, value, error));
        }
    }

    tokens.push_back(Token(TYPE::EOF_, "EOF"));
    return tokens;
}

//Make tokens for the dot function
std::vector<Token> makeTokensFileName(std::string str){
    std::vector<Token> tokens{};
    char c;
    int dotCount = 0;
    for(size_t pos = 0; pos < str.size(); pos++){
        std::string value = "";
        std::string error = "";
        TYPE type = TYPE::INVALID;
        if(pos < str.size()){
            c = str[pos];
            if((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') 
            ||  (c >= '0' && c <= '9')){
                value += c;
                pos++;
                c = str[pos];

                while((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') 
                    ||(c >= '0' && c <= '9') || (c == '.')){
                    if(c == '.')
                        dotCount++;

                    value += c;
                    pos++;
                    c = str[pos];
                }
                pos--;
                type = TYPE::FILENAME;
            }else if(c == ' ' || c == '\t' || c == '\v' || c == '\r'){
                value += " ";
                pos++;
                c = str[pos];
                while(c == ' ' || c == '\t' || c == '\v' || c == '\r'){
                    pos++;
                    c = str[pos];
                }
                pos--;
                type = TYPE::WHITESPACE;
                continue;
            }else if(c == '\n'){
                type = TYPE::NEWLINE;
                value += c;
                continue;
            }else{
                value += c;
                error = "Invalid start character detected";
            }
        }
        //Doesn't start or end with a '.' and has at least 1 '.'
        if(dotCount != 1 || value[0] == '.' || value[value.size()-1] == '.'){
            type = TYPE::INVALID;
            error = "Invalid filename";
        }
        tokens.push_back(Token(type, value, error));
    }
    tokens.push_back(Token(TYPE::EOF_, "EOF"));
    return tokens;
}


//Change some tokens
void changeTokens(std::vector<Token> &tokens){
    for(size_t pos = 0; pos < tokens.size(); pos++){
        Token token = tokens[pos];
        if(token == TYPE::VARIABLE){
            if(token.getValue().front() == '-'){  //-sin to * -1 sin
                tokens[pos].setValue(token.getValue().substr(1));

                if((tokens[pos].getValue() != "sin" && tokens[pos].getValue() != "cos") && tokens[pos].getValue().size() > 1){
                    tokens[pos] = Token(TYPE::INVALID, tokens[pos].getValue());
                    tokens[pos].setError("Variable has more than 1 character");
                }else{
                    tokens.insert(tokens.begin() + pos, Token(TYPE::NUMBER, "-1"));
                    tokens.insert(tokens.begin() + pos, Token(TYPE::OPERATOR, "*"));
                }
            }
            if(token == "sin" || token == "cos"){
                tokens[pos] = Token(TYPE::FUNCTION, tokens[pos].getValue());
            }else if(token == "print" || token == "diff" || token == "eval"
                  || token == "simp"  || token == "end" || token == "exp"){
                tokens[pos] = Token(TYPE::KEYWORD, tokens[pos].getValue());
            }
        }else if(token == TYPE::NUMBER){
            int dotCount = 0;

            for(size_t pos2 = 0; pos2 < token.getValue().size(); pos2++){
                if(token.getValue()[pos2] == '.'){
                    dotCount++;
                    if(dotCount > 1){
                        tokens[pos] = Token(TYPE::INVALID, tokens[pos].getValue());
                        tokens[pos].getError() = "A number contains more than one dot";
                    }
                }
            }
        }
    }
}

//Change 1 - x to 1 - 1*x
void changeMinus(Node* node){
    if(!node){
        return;
    }
   
    if(node->left){
        changeMinus(node->left);
    }
    if(node->right){
        changeMinus(node->right);
    }

    if(*node == "-"){
        if(*node->right == TYPE::VARIABLE || *node->right == TYPE::FUNCTION){
            Node* minusOneNode = new Node(nullptr, nullptr, Token(TYPE::NUMBER, "-1"));
            Node* multNode = new Node(minusOneNode, copyTree(node->right), Token(TYPE::OPERATOR, "*"));

            node->setToken(Token(TYPE::OPERATOR, "+"));
            node->setChildren(nullptr, multNode, 1);
        }
    }
}
//Check for invalid tokens
void checkTokens(std::vector<Token> const &tokens){
    for(size_t pos = 0; pos < tokens.size(); pos++){
        if(tokens[pos].getType() == TYPE::INVALID){
            std::cout << "TOKEN [" << pos << "]: " << tokens[pos].getValue() << " //" << tokens[pos].getError() << std::endl;
            exit(1);
        }
    }
}

//Make tokens from string
std::vector<Token> lex(std::string str){
    std::vector<Token> tokens = makeTokens(str);
    changeTokens(tokens);
    checkTokens(tokens);
    return tokens;
}

//Parse tokens and return tree
Node* parse(std::vector<Token> tokens){
    grabToken(tokens);
    Node* node = makeTree(tokens);
    changeMinus(node);

    if(tokens.size() > 1){
        deleteTree(node, 3);
        return nullptr;
    } 
    return node;
}

//Return a token and remove it from vector
Token grabToken(std::vector<Token> &tokens){
    Token token = tokens.front();
    tokens.erase(tokens.begin());
    return token;
}

//Make the tree containing the expression
Node* makeTree(std::vector<Token> &tokens){
    Node* node = new Node;
    node->setToken(grabToken(tokens));
    
    if(*node == TYPE::OPERATOR){
        node->setChildren(makeTree(tokens), nullptr, 0);
        node->setChildren(nullptr, makeTree(tokens), 1);
    }else if(*node == TYPE::NUMBER || *node == TYPE::VARIABLE){
    }else if(*node == TYPE::FUNCTION){
        node->setChildren(makeTree(tokens), nullptr, 0);
    }else{
        std::cout << node->getToken().getValue() << " : " << " ILLEGAL OR MISSING TOKEN" << std::endl;
        exit(1);
    }
    return node;
}

//Convert tree structure to a more parsable string format
//x + 1 + 2 + (y * z) = +(x)(1)(2)(*(y)(z))
std::string parenthesize(Node* node){
    std::string str;

    if(node){
        str += node->getToken().getValue();
        if(node->left){
            if(node->left->getToken() == node->getToken()
                && (node->getToken() == "+" || node->getToken() == "*")){
                str += parenthesize(node->left).substr(1);
            }else{
                str += "(" + parenthesize(node->left) + ")";
            }
        }
        if(node->right){
            if(node->right->getToken() == node->getToken()
                && (node->getToken() == "+" || node->getToken() == "*")){
                str += parenthesize(node->right).substr(1);
            }else{
                str += "(" + parenthesize(node->right) + ")";
            }
        }
    }

    return str;
}

//Make expression class from tree
Expression expressionize(Node* node){
    std::string str = parenthesize(node);
    std::vector<Token> tokens = lex(str);
    return expressionizeRec(tokens);
}

//Store a mathematic expression inside an Expression datatype
Expression expressionizeRec(std::vector<Token> &tokens){
    Expression expr;
    Token token = grabToken(tokens);
    expr.token = token;
    while(tokens.size() >= 1){
        if(token == TYPE::VARIABLE || token == TYPE::NUMBER){
        }else if(token == TYPE::LBRACE){
            expr.expressions.push_back(expressionizeRec(tokens));
        }else if(token == TYPE::RBRACE){
            return expr;
        }else{
        }
        token = grabToken(tokens);
    }
    return expr;
}

//Comparison by elimination of common terms using backtracking
bool compare(Expression &expr, Expression &expr2){
    size_t pos2 = 0;
    bool matchFound;

    std::vector<Expression> oldExpressions;
    std::vector<Expression> oldExpressions2;

    if(expr.token.getValue() == expr2.token.getValue()){
        while(expr.expressions.size() >= 1){
            matchFound = false;
            pos2 = 0;

            while(pos2 < expr2.expressions.size() && expr.expressions.size() >= 1){
                if(expr.expressions.front().token == expr2.expressions[pos2].token){
                    if(compare(expr.expressions.front(), expr2.expressions[pos2])){
                        matchFound = true;
                        oldExpressions.push_back(expr.expressions.front());
                        oldExpressions2.push_back(expr2.expressions[pos2]);

                        expr.expressions.erase(expr.expressions.begin());
                        expr2.expressions.erase(expr2.expressions.begin() + pos2);
                    }
                }
                pos2++;
            }
            if(matchFound == false){        //Reset expression to previous state by restoring actions
                expr.expressions.insert(expr.expressions.end(), oldExpressions.begin(), oldExpressions.end());
                expr2.expressions.insert(expr2.expressions.end(), oldExpressions2.begin(), oldExpressions2.end());

                return false;
            }
        }
        return true;
    }
    return false;
}

//Checks if 2 expressions are semantically the same; (x + 1) * (z * y) = (y * z) * (1 + x) 
bool checkSameExpression(Node* node, Node* node2){
    Expression expr = expressionize(node);
    Expression expr2 = expressionize(node2);

    if(compare(expr, expr2)){
        return true;
    }

    return false;
}

//Check if two expressions have common terms
int checkIdenticalPower(Node* node, Node* node2){
    if(*node == "^" && *node2 == "^"){
        if(checkSameExpression(node->left, node2->left))
            return 1;
    }else if(*node == "^"){
        if(checkSameExpression(node->left, node2))
            return 2;
    }else if(*node2 == "^"){
        if(checkSameExpression(node, node2->left))
            return 3;
    }else{
        if(checkSameExpression(node, node2))
            return 4;
    }
    return 0;
}

//Check if two subtrees are identical excluding minus operator
bool checkIdenticalMinus(Node* node, Node* node2){
    bool value = false;

    if(*node == "*"){
        if(checkSameExpression(node->right, node2)
        || checkSameExpression(node->left, node2))
            value = true;
    }else if(*node2 == "*"){
        if(checkSameExpression(node, node2->right)
        || checkSameExpression(node, node2->left))
            value = true;
    }
    return value;
}

//Copy all nodes in a tree
Node* copyTree(Node* node){
    if(node){
        Node* newNode = new Node(nullptr, nullptr, node->getToken());
        if(node->left){
            newNode->left = copyTree(node->left);
        }
        if(node->right){
            newNode->right = copyTree(node->right);
        }
        return newNode;
    }
    return nullptr;
}