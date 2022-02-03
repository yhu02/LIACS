/**
 * Description: Implementation of various assist functions
 * @author D.V. Stokkermans, S.S. Zeidi, Y. Hu
 * @date 23-11-2021
 * @file hulpfuncties.cc
 **/

#include "hulpfuncties.h"



// get input from user
std::string input(std::string expl) {
	std::cout << expl << std::endl;
   std::cin.ignore();
   std::string userInput;
	getline(std::cin, userInput);
	return userInput;
}



// print error and exit 1
void error(std::string errorMsg) {
   std::cerr << "Error: " << errorMsg << std::endl;
   exit(1);
}



// find unused val
std::string findNewLval(std::vector<std::string> usedLvars) {
	bool used = false;
	std::string newVal;
	for (int i = 97; i < 123; i++) { // lower case letters in ascii
		newVal = char(i);
		for (std::string c : usedLvars) { // check if used
			if (newVal == c) {
				used = true;
			}
		}
		if (!used) {
			return newVal;
		}
	}
	error("No unused variable found for alpha conversion");
	return "0";
}

bool isWS(char c) {
	return (c == ' ' || c == '\t');
}
bool isNL(char c) {
	return (c == '\r' || c == '\n');
}
bool isAlphaUpper(char c) {
	return (c >= 'A' && c <= 'Z');
}
bool isAlphaLower(char c) {
	return (c >= 'a' && c <= 'z');
}
bool isAlphaNum(char c) {
	return isAlphaUpper(c) || isAlphaLower(c) || (c >= '0' && c <= '9');
}
bool isDash(char c) {
	return (c == '-');
}
bool isRight(char c) {
	return (c == '>');
}
bool isCaret(char c) {
	return (c == '^');
}
bool isColon(char c) {
	return (c == ':');
}
bool isLpar(char c) {
   return (c == '(');
}
bool isRpar(char c) {
   return (c == ')');
}
bool isLambda(char c) {
   return (c == '\\');
}