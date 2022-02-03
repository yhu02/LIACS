/*
Eerste opdracht datastructuren: stapels.
Auteurs: Yvo hu s2962802 en Seyed Saqlain Zeidi s2982048
File: linkedlist.h
*/

#ifndef linkedList_h
#define linkedList_h

#include <iostream>
#include <string>
#include "help.h"

//Node in LinkedList
template <class N>
class Node{
	public:
		Node();
		N data;
		Node<N>* next;
};

//Constructor
template <class N>
Node<N>::Node(){
	next = nullptr;
	data = 0;
}

//Stack as singly linked list
template <class T>
class LinkedList{
	public:
	LinkedList();
	~LinkedList();
    bool isEmpty();
    void clear();
    bool push(long long newItem);
    bool pop();
    bool top(long long &topItem);
	T type;
    int size;

	private:
	Node<T>* topNode;
	template <typename U>
	friend class maxStapel;
};

//Constructor
template <class T>
LinkedList<T>::LinkedList(){
	topNode = nullptr;
	this->size = 0;
}

//Destructor
template <class T>
LinkedList<T>::~LinkedList(){
	this->clear();
}

//Check if stack is empty
template <class T>
bool LinkedList<T>::isEmpty(){
	if(this->size)
		return false;
	return true;
}
//Push Node to LinkedList
template <class T>
bool LinkedList<T>::push(long long newItem){
	
	Node<T>* newNode = new Node<T>;
	//LinkedList overflow
	if(!newNode){ 
		return false;
	}
	newNode->data = (T)newItem;
	newNode->next = topNode;
	topNode = newNode;
	this->size++;
	return true;
}

//Pop Node from LinkedList
template <class T>
bool LinkedList<T>::pop(){
	if (topNode == nullptr) {
        //Pop failed
		return false;
	}
	else{
		Node<T>* temp = topNode;
		topNode = topNode->next;
		delete temp;
		this->size--;
        return true;
	}
}

//Put the top element into topItem
template <class T>
bool LinkedList<T>::top(long long &topItem){
	if(this->size > 0){
		topItem = this->topNode->data;
		return true;
	}else{
		return false;
	}
}

//Clear stack
template <class T>
void LinkedList<T>::clear(){
	while(size){
		this->pop();
	}
}
#endif
