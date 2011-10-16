#ifndef STACK_H
#define STACK_H

#define STACK_INIT_SIZE 10

struct _Stack{
	int *stack;
	int size,top;
};

typedef struct _Stack Stack;

void stack_init(Stack *stack);
void stack_destroy(Stack*);
void stack_push(Stack*,int);
void stack_pop(Stack*);
int  stack_top(Stack*);
int  stack_empty(Stack*);

void stack_set(Stack*,int,int);
int stack_get(Stack*,int);

#endif
