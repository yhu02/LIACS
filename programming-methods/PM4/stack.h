#ifndef _stack
#define _stack
#include <iostream>
//Templates moeten in header file

//Node in stack
template <class Y>
class node{
	public:
		node();
		Y* data;
		node<Y>* next;
};

//Constructor
template <class Y>
node<Y>::node(){
	next = nullptr;
	data = nullptr;
}

//Stack met singly linked list
template <class X>
class stack{
	public:
		stack();
		~stack();
	private:
		node<X>* top;
		void push(X*);
		void pop();
		X* peek();
		int size;
		friend class grootGetal;
};

template <class X>
stack<X>::stack(){
	top = nullptr;
	size = 0;
}

//Destructor
template <class X>
stack<X>::~stack(){
	while(size){
		delete top;
		this->pop();
	}
}

//Push element naar stack
template <class X>
void stack<X>::push(X *gg){
		node<X>* newcijfervak = new node<X>;
		//Stack overflow
		if(!newcijfervak){ 
        	exit(1); 
		}
		newcijfervak->data = gg;
		newcijfervak->next = top;
		top = newcijfervak;
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
	return top->data;
}
#endif
