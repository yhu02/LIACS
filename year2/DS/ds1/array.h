/*
Eerste opdracht datastructuren: stapels.
Auteurs: Yvo hu s2962802 en Seyed Saqlain Zeidi s2982048
File: array.h
*/

#include <iostream>
#include <string>
#include "help.h"

template <class T>
class Array {
    public:
    Array(); //constructor
    ~Array(); //destructor
  
    void create();
    bool isEmpty();
    void clear();
    bool push(long long newItem);
    bool pop();
    bool top(long long &topItem);
    void expand();
        int size;

    private:
    int bufferSize = 1;
    T *stack; 
    template <typename U>
    friend class maxStapel;
};

//Constructor
template<typename T>
Array<T>::Array(){
    size = 0;
    stack = new T [bufferSize];
}

//Destructor
template<typename T>
Array<T>::~Array(){
    delete[] this->stack;
}

//Check if stack is empty
template<typename T>
bool Array<T>::isEmpty(){
    if(size == 0) return true;
    else return false;
}

//Clear stack
template<typename T>
void Array<T>::clear(){
    delete[] this->stack;
    bufferSize = 1;
    this->stack = new T[bufferSize];
    size = 0;
}

//Push item to stack
template<typename T>
bool Array<T>::push(long long newItem){
    //Increase stack size if capacity has been reached
    if(size == bufferSize){
        this->expand();
    }
    stack[size] = (T)newItem;
    size++;
    return true;
}

//Pop item from stack
template<typename T>
bool Array<T>::pop(){
    if(size == 0){
        return false;
    }
    size--;
    return true;
}

//Put the top element into topItem
template<typename T>
bool Array<T>::top(long long &topItem){
    if(size > 0){
        topItem = stack[size - 1];
        return true;
    }else{
        return false;
    }
}

//Increase stack size
template<typename T>
void Array<T>::expand(){
    T* newStack;
    bufferSize = bufferSize*2;
    newStack = new T[bufferSize];
    for(int i = 0; i < bufferSize; i++){
        newStack[i] = this->stack[i];
    }
    delete[] this->stack;
    this->stack = newStack;
}
