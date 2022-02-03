/*
Datastructuren Programmeeropdracht 2 deel 1: Expressies
Auteurs:
Yvo Hu s2962802
Seyed Saqlain Zeidi s2982048
Datum: 01/11/2021
*/

#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <cmath>
#include "../include/constants.h"
#include "../include/node.h"
#include "../include/token.h"
#include "../include/tree.h"
#include "../include/expression.h"



//Menu
void menu(bool debugMode){
    std::string str;
    Tree tree;

    if (!debugMode) {
        std::cout << "____________________________________________________________________________" << std::endl;
        std::cout << "Datastructures 2 | Programmeeropdracht 2: Expressies" << std::endl;
    }
    while(true){
        std::vector<Token> tokens;
        if(debugMode){
            std::getline(std::cin, str);
            tokens = makeTokens(str);
        } else {
            std::cout << "__________________________________________________________________________" << std::endl;
            std::cout << "Enter a valid instruction: exp, dot, print, diff, simp, eval, end" << std::endl;
            std::getline(std::cin, str);
        }
        if(str == "exp" || (debugMode && tokens.front() == "exp")){
            deleteTree(tree.getTop(), 3);
            if(!debugMode){
                std::cout << "Enter a valid sequence of nodes in prefix notation" << std::endl;
                std::getline(std::cin, str);
                str = "exp " + str;
            }
            if(!tree.setTop(parse(lex(str)))){
                std::cout << std::endl << "Couldn't make a tree from the expression" << std::endl;
            }
        }else if(str == "dot" || (debugMode && tokens.front() == "dot")){
            if(tree.getTop() != nullptr){
                std::string fileName;
                
                if(debugMode){
                    tokens = makeTokensFileName(str);
                    tokens.erase(tokens.begin());
                    fileName = tokens.front().getValue();
                }else{
                    std::cout << "Enter a file name for the output" << std::endl;
                    getline(std::cin, fileName);
                }
                tokens = makeTokensFileName(fileName);
                checkTokens(tokens);
                tree.dot(tokens[0].getValue());
            }else{
                std::cout << "No expression has been read" << std::endl;
            }
        }else if(str == "print" || (debugMode && tokens.front() == "print")){
            if(tree.getTop() != nullptr){
                tree.print(tree.getTop());
                std::cout << std:: endl;
            }else{
                std::cout << "No expression has been read" << std::endl;
            }
        }else if(str == "diff" || (debugMode && tokens.front() == "diff")){
            if(tree.getTop() != nullptr){
                tree.diff(tree.getTop(), "x");
            } else {
                std::cout << "No expression has been read" << std::endl;
            }
        }else if(str == "simp" || (debugMode && tokens.front() == "simp")){
            if(tree.getTop() != nullptr){
                tree.simp(tree.getTop());
            } else {
                std::cout << "No expression has been read" << std::endl;
            }
        }else if(str == "end"  || (debugMode && tokens.front() == "end")){
            break;
        }else if(str == "eval"  || (debugMode && tokens.front() == "eval")){
            if(tree.getTop() != nullptr){
                if(!debugMode){
                    std::cout << "Enter a value for x" << std::endl;
                    std::getline(std::cin, str);
                    str = "eval " + str;
                    std::cout << "The result is: ";
                }
                tree.eval(lex(str));
            }else{
                std::cout << "No expression has been read" << std::endl;
            }
        }else{
            std::cout << "This is not a valid instruction" << std::endl;
        }
    }
}

int main(int argc, char **argv) {
    bool debugMode;
    if(argc < 2){
        debugMode = false;
    }else{
        if (std::string(argv[1]) == "d"){
            debugMode = true;
        }
    }
    menu(debugMode);
    return 0;
}