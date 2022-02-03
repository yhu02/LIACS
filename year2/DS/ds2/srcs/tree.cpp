#include "../include/tree.h"

//Constructor
Tree::Tree(Node* top){
    divisionByZero = false;
    this->top = top;
    this->nodeI = 0;
}

//Set the root of the tree
bool Tree::setTop(Node* top){
    divisionByZero = false;
    this->top = top;

    if(top == nullptr){
        return false;
    }else{
        return true;
    }
}

//Get the current root of the tree
Node* Tree::getTop() const{
    return this->top;
}

//Destructor
Tree::~Tree(){
    deleteTree(this->getTop(), 3);
}

//Print infix notation
void Tree::print(Node* node) const{
    if(node){
        if (*node == "sin" || *node == "cos"){
            std::cout << node->getToken().getValue();
            std::cout << "("; print(node->left); std::cout << ")";
        }else if(*node == "/"){
            std::cout<<"("; print(node->left); std::cout<<")";
            std::cout << node->getToken().getValue();
            std::cout<<"("; print(node->right); std::cout<<")";
        }else if(*node == "*"){
            if(*node->left == TYPE::OPERATOR && *node->left != *node){
                std::cout<<"("; print(node->left); std::cout<<")";
            }else{
                print(node->left);
            }
            std::cout << node->getToken().getValue();
            if(*node->right == TYPE::OPERATOR && *node->right != *node){
                std::cout<<"("; print(node->right); std::cout<<")";
            }else{
                print(node->right);
            }
        }else{
            print(node->left);
            std::cout << node->getToken().getValue();
            print(node->right);
        }
    }
}

//Save dot notation to file
void Tree::dot(std::string fileName){
    nodeI = 0;
    std::ofstream output;
    output.open(fileName);
    output << "digraph G {\n";
    output << dotRecursive(top, 0);
    output << "}";
    output.close();
}

//Recursive subfunction of dot to return dot notation
std::string Tree::dotRecursive(Node* node, int parent){
    std::string str;
    nodeI++;
    int currentNode = nodeI;

    str =  '\t' + std::to_string(nodeI) + " [label=\"" + node->getToken().getValue() + "\"]\n";
    if(node != top){
        str +=  '\t' + std::to_string(parent) + " -> " + std::to_string(nodeI) + '\n';
    }
    if(node->left != nullptr){
        str += dotRecursive(node->left, currentNode);
    }
    if(node->right != nullptr){
        str += dotRecursive(node->right, currentNode);
    }
    return str;
};

//Simplify Tree
void Tree::simp(Node* node){
    if(!node)
        return;

    if(node->left)
        simp(node->left);
    if(node->right)
        simp(node->right);
    if(*node == TYPE::OPERATOR){
        treeOperator(node);
    }else if(*node == TYPE::FUNCTION){
        treeTrig(node);
    }else if(*node == TYPE::VARIABLE){
        if(*node == "pi"){
            node->setToken(Token(TYPE::NUMBER, "3.1415926535897"));
        }
    }
}

//Evaluate the tree by substituting x with a value
void Tree::eval(std::vector<Token> tokens){
    Node* newTop = copyTree(this->getTop());
    replaceVariable("x", tokens[1].getValue(), newTop);
    if(divisionByZero){
        std::cout << "Division by zero detected, can't evaluate expression" << std::endl;
        exit(1);
    }
    simp(newTop);
    print(newTop);
    deleteTree(newTop, 3);
    std::cout << std::endl;
}

//Simplify actions based on operator
void Tree::treeOperator(Node* node){
    if(*node->left == TYPE::NUMBER || *node->right == TYPE::NUMBER){
        treeCalculate(node);
    }
    if(*node == "+"){
        treeAddition(node);
    }else if(*node == "*"){
        treeMultiplication(node);
    }else if(*node == "^"){
        treeExponent(node);
    }else if(*node == "/"){
        treeDivision(node);
    }
}

//Simplify exponent
void Tree::treeExponent(Node* node){
    if(*node->right == "0"){// x^0 = 1
        node->setToken(Token(TYPE::NUMBER, "1"));
        deleteTree(node, 2);
    }else if(*node->right == "1"){// x^1 = x
        node->setToken(node->left->getToken());
        
        deleteTree(node, 1);
        if(node->left){
            node->setChildren(copyTree(node->left->left), copyTree(node->left->right), 2);
        }
    }
}

//Simplify multiplication by direct calculation of 2 expressions: node and match
void Tree::treeMultiplicationCalculate(Node* node, Node* nodeParent, Node* match, Node* matchParent){
    int value = checkIdenticalPower(node, match);
    if(value){
        Node* nodeLeft = nullptr;
        Node* nodeRight = nullptr;
        std::string multiplier;
        if(value == 1){// x^n * x^k = x^(n+k)
            multiplier = removeZeroes(std::to_string(ntold(node->right) + ntold(match->right)));
            nodeLeft = copyTree(node->left);
            nodeRight = new Node(nullptr, nullptr, Token(TYPE::NUMBER, multiplier));
        }else if(value == 2){// x^n * x = x^(n+1)
            multiplier = removeZeroes(std::to_string(ntold(node->right) + 1));
            nodeLeft = copyTree(match);
            nodeRight = new Node(nullptr, nullptr, Token(TYPE::NUMBER, multiplier));
        }else if(value == 3){// x * x^n = x^(n+1)
            multiplier = removeZeroes(std::to_string(ntold(match->right) + 1));
            nodeLeft = copyTree(node);
            nodeRight = new Node(nullptr, nullptr, Token(TYPE::NUMBER, multiplier));
        }else if(value == 4){// x * x = x^2
            nodeLeft = copyTree(node);
            nodeRight = new Node(nullptr, nullptr, Token(TYPE::NUMBER, "2"));
        }
        if(nodeParent == matchParent){//x * x = x^2
            matchParent->setToken(Token(TYPE::OPERATOR, "^"));
            matchParent->setChildren(nodeLeft, nodeRight, 2);
        }else{// x * y * x = y * x^2
            if(nodeParent->left == node){
                if(nodeParent->right){
                    nodeParent->setToken(nodeParent->right->getToken());
                    nodeParent->setChildren(copyTree(nodeParent->right->left), copyTree(nodeParent->right->right), 2);
                }
            }else if(nodeParent->right == node){
                if(nodeParent->left){
                    nodeParent->setToken(nodeParent->left->getToken());
                    nodeParent->setChildren(copyTree(nodeParent->left->left), copyTree(nodeParent->left->right), 2);
                }
            }
            match->setToken(Token(TYPE::OPERATOR, "^"));
            match->setChildren(nodeLeft, nodeRight, 2);
        }
    }else{//x * 1 = x
        if(*node == "1"){
            if(nodeParent->left == node){
                nodeParent->setToken(nodeParent->right->getToken());
                if(nodeParent->right){
                    nodeParent->setChildren(copyTree(nodeParent->right->left), copyTree(nodeParent->left->right), 2);
                }else{
                    deleteTree(nodeParent, 2);
                }
            }else if(nodeParent->right == node){
                nodeParent->setToken(nodeParent->left->getToken());
                if(nodeParent->left){
                    nodeParent->setChildren(copyTree(nodeParent->left->left), copyTree(nodeParent->left->right), 2);
                }else{
                    deleteTree(nodeParent, 2);
                }
            }
        }
    }
}

//Simplify multiplication at different depths of tree
void Tree::treeMultiplicationRec(Node* node, Node* nodeParent, Node* match, Node* matchParent, std::string operator_){
    if(node){
        if(*node == operator_){
            if(node->left){
                treeMultiplicationRec(node->left, node, match, matchParent, operator_);
            }
            if(node->right){
                treeMultiplicationRec(node->right, node, match, matchParent, operator_);
            }
        }else if(*node != TYPE::OPERATOR || *node == "^"){
            treeMultiplicationCalculate(node, nodeParent, match, matchParent);
        }
    }
}

//Simplify multiplication
void Tree::treeMultiplication(Node* node){
    Node* zeroNode = findNode(node , "*", TYPE::INVALID, "0");
    if(zeroNode){
        node->setToken(Token(TYPE::NUMBER, "0"));
        deleteTree(node, 2);
    }else{
        treeMultiplicationRec(node->left, node, node->right, node, "*");
        treeMultiplicationRec(node->right, node, node->left, node, "*");
    }
}

//Direct simplification by calculation of numbers
void Tree::treeCalculate(Node* node){
    long double value;

    Node* numberNode;
    Node* numberNode2;

    numberNode = findNode(node->left, node->getToken().getValue(), TYPE::NUMBER);
    numberNode2 = findNode(node->right, node->getToken().getValue(), TYPE::NUMBER);       

    if(numberNode && numberNode2){//Do an operation on 2 numbers
        if(*node == "+"){
            value = ntold(numberNode) + ntold(numberNode2);
        }else if(*node == "-"){
            value = ntold(numberNode) - ntold(numberNode2);
        }else if(*node == "*"){
            value = ntold(numberNode) * ntold(numberNode2);
        }else if(*node == "/"){
            if(ntold(numberNode2) == 0){
                divisionByZero = true;
                return;
            }else{
                value = ntold(numberNode) / ntold(numberNode2);
            }
        }else if(*node == "^" && numberNode == node->left && numberNode2 == node->right){
            value = std::pow(ntold(numberNode), ntold(numberNode2));
        }

        if(numberNode == node->left && numberNode2 == node->right){// 2 * 2 = 4
            node->setToken(Token(TYPE::NUMBER, removeZeroes(std::to_string(value))));
            deleteTree(node, 2);
        }else if(numberNode == node->left){
            numberNode2->setToken(Token(TYPE::NUMBER, removeZeroes(std::to_string(value))));
            node->setToken(node->right->getToken());
            node->setChildren(copyTree(node->right->left), copyTree(node->right->right), 2);
        }else if(numberNode2 == node->right){
            numberNode->setToken(Token(TYPE::NUMBER, removeZeroes(std::to_string(value))));
            node->setToken(node->left->getToken());
            node->setChildren(copyTree(node->left->left), copyTree(node->left->right), 2);
        }
    }
}

//Simplify addition
void Tree::treeAddition(Node* node){
    if(*node->left == "0" ){// 0 + x = x
        node->setToken(node->right->getToken());

        if(node->right){
            node->setChildren(copyTree(node->right->left), copyTree(node->right->right), 2);
        } else {
            deleteTree(node, 2);
        }
    }else if(*node->right == "0"){// x + 0 = x
        node->setToken(node->left->getToken());

        if(node->left){
            node->setChildren(copyTree(node->left->left), copyTree(node->left->right), 2);
        } else {
            deleteTree(node, 2);
        }
    } else if (checkIdenticalMinus(node->left, node->right)){// x - x = 0
        node->setToken(Token(TYPE::NUMBER, "0"));
        deleteTree(node, 2);
    } else if (checkSameExpression(node->left, node->right)){// x + x = x * 2
        node->setToken(Token(TYPE::OPERATOR, "*"));
        deleteTree(node, 1);
        Node* nodeRight = new Node(nullptr, nullptr, Token(TYPE::NUMBER, "2"));
        node->setChildren(nullptr, nodeRight, 1);
    }
}

//Simplify division
void Tree::treeDivision(Node* node){
    if(*node->right == "0"){// x / 0 = error
        this->divisionByZero = true;
    }else if(*node->left == "0"){// 0 / x = 0
        node->setToken(Token(TYPE::NUMBER, "0"));
        deleteTree(node, 2);
    }else if(checkSameExpression(node->left, node->right)){// x / x = 0
        node->setToken(Token(TYPE::NUMBER, "1"));
        deleteTree(node, 2);
    }
}

//Simplify trigonometric functions to number
void Tree::treeTrig(Node* node){          
    long double value;
    if(*node->left == TYPE::NUMBER){
        if(*node == "sin"){// sin(pi) = 0
            value = std::sin(ntold(node->left));
        }else if(*node == "cos"){// cos(pi) = -1
            value = std::cos(ntold(node->left));
        }

        if(value > -0.01 && value < 0.01){//Rounding error fix
            value = 0;
        }

        node->setToken(Token(TYPE::NUMBER, removeZeroes(std::to_string(value))));
        deleteTree(node, 0);
    }
}

//Differentiate tree
void Tree::diff(Node* node, std::string variable, Node* parent){    
    if(!node)
        return;

    if(*node == TYPE::NUMBER){// constant = 0
        node->setToken(Token(TYPE::NUMBER, "0"));
    }else if(*node == TYPE::VARIABLE){
        if(*node == variable){// x = 1
            node->setToken(Token(TYPE::NUMBER, "1"));
        }else{// y = 0
            node->setToken(Token(TYPE::NUMBER, "0"));
        }
    }else if(*node == TYPE::OPERATOR){//x^n = n * x^(n-1)
        treeDiffOperator(node, variable);
    }else if(*node == TYPE::FUNCTION){//sin = cos etc.
        treeDiffFunction(node, variable, parent);
    }
}

//Differentiate actions based on operator
void Tree::treeDiffOperator(Node* node, std::string variable){
    if(*node == "+"){                                                                
        if(node->left){
            diff(node->left, variable, node);
        }
        if(node->right){
            diff(node->right, variable, node);
        }
    }else if(*node == "^"){
        treeDiffExponent(node, variable);
    }else if(*node == "*"){
        treeDiffMultiplication(node, variable);
    }else if(*node == "/"){
        treeDiffDivision(node, variable);
    }
}

//Differentiate division quotient rule
void Tree::treeDiffDivision(Node* node, std::string variable){
    Tree subTreeLeft(copyTree(node->left));
    Tree subTreeRight(copyTree(node->right));;

    Tree subTreeLeftDiff(copyTree(node->left));
    Tree subTreeRightDiff(copyTree(node->right));
    
    subTreeLeftDiff.diff(subTreeLeftDiff.getTop(), variable);
    subTreeRightDiff.diff(subTreeRightDiff.getTop(), variable);
    
    Node* newNode1 = new Node(copyTree(subTreeLeftDiff.getTop()), copyTree(subTreeRight.getTop()), Token(TYPE::OPERATOR, "*"));
    Node* newNode2 = new Node(copyTree(subTreeLeft.getTop()), copyTree(subTreeRightDiff.getTop()), Token(TYPE::OPERATOR, "*"));
    Node* twoNode = new Node(nullptr, nullptr, Token(TYPE::OPERATOR, "2"));

    Node* numerator = new Node(newNode1, newNode2, Token(TYPE::OPERATOR, "-"));
    Node* denominator = new Node(copyTree(node->right), twoNode, Token(TYPE::OPERATOR, "^"));

    node->setChildren(numerator, denominator, 2);
    node->setToken(Token(TYPE::OPERATOR, "/"));
}

//Differentiate trigonometric functions if it contains the differentiated variable
void Tree::treeDiffFunction(Node* node, std::string variable, Node* parent){
    Node* xNode = findNode(node, "", TYPE::INVALID, variable);
    if(xNode){// sin(x) = cos(x) * /(x) = cos(x) * 1
        Tree subTree;
        Node* tmp = copyTree(node->left);
        treeDiffTrig(node, parent);

        subTree.setTop(tmp);
        subTree.diff(subTree.getTop(), variable);
        node->setChildren(copyTree(node), copyTree(subTree.getTop()), 2);
        node->setToken(Token(TYPE::OPERATOR, "*"));
    }else{// sin(1) = 0
        node->setToken(Token(TYPE::NUMBER, "0"));
        node->setChildren(nullptr, nullptr, 2);
    }
}

//Differentiate multiplication chain rule
//f(x)g(x) = f/(x) * g(x) + f(x) * g/(x)
void Tree::treeDiffMultiplication(Node* node, std::string variable){
    Tree subTreeLeft(copyTree(node->left));
    Tree subTreeRight(copyTree(node->right));;

    Tree subTreeLeftDiff(copyTree(node->left));
    Tree subTreeRightDiff(copyTree(node->right));
    
    subTreeLeftDiff.diff(subTreeLeftDiff.getTop(), variable);
    subTreeRightDiff.diff(subTreeRightDiff.getTop(), variable);
    
    Node* newNode1 = new Node(copyTree(subTreeLeftDiff.getTop()), copyTree(subTreeRight.getTop()), Token(TYPE::OPERATOR, "*"));
    Node* newNode2 = new Node(copyTree(subTreeLeft.getTop()), copyTree(subTreeRightDiff.getTop()), Token(TYPE::OPERATOR, "*"));
    
    node->setChildren(newNode1, newNode2, 2);
    node->setToken(Token(TYPE::OPERATOR, "+"));
}

//Differentiate terms with exponents
void Tree::treeDiffExponent(Node* node, std::string variable){
    Node* xNode = findNode(node->left, "", TYPE::INVALID, variable);
    if(xNode && *node->right == TYPE::NUMBER){//x^n = n * x^(n-1) 
        std::string value = removeZeroes(std::to_string(ntold(node->right)));
        std::string value2= removeZeroes(std::to_string(ntold(node->right) - 1));
        Node* tmp = copyTree(node->left);

        Node* constantNode = new Node(nullptr, nullptr, Token(TYPE::NUMBER, value));

        node->setChildren(constantNode, copyTree(node), 2);
        node->setToken(Token(TYPE::OPERATOR, "*"));
        node->right->right->setToken(Token(TYPE::NUMBER, value2));

        Tree subTree;
        subTree.setTop(tmp);
        subTree.diff(subTree.getTop(), variable);
        node->right->setChildren(copyTree(node->right), copyTree(subTree.getTop()), 2);
        node->right->setToken(Token(TYPE::OPERATOR, "*"));
    }
}

//Differentiate trigonometric functions
void Tree::treeDiffTrig(Node* node, Node* parent){
    if(*node == "sin"){//sin = cos
        node->setToken(Token(TYPE::FUNCTION, "cos"));
    }else if(*node == "cos"){// cos = sin
        node->setToken(Token(TYPE::FUNCTION, "sin"));

        if(parent && *parent == "*"){//cos = -1 * sin and -1 * cos = 1 * sin
            Node* numberNode = findNode(parent->left, "*", TYPE::NUMBER);
            Node* numberNode2 = findNode(parent->right, "*", TYPE::NUMBER);
            std::string newValue;;
            if(numberNode){
                newValue = removeZeroes(std::to_string(ntold(numberNode) * -1));
                numberNode->setToken(Token(TYPE::NUMBER, newValue));
            }else if(numberNode2){
                newValue = removeZeroes(std::to_string(ntold(numberNode2) * -1));
                numberNode2->setToken(Token(TYPE::NUMBER, newValue));
            }
        }else if(!parent || (parent && *parent != "*")){
            Node* minusOneNode = new Node(nullptr, nullptr, Token(TYPE::NUMBER, "-1"));
            node->setChildren(minusOneNode, copyTree(node), 2);
            node->setToken(Token(TYPE::OPERATOR, "*"));
        }
    }
}