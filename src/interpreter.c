#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "parser.h"
#include "bytecode.h"
#include "memory.h"
#include "map.h"
#include "stack.h"
#include "instructions.h"
#include "error.h"
#include "semantics.h"

Memory memory;
Program program;

int yyparse();

extern FILE* yyin;

Map operators;
Map keywords;
int FatalError = 0;

void createProgram(FILE *input){
	map_init(&operators);
	map_init(&keywords);
	map_set(&keywords,"new",IN_NEW);
	map_set(&keywords,"delete",IN_DELETE);
	map_set(&keywords,"pop",IN_POP);
	map_set(&keywords,"return",IN_RETURN);
	map_set(&keywords,"jump",IN_JUMP);
	map_set(&keywords,"readint",IN_READINT);
	map_set(&keywords,"writeint",IN_WRITEINT);
	map_set(&keywords,"readchar",IN_READCHAR);
	map_set(&keywords,"writechar",IN_WRITECHAR);
	map_set(&keywords,"top",IN_TOP);
	map_set(&keywords,"push",IN_PUSH);
	map_set(&keywords,"call",IN_CALL);
	map_set(&keywords,"if",IN_IFG);
	map_set(&keywords,"ifg",IN_IFG);
	map_set(&keywords,"ifgeq",IN_IFGEQ);
	map_set(&keywords,"ifneq",IN_IFNEQ);
	map_set(&keywords,"ifeq",IN_IFEQ);
	map_set(&keywords,"ifleq",IN_IFLEQ);
	map_set(&keywords,"ifl",IN_IFL);
	map_set(&operators,"+",OP_PLUS);
	map_set(&operators,"-",OP_MINUS);
	map_set(&operators,"*",OP_MULTIPLY);
	map_set(&operators,"/",OP_DIVIDE);
	map_set(&operators,"%",OP_MODULO);
	map_set(&operators,"&",OP_AND);
	map_set(&operators,"|",OP_OR);
	map_set(&operators,"^",OP_XOR);
	map_set(&operators,"<<",OP_SHL);
	map_set(&operators,">>",OP_SHR);
	memory_init(&memory);
	memory_init(&program.memory);

	//Nastavime Flexu aby cital vstup zo suboru input
	yyin = input;
	// Zavolame parser 
	yyparse();
	if(FatalError) ERROR(ERROR_USER,"Program has too many syntax errors.\n");

}

int get_value_from_memory (Memory *memory, Program *program, Stack *stack, Parameter *parameter, int canconst) {
	if (parameter->type == VAR_V) {
		return memory_get (&program->memory, parameter->value);
	} else if (parameter->type == VAR_P) {
		return memory_get (memory, memory_get (&program->memory, parameter->value) );
	} else if (parameter->type == VAR_S) {
		return stack_get(stack,parameter->value);		
	} else if (parameter->type == VAR_SP) {
		return memory_get (memory, stack_get(stack,parameter->value));
	} else if (parameter->type == VAR_C && canconst) {
		return parameter->value;
	}
	ERROR(ERROR_INTERNAL,"Unsupported parameter type. Type is: %d\n",parameter->type);
}

int set_value_to_memory (Memory *memory,Program *program,Stack *stack, Parameter *parameter, int value) {
	if (parameter->type == VAR_V) {
		memory_set (&program->memory, parameter->value, value);
	} else if (parameter->type == VAR_P) {
		memory_set (memory, memory_get (&program->memory, parameter->value), value);
	} else if (parameter->type == VAR_S) {
		stack_set(stack,parameter->value,value);	
	} else if (parameter->type == VAR_SP) {
		memory_set (memory, stack_get(stack,parameter->value),value);
	} else {
		ERROR(ERROR_INTERNAL,"Unsupported parameter type. Type is: %d\n",parameter->type);
	}
}


extern Map labelmap;
extern Map variablemap; 

void debugMemory(Instruction *in,int incount, Stack *stack, Stack *instructionsStack){
	fprintf(stderr,"=================DEBUG===================\nInstruction count: %d\n",incount);
	//print registers
	fprintf(stderr,"Registers:\n");
	for(int i=0;i< program.memory.max_index;i++){
		fprintf(stderr,"%s: %d\n",map_reverse_get(&variablemap,i),program.memory.memory[i]);
	}
	//print memory
	fprintf(stderr,"Memory:\n");
	for(int i=0;i<memory.max_index;i++){
		if(memory.goodbits[i]>=0){
			fprintf(stderr," %d:%d",i,memory.memory[i]);
		}else if (memory.goodbits[i]==-1){
			fprintf(stderr," %d:A",i);
		}else if (memory.goodbits[i]==-2){
			fprintf(stderr," %d:D",i);
		}else {
			fprintf(stderr," %d:?",i);
		}

	}fprintf(stderr,"\n");
	//print stack
	fprintf(stderr,"Stack (from bottom to top):\n");
	for(int i=0;i<stack->top;i++){
		fprintf(stderr," %d",stack->stack[i]);
	}fprintf(stderr,"\n");
	//print instruction stack
	fprintf(stderr,"Internal stack (from bottom to top):\n");
	for(int i=0;i<instructionsStack->top;i++){
		fprintf(stderr," %d",program.instructions[instructionsStack->stack[i]].line);
	}fprintf(stderr,"\n");
	fprintf(stderr,"Current instruction line: %d\n",in->line);
}

int main (int argc, char **argv) {
	reuseMemory = 0;
	randomMemory = 0;
	int debug  = 0;
	char *file=NULL;
	for(int i=1;i<argc;i++){
		if(!strcmp(argv[i],"--random-memory")){
			randomMemory=1;
		}else if (!strcmp(argv[i],"--reuse-memory")){
			reuseMemory=1;
		}else if (!strcmp(argv[i],"--debug")){
			debug = 1;
		} else {
			file=argv[i];
		}
	}
	if (file==NULL) {
		printf ("USAGE: ./interpreter [parameters] <source file>\nPossible parameters:\n    --random-memory | initialize memory to random integers\n    --reuse-memory  | reuse unallocated memory\n");
		return 1;
	}
	FILE *input=fopen(file,"r");

	createProgram(input);

	program.ip = 0;


	Stack stack;
	stack_init (&stack);
	Stack instructionsStack;
	stack_init (&instructionsStack);

	long long incount = 0;
	
/*	for(int i=0;i<program.len;i++){
		printf("%d %d\n",program.instructions[i].type,program.instructions[i].param_len);
	}*/

	int run=1;
	while (run) {
//		printf("Instruction: %d\n",program.ip);
		int increment_instruction = 1;
		if(program.ip<0||program.ip>=program.len)ERROR(ERROR_INTERNAL,"Instruction pointer is outside program\n");
		Instruction in = program.instructions[program.ip];
		if (debug) {
			debugMemory(&in,incount,&stack,&instructionsStack);
		//	fgetc(stdin);
			//TODO: READKEY
		}
		switch (in.type) {
		case IN_NEW:
		{
			if (in.param_len != 2) ERROR(ERROR_INTERNAL,"new -- wrong number of parameters: %d\n",in.param_len);
			int len = get_value_from_memory(&memory,&program,&stack,&in.param[1],1);
			int pointer = memory_new (&memory, len);
			set_value_to_memory(&memory,&program,&stack,&in.param[0],pointer);
		}
		break;
		case IN_DELETE:
		{
			if (in.param_len != 1) ERROR(ERROR_INTERNAL,"delete -- wrong number of parameters: %d\n",in.param_len);
			int pointer = get_value_from_memory(&memory,&program,&stack,&in.param[0],0);
			memory_delete (&memory, pointer);
		}
		break;
		case IN_POP: {
			stack_pop (&stack);
		}
		break;
		case IN_TOP: {
			if (in.param_len != 1) ERROR(ERROR_INTERNAL,"top -- wrong number of parameters: %d\n",in.param_len);
			set_value_to_memory(&memory,&program,&stack,&in.param[0],stack_top(&stack));
		}
		break;
		case IN_PUSH:
		{
			if (in.param_len != 1)ERROR(ERROR_INTERNAL,"push -- wrong number of parameters: %d\n",in.param_len);
			int value = get_value_from_memory(&memory,&program,&stack,&in.param[0],1);
			stack_push (&stack, value);
		}
		break;
		case IN_RETURN:
		{
			if (in.param_len != 0) ERROR(ERROR_INTERNAL,"return -- wrong number of parameters: %d\n",in.param_len);
			increment_instruction = 1;
			if (stack_empty (&instructionsStack) ) run=0;
			program.ip = stack_top (&instructionsStack);
			stack_pop (&instructionsStack);
		}
		break;
		case IN_JUMP:
		{
			if (in.param_len != 1) ERROR(ERROR_INTERNAL,"jump -- wrong number of parameters: %d\n",in.param_len);
			increment_instruction = 0;
			program.ip = in.param[0].value;
		}
		break;
		case IN_READINT:
		{
			if (in.param_len != 1) ERROR(ERROR_INTERNAL,"readint -- wrong number of parameters: %d\n",in.param_len);
			int r;
			scanf (" %d", &r);
			set_value_to_memory(&memory,&program,&stack,&in.param[0],r);
		}
		break;
		case IN_WRITEINT:
		{
			if (in.param_len != 1)  ERROR(ERROR_INTERNAL,"writeint -- wrong number of parameters: %d\n",in.param_len);
			int r = get_value_from_memory(&memory,&program,&stack,&in.param[0],1);
			printf ("%d", r);
		}
		break;
		case IN_READCHAR:
		{
			if (in.param_len != 1) ERROR(ERROR_INTERNAL,"readchar -- wrong number of parameters: %d\n",in.param_len);
			char r;
			scanf ("%c", &r);
			set_value_to_memory(&memory,&program,&stack,&in.param[0],r);
		}
		break;
		case IN_WRITECHAR:
		{
			if (in.param_len != 1)ERROR(ERROR_INTERNAL,"writechar -- wrong number of parameters: %d\n",in.param_len);
			int r = get_value_from_memory(&memory,&program,&stack,&in.param[0],1);
			printf ("%c", r);
		}
		break;
		case IN_CALL:
		{
			if (in.param_len != 1)ERROR(ERROR_INTERNAL,"call -- wrong number of parameters: %d\n",in.param_len);
			increment_instruction = 0;
			stack_push (&instructionsStack, program.ip);
			program.ip = in.param[0].value;
		}
		break;
		case IN_IFG:
		{
			if (in.param_len != 2)ERROR(ERROR_INTERNAL,"if -- wrong number of parameters: %d\n",in.param_len);
			int val=get_value_from_memory(&memory,&program,&stack,&in.param[0],1);
			//printf("ifg-wtf: %d (%d)\n",val,incount);
			if (val > 0) {
				increment_instruction = 0;
				program.ip = in.param[1].value;
			}
		}
		break;
		case IN_IFGEQ:
		{
			if (in.param_len != 2)ERROR(ERROR_INTERNAL,"if -- wrong number of parameters: %d\n",in.param_len);
			int val=get_value_from_memory(&memory,&program,&stack,&in.param[0],1);
			//printf("ifgeq-wtf: %d (%d)\n",val,incount);
			if (val >= 0) {
				increment_instruction = 0;
				program.ip = in.param[1].value;
			}
		}
		break;
		case IN_IFEQ:
		{
			if (in.param_len != 2)ERROR(ERROR_INTERNAL,"if -- wrong number of parameters: %d\n",in.param_len);
			int val=get_value_from_memory(&memory,&program,&stack,&in.param[0],1);
			//printf("ifeq-wtf: %d (%d)\n",val,incount);
			if (val == 0) {
				increment_instruction = 0;
				program.ip = in.param[1].value;
			}
			//printf("ifeq-wtf: %d (%d)\n",val,incount);
		}
		break;
		case IN_IFNEQ:
		{
			if (in.param_len != 2)ERROR(ERROR_INTERNAL,"if -- wrong number of parameters: %d\n",in.param_len);
			int val=get_value_from_memory(&memory,&program,&stack,&in.param[0],1);
			//printf("ifneq-wtf: %d (%d)\n",val,incount);
			if (val != 0) {
				increment_instruction = 0;
				program.ip = in.param[1].value;
			}
		}
		break;
		case IN_IFL:
		{
			if (in.param_len != 2)ERROR(ERROR_INTERNAL,"if -- wrong number of parameters: %d\n",in.param_len);
			int val=get_value_from_memory(&memory,&program,&stack,&in.param[0],1);
			//printf("ifl-wtf: %d (%d)\n",val,incount);
			if (val < 0) {
				increment_instruction = 0;
				program.ip = in.param[1].value;
			}
		}
		break;
		case IN_IFLEQ:
		{
			if (in.param_len != 2)ERROR(ERROR_INTERNAL,"if -- wrong number of parameters: %d\n",in.param_len);
			int val=get_value_from_memory(&memory,&program,&stack,&in.param[0],1);
			//printf("ifleq-wtf: %d (%d)\n",val,incount);
			if (val <= 0) {
				increment_instruction = 0;
				program.ip = in.param[1].value;
			}
		}
		break;
		case IN_OPERATOR:
		{
			//4 parametre -- co, kam, odkial 1, odkial 2
			if (in.param_len != 4) ERROR(ERROR_INTERNAL,"operator -- wrong number of parameters: %d\n",in.param_len);
			int result = get_value_from_memory(&memory,&program,&stack,&in.param[2],1);
			int val = get_value_from_memory(&memory,&program,&stack,&in.param[3],1);
			switch (in.param[0].value) {
			case OP_PLUS:
				result += val;
				break;
			case OP_MINUS:
				result -= val;
				break;
			case OP_MULTIPLY:
				result *= val;
				break;
			case OP_DIVIDE: {

				if (val == 0) ERROR(ERROR_USER,"Division by zero\n");
				result /= val;
			}
			break;
			case OP_MODULO: {
				if (val == 0) ERROR(ERROR_USER,"Division by zero\n");
				result %= val;
			}
			break;
			case OP_AND:
				result &= val;
				break;
			case OP_OR:
				result |= val;
				break;
			case OP_XOR:
				result ^= val;
				break;
			case OP_SHL:
				result = result << val;
				break;
			case OP_SHR:
				result = result >> val;
				break;
			default:
				ERROR(ERROR_INTERNAL,"Unknown operator: %d",in.param[0].value);
				break;

			}
			set_value_to_memory(&memory,&program,&stack,&in.param[1],result);
		}
		break;
		default:
			ERROR(ERROR_INTERNAL,"Unknown instruction with ID: %d\n", in.type);
			break;
		}
		if (increment_instruction) {
			program.ip++;
			if(program.ip==program.len)run=0;
		}
		incount++;
	}
	fprintf (stderr, "Program successfully ended after executing %lld instructions.\n", incount);
	return 0;
}


