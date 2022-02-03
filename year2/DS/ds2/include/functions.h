#ifndef FUNCTIONS_H
#define FUNCTIONS_H
#include <vector>
#include <string>
#include <cmath>
#include <iostream>
#include "./expression.h"
#include "./node.h"
#include "./token.h"

//Functions for parsing and lexing
Node* parse(std::vector<Token> tokens);
std::vector<Token> lex(std::string str);
Token grabToken(std::vector<Token> &tokens);
Node* makeTree(std::vector<Token> &tokens);
void checkTokens(std::vector<Token> const &tokens);
void changeTokens(std::vector<Token> &tokens);
std::vector<Token> makeTokens(std::string str);
std::vector<Token> makeTokensFileName(std::string str);

//Functions to traverse a tree
Node* findNode(Node* node, std::string operator_ = "", TYPE type = TYPE::INVALID, std::string match = "");
Node* copyTree(Node* node);
void deleteTree(Node* node, int mode);
bool checkIdenticalMinus(Node* node, Node* node2);
int checkIdenticalPower(Node* node, Node* node2);
bool checkSameExpression(Node* node, Node* node2);
bool compare(Expression &expr, Expression &expr2);
void replaceVariable(std::string variable, std::string number, Node* node);

//Functions related to Expression class
Expression expressionize(Node* node);
Expression expressionizeRec(std::vector<Token> &tokens);
std::string stringify(Expression expr);
std::string parenthesize(Node* node);

//Functions to manipulate values
long double ntold(Node* node);
std::string removeZeroes(std::string value);


#endif