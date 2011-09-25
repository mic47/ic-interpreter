#include <stdio.h>
#include <stdlib.h>

#include "stack.h"
#include "error.h"

void stack_init (Stack* stack) {
	stack->stack = (int*) malloc (sizeof (int) * STACK_INIT_SIZE);
	stack->size = STACK_INIT_SIZE;
	stack->top = 0;
}

void stack_destroy (Stack *stack) {
	free (stack->stack);
	stack->stack = NULL;
	stack->size = stack->top = 0;
}

void stack_push (Stack *stack, int value) {
	if(stack->size==0) ERROR(ERROR_INTERNAL,"Stack has zero capacity!\n");
	int newsize = stack->size;
	while (stack->top >= newsize) newsize *= 2;
	if (stack->size != newsize) {
		stack->stack = realloc (stack->stack, newsize);
		stack->size = newsize;
	}
	stack->stack[stack->top++] = value;
}

void stack_pop (Stack *stack) {
	if (stack->top > 0) stack->top--;
	int newsize = stack->size;
	if (stack->top * 4 < stack->size ) {
		newsize /= 2;
	}
	if (newsize < STACK_INIT_SIZE) newsize = STACK_INIT_SIZE;
	if (newsize != stack->size) {
		stack->stack = (int*) realloc (stack->stack, newsize);
		stack->size = newsize;
	}
}

int  stack_top (Stack *stack) {
	if (stack->top<0){
		ERROR(ERROR_INTERNAL,"Stack has weird size: %d\n",stack->top);
	}
	if(stack->top==0){
		ERROR(ERROR_USER,"Cannot look on the top of the empty stack!");
	}
	return stack->stack[stack->top-1];
}

int  stack_empty (Stack* stack) {
	if (stack->top == 0) return 1;
	return 0;
}
