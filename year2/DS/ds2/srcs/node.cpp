#include "../include/node.h"
#include "../include/functions.h"

Node::Node(Node* left, Node* right, Token token){
    this->left = left;
    this->right = right;
    this->token = token;
}

//mode = 0 set left, mode = 1 set right, mode = 2 set left and right
void Node::setChildren(Node* left, Node* right, int mode){
    if(mode == 0){
        deleteTree(this, 0);
        this->left = left;
    }else if(mode == 1){
        deleteTree(this, 1);
        this->right = right;
    }else if(mode == 2){
        deleteTree(this, 2);
        this->right = right;
        this->left = left;
    }
}

void Node::setToken(Token token){
    this->token = token;
}
Token Node::getToken() const {
    return this->token;
}

//Node operator overloading
bool operator==(Node const node, Node const node2){
    return node.getToken() == node2.getToken();
}
bool operator==(Node const node, std::string const str){
    return node.getToken() == str;
}
bool operator==(Node const node, TYPE const type){
    return node.getToken() == type;
}
bool operator!=(Node const node, Node const node2){
    return node.getToken() != node2.getToken();
}
bool operator!=(Node const node, std::string const str){
    return node.getToken() != str;
}
bool operator!=(Node const node, TYPE const type){
    return node.getToken() != type;
}