/*
Eerste opdracht datastructuren: stapels.
Auteurs: Yvo hu s2962802 en Seyed Saqlain Zeidi s2982048
File: stlvector.h
*/

#ifndef stlvector_h
#define stlvector_h

#include <iostream>
#include <string>
#include "help.h"
#include <vector>

//stack met singly linked list
template <class T>
class StlVector{
	public:
	StlVector();
	~StlVector();
    bool isEmpty();
    void clear();
    bool push(long long newItem);
    bool pop();
    bool top(long long &topItem);
    int size;

	private:
	std::vector<T> stack;
    template <typename U>
	friend class maxStapel;
};

//Constructor
template <class T>
StlVector<T>::StlVector(){
    this->size = 0;
}

//Destructor
template <class T>
StlVector<T>::~StlVector(){
	this->clear();
}

//Check if stack is empty
template <class T>
bool StlVector<T>::isEmpty(){
	return this->stack.empty();
}
//Push element to StlVector
template <class T>
bool StlVector<T>::push(long long newItem){
    this->stack.push_back((T)newItem);
    this->size = this->stack.size();
	return true;
}

//Pop element from StlVector
template <class T>
bool StlVector<T>::pop(){
	if (this->stack.size() == 0) {
        //Pop failed
		return false;
	}
	else{
        this->stack.pop_back();
        this->size = this->stack.size();
        return true;
	}
}

//Put the top element into topItem
template <class T>
bool StlVector<T>::top(long long &topItem){
	if(this->stack.size() > 0){
		topItem = this->stack.back();
		return true;
	}
	return false;
}

//Clear stack
template <class T>
void StlVector<T>::clear(){
	while(this->size){
		this->pop();
	}
}
#endif
