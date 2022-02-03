/**
 * Classname: Parser
 * @brief Definition off the class Parser.
 * @author Joris Gravesteijn & Valentijn Stokkermans
 * @file parser.h
 * @date 19-11-2021
 **/

/**
 * Description: delcaration of the parser class
 * @author D.V. Stokkermans, S.S. Zeidi, Y. Hu
 * @date 23-11-2021
 * @file hulpfuncties.h
 **/

#ifndef PARSER_H
#define PARSER_H

#include <iostream>
#include <vector>
#include <tuple>  
#include "hulpfuncties.h"
#include "token.h"
#include "node.h"

class Parser
{
   public:
      Parser();
      Parser(std::string input);
      ~Parser();

      //parser functions
      Node* judgement(Node* node);
      Node* expr(Node* node, bool expr1);
      Node* lexpr(Node* node);
      Node* pexpr(Node* node);
      Node* type(Node* node);

      // beta reduction and alpha conversion
      bool findLambda(Node* &node);
      void replaceLambda(std::string var, Node* &body, Node *arg);
      void findUsedLvar(Node *node, std::vector<std::string> &usedLvars);
      void alphaConv(Node* &node, std::vector<std::string> usedLvars);

      // token functions
      Token peekToken();
      Token peekSecondToken();
      Token getToken();
      void makeTokens(std::string input);
      void printTokens();

      // type functions
      void setUsedType(std::string value, Node *node);
      Node* getUsedType(std::string var);
      Node* deriveType(Node *node, Node* &derivedNode);

      // tree functions
      void printTree(Node* node);
      void printSimpleTree(Node* node);
      Node* copyTree(Node* node);
      void deleteTree(Node* node);
      bool compareTree(Node* left, Node *right);
   
   private:
      std::vector<Token> tokens;
      usedTypes typesUsed;
      Node *root;
      Node *derivedTypeRoot;
};

#endif