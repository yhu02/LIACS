/*
Eerste opdracht datastructuren: stapels.
Auteurs: Yvo hu s2962802 en Seyed Saqlain Zeidi s2982048
File: maxstapel.h
*/

#ifndef maxstapel_h
#define maxstapel_h

#include <iostream>
#include <string>
#include <vector>
#include "array.h"
#include "linkedlist.h"
#include "stlstack.h"
#include "stlvector.h"
#include "help.h"

template <class T>
class maxStapel {
    public:
    maxStapel(std::string input);
    void decode(std::string num);
    void read();
    void fill();

    private:
    T stack;
    T stack2;
    std::vector<std::string> stackString;
	std::string input;
};

template <class T>
maxStapel<T>::maxStapel(std::string input){
    this->input = input;
    this->read();
    this->fill();
}

//Turn string into a number and decide what to do
template <class T>
void maxStapel<T>::decode(std::string num){
    long long n = atoi<long long>(num);
    long long temp;
    long long temp2;

    switch(n){
    case(-2):
        if(this->stack2.top(temp)){
            std::cout << temp << " ";
        }else{
            std::cout << "_ ";
        }
        break;
    case(-1):
        this->stack.top(temp);
        this->stack2.top(temp2);
        if(temp == temp2){
            this->stack2.pop();
        }
        this->stack.pop();
        break;
    case(0):
        this->stack.clear();
        this->stack2.clear();
        break;
    default:
        this->stack.top(temp);
        if(this->stack2.size == 0 || n >= temp){
            this->stack2.push(n);
        }
        this->stack.push(n);
        break;
    }
}

//Read the input string, check syntax and split numbers
template <class T>
void maxStapel<T>::read(){
    for(size_t i = 0; i < this->input.size(); i++){
        std::string num = "";
        while((this->input[i] >= '0' && this->input[i] <= '9')
            || this->input[i] == '+' || this->input[i] == '-'){
            num += this->input[i];
            i++;
        }
        if(this->input[i] != ' ' && i != this->input.size()){
            error("Wrong syntax");
        }
        if(num != "")
            this->stackString.push_back(num);
    }
}

//Feed number string vector to decode
template <class T>
void maxStapel<T>::fill(){
    for(size_t i = 0; i < this->stackString.size(); i++){
        decode(this->stackString[i]);
    }
}

#endif