/**
 * Description: delcaration of the hulpfuncties
 * @author D.V. Stokkermans, S.S. Zeidi, Y. Hu
 * @date 23-11-2021
 * @file hulpfuncties.h
 **/

#ifndef HULPFUNCTIES_H
#define HULPFUNCTIES_H

#include <iostream>
#include <vector>
#include <tuple>
#include <string>
#include "token.h"
#include "node.h"

// custom type to store lvar with type
typedef std::vector< std::tuple <std::string, Node*> > usedTypes;

/**
 * @brief gets user input and returns it
 * @param expl is a string telling what input is expected
 * @return returns the user input as string
 **/
std::string input(std::string expl);

/**
 * @brief prints the error msg to cerr and exits with 1
 * @param errorMsg contains error message
 **/
void error(std::string errorMsg);

// finds unused lval
std::string findNewLval(std::vector<std::string> usedLvars);

// check chars
bool isWS(char c);
bool isNL(char c);
bool isAlphaUpper(char c);
bool isAlphaLower(char c);
bool isAlphaNum(char c);
bool isDash(char c);
bool isRight(char c);
bool isCaret(char c);
bool isColon(char c);
bool isLpar(char c);
bool isRpar(char c);
bool isLambda(char c);

#endif
