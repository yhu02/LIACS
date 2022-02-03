/**
 * Description: Implementation of the parser class
 * @author D.V. Stokkermans, S.S. Zeidi, Y. Hu
 * @date 23-11-2021
 * @file parser.cc
 **/

#include "parser.h"

// constructor
Parser::Parser(std::string input)
{
   makeTokens(input);
   root = judgement(root); // parse
   // aplha and beta
   for (int i  = 0; i < 50; i++) { // max 50 times to prevent infinite loop
      if (!findLambda(root)) {
         break;
      }
   }
   deriveType(root->left, derivedTypeRoot); // get left hand type
   if (compareTree(derivedTypeRoot, root->right)) { // compare types
      printTree(root);
      std::cout << "\n\n";
   } else {
      error("no equal type");
   }
}



// destructor
Parser::~Parser()
{
   deleteTree(root);
   deleteTree(derivedTypeRoot);
}



// derive type from the left side of the judgement
Node* Parser::deriveType(Node *node, Node* &derivedNode) {
   if (node == nullptr) {
      return node;
   }
   if (node->getToken().getType() == TYPE::LAMBDA) {
      derivedNode = new Node(Token(TYPE::ARROW, "->"));
      derivedNode->left = copyTree(node->left->right); // left
      setUsedType(node->left->left->getToken().getValue(), 
                     copyTree(node->left->right)); // save lvar with type
      if (node->right->getToken().getType() == TYPE::APPLY) {
         if (compareTree(getUsedType(node->right->left->getToken().getValue()),
             node->left->right) &&
             compareTree(getUsedType(node->right->right->getToken().getValue()),
             node->left->right->left)) { // check if lvar matches types
                  derivedNode->right = copyTree(node->left->right->right);
                  return derivedNode;
             } else {
                error("incorrect application while deriving type");
             }
      } else if (node->right->getToken().getType() == TYPE::LVAR) { // get type from lvar
         derivedNode->right = getUsedType(node->right->getToken().getValue()); // right
      } else {
         derivedNode->right = deriveType(node->right, derivedNode->right); // right
      }
   }
   return derivedNode;
}



// save used type in typesUsed
void Parser::setUsedType(std::string value, Node *node) {
   typesUsed.push_back(std::tuple<std::string, Node*>(value, node));
}



// get used type for var from usedTypes
Node* Parser::getUsedType(std::string var) {
   // laatst toegevoegd eerst checken, achter naar voren
   for( auto rit = std::rbegin(typesUsed); rit != std::rend(typesUsed); ++rit ) {
      if (std::get<0>(*rit) == var) { // als we al een type hebben voor de lvar
         return std::get<1>(*rit); // return type
      }
   }
   error("unknown type");
   return nullptr;
}



// judgement part of the parser, judgement ::= expr ':' type
Node* Parser::judgement(Node* node)
{
   node = new Node(Token(TYPE::COLON, ":")); // alvast plaatsen
   node->left = expr(node->left, false); // left
   if (peekToken().getType() == TYPE::COLON) {
      getToken(); // al geplaatst, niets doen
      node->right = type(node->right); // right
   } else {
      error("parser expected ':', got: " + peekToken().getValue());
   }
   return node;
}



// expr and expr' part of the parser, expr ::= lexpr expr', expr' ::= lexpr expr' | nothing
Node* Parser::expr(Node* node, bool expr1)
{
   if (((peekToken().getType() == TYPE::LVAR ||
       peekToken().getType() == TYPE::LAMBDA ||
       peekToken().getType() == TYPE::LPAR) && expr1) || !expr1)  {
          node = new Node(Token(TYPE::APPLY, "*"));
          node->left = lexpr(node->left); // left
          node->right = expr(node->right, true); // right
          if (node->right == nullptr) {
             Node *hulp = copyTree(node->left);
             deleteTree(node);
             node = hulp;
          } // if expr = nullptr, root = left
   }
   return node;
}



// lexpr part of the parser, lexpr ::= pexpr | '\' lvar '^' type expr'
Node* Parser::lexpr(Node* node)
{
   if (peekToken().getType() == TYPE::LAMBDA) {
      node = new Node(getToken()); // root
      if (peekToken().getType() == TYPE::LVAR) {
         node->left = new Node(Token(TYPE::CARET, "^")); // alvast plaatsen
         node->left->left = new Node(getToken()); // root left left
      } else {
         error("parser expected 'lvar', got: " + peekToken().getValue());
      }
      if (peekToken().getType() == TYPE::CARET) {
         getToken(); // root left
      } else {
         error("parser expected '^', got: " + peekToken().getValue());
      }
      node->left->right = type(node->left->right); // root left right
      if (peekToken().getType() == TYPE::DOT)
         getToken();
      node->right = expr(node->right, true); // root right
   } else {
      node = pexpr(node); // root
   }
   return node;
}



// pexpr part of the parser, pexpr ::= lvar | '(' expr ')'
Node* Parser::pexpr(Node* node)
{
   if (peekToken().getType() == TYPE::LVAR) {
      if (peekSecondToken().getType() == TYPE::LVAR) {
         node = new Node(Token(TYPE::APPLY, "*"));
         node->left = new Node(getToken()); // left
         node->right = new Node(getToken()); // right
         while(peekToken().getType() == TYPE::LVAR) { // left recusive
            Node *hulp = node;
            node = new Node(Token(TYPE::APPLY, "*"));
            node->left = hulp;
            node->right = new Node(getToken()); // root right
         }
      } else {
         node = new Node(getToken());
      }
   } else if (peekToken().getType() == TYPE::LPAR) {
      getToken();
      node = expr(node, false); // root
      if (peekToken().getType() == TYPE::RPAR) {
         getToken();
      } else {
         error("parser expected ')',got: " + peekToken().getValue());
      }
   } else {
      error("parser expected pexpr");
   }
   return node;
}



// type part of the parser, type ::= uvar | '(' type ')' | type '->' type
Node* Parser::type(Node* node)
{
   if (peekToken().getType() == TYPE::UVAR) {
      if (peekSecondToken().getType() == TYPE::ARROW) {
         node = new Node(Token(TYPE::ARROW, "->"));
         node->left = new Node(getToken()); // left
         getToken(); // arrow, do nothing
         node->right = type(node->right); // right
      } else {
         node = new Node(getToken());
      }
   } else if (peekToken().getType() == TYPE::LPAR) {
      getToken();
      node = type(node);
      if (peekToken().getType() == TYPE::RPAR) {
         getToken();
      } else {
         error("parser expected ')', got: " + peekToken().getValue());
      }
      if (peekToken().getType() == TYPE::ARROW) { // exra arrow after ')'
         Node* hulp = copyTree(node);
         deleteTree(node);
         node = new Node(getToken());
         node->left = hulp;
         node->right = type(node->right);
      }
   } else {
      error("parser expected type");
   }
   return node;
}



// return copy of first token in tokenstream
Token Parser::peekToken()
{
   return tokens.front();
}



// return copy of second token in tokenstream
Token Parser::peekSecondToken()
{
   return tokens.at(1);
}



// get first token in tokenstream
Token Parser::getToken()
{
   Token token = tokens.front();
   tokens.erase(tokens.begin());
   return token;
}



// print tokens to debug
void Parser::printTokens()
{
   std::cout << "\n";
   for (size_t i = 0; i < tokens.size(); i++)
   {
      std::cout << " " << i << ": " << tokens.at(i).getValue();
   }
   std::cout << "\n";
}



// make tokens from string
void Parser::makeTokens(std::string input)
{
   std::string value;
   for(u_int8_t i = 0; i < input.size(); i++) {
      value.clear();
      if (isDash(input[i]) && isRight(input[i+1])) {
         tokens.push_back(Token(TYPE::ARROW, "->"));
         i++;
      } else if (isAlphaLower(input[i])) {
         while(isAlphaNum(input[i])) {
            value += input[i];
            i++;
         }
         i--;
         tokens.push_back(Token(TYPE::LVAR, value));
      } else if (isAlphaUpper(input[i])) {
         while(isAlphaNum(input[i])) {
            value += input[i];
            i++;
         }
         i--;
         tokens.push_back(Token(TYPE::UVAR, value));
      } else if (isLpar(input[i])) {
         tokens.push_back(Token(TYPE::LPAR, "("));
      } else if (isRpar(input[i])) {
         tokens.push_back(Token(TYPE::RPAR, ")"));
      } else if (isLambda(input[i])) {
         tokens.push_back(Token(TYPE::LAMBDA, "\\"));
      } else if (isColon(input[i])) {
         tokens.push_back(Token(TYPE::COLON, ":"));
      } else if (isCaret(input[i])) {
         tokens.push_back(Token(TYPE::CARET, "^"));
      } else if (input[i] == '.') {
         tokens.push_back(Token(TYPE::DOT, "."));
      }
   }
   tokens.push_back(Token(TYPE::eof, "eof"));
}



// delete a tree from the given node down
void Parser::deleteTree(Node* node) {
   if (node == nullptr) {
       return;
   }
   if (node->left != nullptr) {
      deleteTree(node->left);
      node->left = nullptr;
   }
   if (node->right != nullptr) {
      deleteTree(node->right);
      node->right = nullptr;
   }
   delete node;
}



// copy a tree from the given node down
Node* Parser::copyTree(Node *node) {
   Node *hulp;
   if (node == nullptr) {
       return node;
   }
   hulp = new Node(node->getToken());
   hulp->left = copyTree(node->left);
   hulp->right = copyTree(node->right);
   return hulp;
}



// compare 2 tree's, true if they are the same
bool Parser::compareTree(Node *left, Node *right) {
   if (left == nullptr && right == nullptr) {
      return true;
   } 
   if (!(left == nullptr) != !(right==nullptr)) { //XOR
      return false;
   }
   if (!compareTree(left->left, right->left)) {
      return false;
   }
   if (!compareTree(left->right, right->right)) {
      return false;
   }
   if (left->getToken() == right->getToken()) { // compare tokens
      return true;
   }
   return false;
}



// find lambda for alpha and beta reduction, pre-order
bool Parser::findLambda(Node* &node) {
   if (node == nullptr) {
       return false;
   }
   std::vector<std::string> usedLvars;
   if (node->getToken().getType() == TYPE::APPLY && 
       node->left->getToken().getType() == TYPE::LAMBDA && // find lambda
       node->right != nullptr) { // with argument
        findUsedLvar(node->right, usedLvars);
        alphaConv(node->left, usedLvars);
        replaceLambda(node->left->left->left->getToken().getValue(), 
                      node->left->right, node->right);
        Node *hulp = copyTree(node->left->right);
        deleteTree(node);
        node = hulp;
        return true;
   }
   if (findLambda(node->left))
     return true;
   if (findLambda(node->right))
     return true;
   return false;
}



// replace lambda body for beta reduction
void Parser::replaceLambda(std::string var, Node* &body, Node *arg) {
   if (body == nullptr) {
       return;
   }
   replaceLambda(var, body->left, arg);
   replaceLambda(var, body->right, arg);
   if (body->getToken().getValue() == var) { // post-order
       deleteTree(body); // replace body tree with arg tree
       body = copyTree(arg);
       return;
   }
}



// find lvar that has been used for alpha conversion
void Parser::findUsedLvar(Node* node, std::vector<std::string> &usedLvars) {
   if (node->getToken().getType() == TYPE::LVAR) {
      usedLvars.push_back(node->getToken().getValue());
   }
   if (node->left != nullptr) {
      findUsedLvar(node->left, usedLvars);
   }
   if (node->right != nullptr) {
      findUsedLvar(node->right, usedLvars);
   }
}



// performs alpha conversion
void Parser::alphaConv(Node* &node, std::vector<std::string> usedLvars) {
   bool used = false;
   if (node->getToken().getType() == TYPE::LVAR) {
      for (std::string c : usedLvars) { // c in usedLvars
         if (node->getToken().getValue() == c) {
            used = true;
         }
      }
      if (used) {
         node->setTokenValue(findNewLval(usedLvars)); // replace lvar
      }
   }
   if (node->left != nullptr) {
      alphaConv(node->left, usedLvars);
   }
   if (node->right != nullptr) {
      alphaConv(node->right, usedLvars);
   }
}



// print tree
void Parser::printTree(Node *node) {
   bool par = false;
   if (node == nullptr) {
       return;
   }
   if (node->getToken().getType() == TYPE::COLON) {
      if (node->left->getToken().getType() == TYPE::LVAR) {
         printTree(node->left);
      } else {
         std::cout << "(";
         printTree(node->left);
         std::cout << ")";
      }
      std::cout << node->getToken().getValue();
      if (node->right->getToken().getType() == TYPE::UVAR) {
         printTree(node->right);
      } else {
         std::cout << "(";
         printTree(node->right);
         std::cout << ")";
      }
   } else if (node->getToken().getType() == TYPE::LAMBDA) {
       std::cout << node->getToken().getValue();
       std::cout << node->left->left->getToken().getValue();
       std::cout << node->left->getToken().getValue();
       if (node->left->right->getToken().getType() == TYPE::ARROW) {
           par = true;
           std::cout << "(";
       }
       printTree(node->left->right);
       if (par) {
           par = false;
           std::cout << ")";
       }
       if (node->right->getToken().getType() == TYPE::APPLY ||
           node->right->getToken().getType() == TYPE::LAMBDA) {
           par = true;
           std::cout << "(";
       } else {
           std::cout << " ";
       }
       printTree(node->right);
       if (par) {
           par = false;
           std::cout << ")";
       }
   } else if (node->getToken().getType() == TYPE::APPLY) {
       if (node->left->getToken().getType() == TYPE::LVAR &&
           node->right->getToken().getType() == TYPE::LVAR) {
               std::cout << node->left->getToken().getValue();
               std::cout << " ";
               std::cout << node->right->getToken().getValue();
       } else {
           if (node->left->getToken().getType() == TYPE::LVAR) {
               std::cout << node->left->getToken().getValue();
           } else {
               std::cout << "(";
               printTree(node->left);
               std::cout << ")";
           }
           if (node->right->getToken().getType() == TYPE::LVAR) {
               std::cout << node->right->getToken().getValue();
           } else {
               std::cout << "(";
               printTree(node->right);
               std::cout << ")";
           }
       }
   } else if (node->getToken().getType() == TYPE::ARROW) {
       if (node->left->getToken().getType() == TYPE::UVAR &&
           node->right->getToken().getType() == TYPE::UVAR) {
               std::cout << node->left->getToken().getValue();
               std::cout << node->getToken().getValue();
               std::cout << node->right->getToken().getValue();
       } else {
           if (node->left->getToken().getType() == TYPE::UVAR) {
               std::cout << node->left->getToken().getValue();
               std::cout << node->getToken().getValue();
           } else {
               std::cout << "(";
               printTree(node->left);
               std::cout << ")";
               std::cout << node->getToken().getValue();
           }
           if (node->right->getToken().getType() == TYPE::UVAR) {
               std::cout << node->right->getToken().getValue();
           } else {
               std::cout << "(";
               printTree(node->right);
               std::cout << ")";
           }
       }
   } else {
       std::cout << node->getToken().getValue();
   }
}



// simple printer to check tree structure
void Parser::printSimpleTree(Node *node) {
   if (node == nullptr) {
      return;
   }
   std::cout << node->getToken().getValue() << "\n";
   if (node->left != nullptr) {
      std::cout << "left\n";
      printSimpleTree(node->left);
      std::cout << "bakc_left\n";
   }
   if (node->right != nullptr) {
      std::cout << "right\n";
      printSimpleTree(node->right);
      std::cout << "back_right\n";
   }
}