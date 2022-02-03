/**
 * Description: Implementation of the node class
 * @author D.V. Stokkermans, S.S. Zeidi, Y. Hu
 * @date 23-11-2021
 * @file node.cc
 **/

#include "node.h"

Node::Node(Token token):
    left(nullptr), right(nullptr), token(token) 
{

}

void Node::setToken(Token token){
    this->token = token;
}

Token Node::getToken() const {
    return this->token;
}

void Node::setTokenValue(std::string val) {
   token.setValue(val);
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