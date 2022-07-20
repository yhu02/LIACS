//Yvo Hu			: 2962802
//Seyed Salqin Zeidi: 2982048

#include "../include/stack.h"

Stack *createStack(unsigned capacity)
{
	Stack *stack = (Stack *)malloc(sizeof(Stack));
	stack->capacity = capacity;
	stack->top = -1;
	stack->array = (int *)malloc(stack->capacity * sizeof(int));
	return stack;
}

// Stack is full when top is equal to the last index
int isFull(Stack *stack)
{
	return stack->top == (int)(stack->capacity - 1);
}

// Stack is empty when top is equal to -1
int isEmpty(Stack *stack)
{
	return stack->top == -1;
}

// Function to add an item to stack.  It increases top by 1
void push(Stack *stack, int item)
{
	if (isFull(stack))
		return;
	stack->array[++stack->top] = item;
}

// Function to remove an item from stack.  It decreases top by 1
int pop(Stack *stack)
{
	if (isEmpty(stack))
		return INT_MIN;
	return stack->array[stack->top--];
}

// Function to return the top from stack without removing it
int peek(Stack *stack)
{
	if (isEmpty(stack))
		return INT_MIN;
	return stack->array[stack->top];
}