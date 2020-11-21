#ifndef _stack
#define _stack
#include <iostream>

//Templates moeten in header file

//Node in stack
template <class X>
class node{
	public:
		node();
		X* data;
		node<X>* next;
};

//Stack met singly linked list implementatie
template <class X>
class stack{
	public:
		stack();
		~stack();
		node<X>* top = nullptr;
		void push(X*);
		void pop();
		X* peek();
		int size;
};

//Constructor
template <class X>
node<X>::node(){
	next = nullptr;
	data = nullptr;
}

template <class X>
stack<X>::stack(){
	top = nullptr;
	size = 0;
}

template <class X>
stack<X>::~stack(){
	delete top;
}

//Push element naar stack
template <class X>
void stack<X>::push(X *gg){
	if (top == nullptr) {
		top = new node<X>;
		top->next = nullptr;
		top->data = gg;
	}
	else{
		node<X>* temp = new node<X>;
		//Stack overflow
		if(!temp){ 
        	exit(1); 
		}
		temp->data = gg;
		temp->next = top;
		top = temp;
	}
	size++;
}

//Pop element van stack
template <class X>
void stack<X>::pop(){
	if (top == nullptr) {
		std::cout << "Stack is leeg" << std::endl;
		return;
	}
	else{
		node<X>* temp = top;
		top = top->next;
		delete temp;
		size--;
	}
}

//Return element van stack
template <class X>
X* stack<X>::peek(){
	if (top)
		return top->data;
	else{
		X* temp = new X;
		return temp;
	}
}

#endif
