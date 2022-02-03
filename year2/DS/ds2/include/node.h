#ifndef NODE_H
#define NODE_H
#include "./constants.h"
#include "./token.h"

class Node{
    public:
    Node(Node* left = nullptr, Node* right = nullptr, Token token = Token(TYPE::INVALID, ""));
    void setChildren(Node* left, Node* right, int mode);
    void setToken(Token token);
    Token getToken() const;
    Node* left;
    Node* right;
    private:
    Token token;
};

//Node operator overloading
bool operator==(Node const node, Node const node2);
bool operator==(Node const node, std::string const str);
bool operator==(Node const node, TYPE const type);
bool operator!=(Node const node, Node const node2);
bool operator!=(Node const node, std::string const str);
bool operator!=(Node const node, TYPE const type);

#endif