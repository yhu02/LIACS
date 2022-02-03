#include "automata.h"
#include <iostream>

std::string Automata::input = "";
int Automata::current = 0;
int Automata::count = 0;
std::vector<int> Automata::start;

//Save dot notation to file
void Automata::dot(std::string fileName) {
    std::ofstream output;
    output.open(fileName);
    output << "digraph G {\n";
    output << "\trankdir=\"LR\"\n";
    for(size_t i = 0; i < states.size(); i++) {
   
        if (states[i].to != -1) {
            output << '\t' + std::to_string(i + 1) + " -> " +  std::to_string(states[i].to + 1);
            if (isAlphaLower(states[i].c)) {
                output << " [label=\"" << states[i].c << "\"]";
            }
            output << '\n';
        }
        if (states[i].to2 != -1) {
            output << '\t' + std::to_string(i + 1) + " -> " +  std::to_string(states[i].to2 + 1);
            if (isAlphaLower(states[i].c)) {
                output << " [label=\"" << states[i].c << "\"]";
            }
            output << '\n';
        }
    }
    output << "}";
    output.close();
}

void Automata::mat(std::string regexp) {
    bool lastState = false;
    size_t k = 0;                //Current index of character of string
    std::vector<int> closures;   //Vector with all known closures

    //Replace empty string
    size_t pos;
    while ((pos = regexp.find('$')) != std::string::npos) {
        regexp.replace(pos, 1, "");
    }

    //Initialize the vector closures with the start of the automata
    if (states.size() >= 1) {
        closures.push_back(this->start.back());
    }
    
    //Loop until it reaches the end of the matching string
    while (k < regexp.size() + 1) {
        std::vector<int> newClosures;

        //Add all states directly accessible by the current states (those who do not require any input character)
        //For example state a0 -> state a1 requires the character a, but a1 -> a1b0 doesnt require an input character
        for (size_t i = 0; i < closures.size(); i++) {                          
            if (states[closures[i]].c == '\0') {
                if (states[closures[i]].to != -1) {
                    closures.push_back(states[closures[i]].to);
                }
                if (states[closures[i]].to2 != -1) {
                    closures.push_back(states[closures[i]].to2);
                }
            } 
            
            //Remove duplicates to get out of loop when * is used            
            for(size_t m = 0; m < closures.size(); m++) {
                for(size_t n = m + 1; n < closures.size(); n++) {
                    if (closures[m] == closures[n]) {
                        closures.erase(closures.begin() + n);
                    }
                }
            }
        }
        
        //Add all states directly accessible by the current states (those who require the correct input character)
        //And save these in newClosures. At the end, assign closures to be newClosures.
        //For example state a0 -> state a1 requires the character a, and if a is the current character then a1 will be added
        //Stop when k reaches the last iteration because otherwise you will add the next state of the final state
        if (k < regexp.size()) {
            for (size_t i = 0; i < closures.size(); i++) {
                if (states[closures[i]].c == regexp[k]) {
                    if (states[closures[i]].to != -1) {
                        newClosures.push_back(states[closures[i]].to);
                    }
                    if (states[closures[i]].to2 != -1) {
                        newClosures.push_back(states[closures[i]].to2);
                    }
                }
            }
            closures = newClosures;
        }
        k++;
    }
   
    //Check if the last state has been found in any closure
    for (size_t i = 0; i < closures.size(); i++) {
        if (closures[i] == int(states.size() - 1)) {
            lastState = true;
        }
    }
    if (lastState == true) {
        std::cout << "Match\n"; 

    } else {
        std::cout << "No match\n"; 
    }
}

char Automata::peek() {
    return input[current];
}

void Automata::next() {
    current++;
}

State::State(char c, int to, int to2) {
    this->c = c;
    this->to = to;
    this->to2 = to2;
}

void Automata::setInput(std::string input) {
    this->input = input;
    current = 0;
}

Automata operator+(Automata a, Automata b) {
    a.states.insert(a.states.end(), b.states.begin(), b.states.end());
    return a;
}

//<expr>
Automata Expr() {
    Automata Aut1;
    int start2;
    int start1;

    Aut1 = Term();

    //Handle | state
    if (isBar(Automata().peek())) {
        Automata().next();
        Automata Aut2 = Expr();

        //Point ends of choice a and b to the end of | state
        Aut1.states.back().to = Automata().count + 1;    
        Aut2.states.back().to = Automata().count + 1;    
        Aut1 = Aut1 + Aut2;
        
        //Assign starts to start of choice a and b in the vector
        start2 = Automata().start.back(); Automata().start.pop_back();
        start1 = Automata().start.back(); Automata().start.pop_back();

        //Push back new start state
        Automata().start.push_back(Automata().count);

        //Point start of | state to starts of a and b
        ++Automata().count;
        Aut1.states.push_back(State('\0', start1, start2));
        ++Automata().count;
        Aut1.states.push_back(State());
    }

    return Aut1;
} // Expr

//<term>
Automata Term() {
    Automata Aut1; 
    Aut1 = Fact();
    
    if (isLpar(Automata().peek()) || isAlphaLower(Automata().peek())) {
        Automata Aut2 = Term();

        //Connect end of Aut1 to start of Aut2 written by Automata().start.push_back(Automata().count);
        //For example a1 -> b0. This is also why we only do push_back(State()) 
        //everywhere at the end because it gets overwritten by this code
        Aut1.states.back().to = Automata().start.back();
        Automata().start.pop_back();
        Aut1 = Aut1 + Aut2;

    } else if (!isValid(Automata().peek())) {
        std::cout << "String is not in language" << std::endl;
        std::cout << "Error located at character " << Automata().peek() << std::endl;
        exit(1);
    }
    return Aut1;
} // Term

//<fact>
Automata Fact() {
    Automata Aut1;
    int start1;

    //Handle (<expr>)
    if (isLpar(Automata().peek())) {
        Aut1.next();
        Aut1 = Expr();
                                                                                                                            
        if (isRpar(Automata().peek())) {
            Aut1.next();
        } else {
            std::cout << std::endl << "Missing right parantheses" << std::endl;
            exit(1);
        }

    //Handle <char>
    } else if (isAlphaLower(Automata().peek())) {
        //Push back new start state
        Automata().start.push_back(Automata().count);

        ++Automata().count;
        Aut1.states.push_back(State(Automata().peek(), Automata().count));
        ++Automata().count;
        Aut1.states.push_back(State());
        Automata().next();
    }

    //Handle *
    while (isAsterisk(Automata().peek())) {
        //Start1 = last known start state
        start1 = Automata().start.back(); Automata().start.pop_back();

        //Point last state to last known start state and to end of * state
        //For example a0 -> a1 will become a0 <-> a1 -> *0
        Aut1.states.back().to = start1;
        Aut1.states.back().to2 = Automata().count + 1;

        //Push back new start state
        Aut1.start.push_back(Automata().count);

        //Insert *0 and *1 states
        //*0 will point to *1 and last known start state
        //For example a0 <-> a1 -> *0 will become a0 <-> a1 -> *0 -> {*1, a0}  
        ++Automata().count;
        Aut1.states.push_back(State('\0', Automata().count, start1));
        ++Automata().count;
        Aut1.states.push_back(State());
        Automata().next();
    }
    return Aut1;
} // Fact