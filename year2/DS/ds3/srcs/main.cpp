/*
Datastructuren Programmeeropdracht 3
Auteurs:
Yvo Hu s2962802
Seyed Saqlain Zeidi s2982048
Datum: 25/01/2022
*/
#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>

#include "help_functions.h"
#include "automata.h"

int main(int argc, char** argv) {
    bool debugMode = false;

    if(argc == 2 && std::string(argv[1]) == "d") {
        debugMode = true;
    }

    std::stringstream ss;
    std::string command;
    std::string arg;
    Automata Aut;  

    if(!debugMode) {
        std::cout << "____________________________________________________________________________" << std::endl;
        std::cout << "Regular Expression Matching" << std::endl;
        std::cout << "This program reads in a regular expression, and checks whether a given string" << std::endl;
        std::cout << "matches with the current regular expression" << std::endl;
        std::cout << "____________________________________________________________________________" << std::endl;
        std::cout << "Please enter one of the following commands:" <<std::endl;
        std::cout << "exp 'regexp'" << std::endl;
        std::cout << "dot 'filename'" << std::endl;
        std::cout << "mat 'matching string'" << std::endl;
        std::cout << "end" << std::endl;
        std::cout << "____________________________________________________________________________" << std::endl;
    }

    while(true){
        ss.clear();
        arg.clear();

        std::string tmp;
        std::getline(std::cin, tmp);
        ss << tmp;
        ss >> command;
        ss >> arg;

        if(arg == "" && command != "end") {
            std::cout << "The command has no argument, try again" << std::endl;
            continue;
        }

        if(command == "exp"){
            //Reset automata after every call
            Aut.start.clear();
            Aut.count = 0;
            Aut.setInput(arg);
            Aut = Expr();

            //Final state points to nothing
            Aut.states.back().to = -1;
            Aut.states.back().to2 = -1;
        }else if (Aut.states.size() == 0 && ((command == "dot") || (command == "mat"))) {
            std::cout << "No regular expression has been read in" << std::endl;
            std::cout << "Please enter one by using the command exp 'regexp'" << std::endl;
        }else if(command == "dot"){
            Aut.dot(arg);
        }else if(command == "mat"){
            Aut.mat(arg);
        }else if(command == "end"){
            break;
        }else{
            std::cout << "This is not a valid instruction" << std::endl;
        }
        if(!debugMode) {
            std::cout << "____________________________________________________________________________" << std::endl;
        }
    }

    return 0;
}