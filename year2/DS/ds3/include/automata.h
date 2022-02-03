#ifndef AUTOMATA_H
#define AUTOMATA_H

#include "help_functions.h"
#include <string>
#include <vector>
#include <fstream>

//State class. The char c has to match before you can proceed to the next states in to and to2
class State {
    public:
    State(char c = '\0', int to = -1, int to2 = -1);
    char c;                             //Character required to advance to next states stored in to and to2
    int to;                             //Points to state; -1 for no state
    int to2;                            //Points to state; -1 for no state
    private:
};

class Automata{
    public:
    char peek();                        //Returns current character of input string
    void next();                        //Increases current character of input string
    void setInput(std::string input);   //Sets the input string
    void dot(std::string fileName);     //Writes the current automata in dot notation to a file
    void mat(std::string regexp);       //Matches the regexp with the current automata

    std::vector<State> states;          //Vector with the states of the current sub automata. These will be merged into a bigger automata
    static std::vector<int> start;      //Vector that keeps track of start indices of sub automata when reconnection is needed
    static int count;                   //Number of states in all sub automata

    private:
    static std::string input;           //Current input string
    static int current;                 //Current index of the read character in the string
};

//BNF grammar
//<expr> := <term> [| <expr>]
//<term> := <fact> [term]
//<fact> := <char> [*] | (<expr>) [*] | <expr> *

Automata Expr();
Automata Term();
Automata Fact();
#endif