/*
Eerste opdracht datastructuren: stapels.
Auteurs: Yvo hu s2962802 en Seyed Saqlain Zeidi s2982048
File: stlstack.h
*/

#ifndef stlstack_h
#define stlstack_h

#include <iostream>
#include <string>
#include <stack>
#include "help.h"

template <class T>
class StlStack {
    public:
    StlStack();
    bool isEmpty();
    void clear();
    bool push(long long newItem);
    bool pop();
    bool top(long long &topItem);
    int size;

    private:
    std::stack<T> stack;
    template <typename U>
	friend class maxStapel;
};

//Constructor
template <class T>
StlStack<T>::StlStack(){
    this->size = 0;
}

//Check if stack is empty
template <class T>
bool StlStack<T>::isEmpty(){
    return this->stack.empty();
}

//Clear stack
template <class T>
void StlStack<T>::clear(){
    while(this->stack.size()){
        this->pop();
    }
}

//Push item to stack
template <class T>
bool StlStack<T>::push(long long newItem){
    this->stack.push((T) newItem);
    this->size = this->stack.size();
    return true;
}

//Pop item from stack
template <class T>
bool StlStack<T>::pop(){
    if(this->size == 0)
        return false;
    this->stack.pop();
    this->size = this->stack.size();
    return true;
}

//Put the top element into topItem
template <class T>
bool StlStack<T>::top(long long &topItem){
    if(this->size > 0){
        topItem = this->stack.top();
        return true;
    }else{
        return false;
    }
}
#endif

