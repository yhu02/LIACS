/**
 * Description: delcaration of the node class
 * @author D.V. Stokkermans, S.S. Zeidi, Y. Hu
 * @date 23-11-2021
 * @file hulpfuncties.h
 **/

#ifndef NODE_H
#define NODE_H

#include "token.h"

class Node{
    public:
        Node(Token token);

        void setToken(Token token);
        void setTokenValue(std::string val);
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