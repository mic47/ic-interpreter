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

void createProgram(Program *program,FILE *input){

}

int get_value_from_memory (Memory *memory, Program *program, Parameter *parameter, int canconst) {
	if (parameter->type == VAR_V) {
		return memory_get (&program->memory, parameter->value);
	} else if (parameter->type == VAR_P) {
		return memory_get (memory, memory_get (&program->memory, parameter->value) );
	} else if (parameter->type == VAR_C && canconst) {
		return parameter->value;
	}
	ERROR(ERROR_INTERNAL,"Unsupported parameter type. Type is: %d\n",parameter->type);
}

int set_value_to_memory (Memory *memory,Program *program, Parameter *parameter, int value) {
	if (parameter->type == VAR_V) {
		memory_set (&program->memory, parameter->value, value);
	} else if (parameter->type == VAR_P) {
		memory_set (memory, memory_get (&program->memory, parameter->value), value);
	} else {
		ERROR(ERROR_INTERNAL,"Unsupported parameter type. Type is: %d\n",parameter->type);
	}
}


int main (int argc, char **argv) {
	if (argc == 1) {
		printf ("USAGE: ./interpreter <source file>\n");
		return 1;
	}
	FILE *input = fopen (argv[1], "r");

	Program program;
	
	createProgram(&program,input);

	program.ip = 0;

	Memory memory;

	Stack stack;
	stack_init (&stack);

	long long incount = 0;

	while (1) {
		int increment_instruction = 1;
		//TODO: skontroluj ci instrukcia je OK
		Instruction in = program.instructions[program.ip];
		switch (in.type) {
		case IN_NEW:
		{
			if (in.param_len != 2) ERROR(ERROR_INTERNAL,"new -- wrong number of parameters: %d\n",in.param_len);
			int len = get_value_from_memory(&memory,&program,&in.param[1],1);
			int pointer = memory_new (&memory, len);
			set_value_to_memory(&memory,&program,&in.param[0],pointer);
		}
		break;
		case IN_DELETE:
		{
			if (in.param_len != 1) ERROR(ERROR_INTERNAL,"delete -- wrong number of parameters: %d\n",in.param_len);
			int pointer = get_value_from_memory(&memory,&program,&in.param[0],0);
			memory_delete (&memory, pointer);
		}
		break;
		case IN_POP: {
			stack_pop (&stack);
		}
		break;
		case IN_TOP: {
			if (in.param_len != 1) ERROR(ERROR_INTERNAL,"top -- wrong number of parameters: %d\n",in.param_len);
			set_value_to_memory(&memory,&program,&in.param[0],stack_top(&stack));
		}
		break;
		case IN_PUSH:
		{
			if (in.param_len != 1)ERROR(ERROR_INTERNAL,"push -- wrong number of parameters: %d\n",in.param_len);
			int value = get_value_from_memory(&memory,&program,&in.param[0],1);
			stack_push (&stack, value);
		}
		break;
		case IN_RETURN:
		{
			if (in.param_len != 0) ERROR(ERROR_INTERNAL,"return -- wrong number of parameters: %d\n",in.param_len);
			increment_instruction = 1;
			if (stack_empty (&stack) ) exit (0);
			program.ip = stack_top (&stack);
			stack_pop (&stack);
		}
		break;
		case IN_JUMP:
		{
			if (in.param_len != 1) ERROR(ERROR_INTERNAL,"jump -- wrong number of parameters: %d\n",in.param_len);
			if (in.param[0].type != VAR_C) {
				exit (1);
			}
			increment_instruction = 0;
			program.ip = in.param[0].value;
		}
		break;
		case IN_READINT:
		{
			if (in.param_len != 1) ERROR(ERROR_INTERNAL,"readint -- wrong number of parameters: %d\n",in.param_len);
			int r;
			scanf (" %d", &r);
			set_value_to_memory(&memory,&program,&in.param[0],r);
		}
		break;
		case IN_WRITEINT:
		{
			if (in.param_len != 1)  ERROR(ERROR_INTERNAL,"writeint -- wrong number of parameters: %d\n",in.param_len);
			int r = get_value_from_memory(&memory,&program,&in.param[0],1);
			printf ("%d", r);
		}
		break;
		case IN_READCHAR:
		{
			if (in.param_len != 1) ERROR(ERROR_INTERNAL,"readchar -- wrong number of parameters: %d\n",in.param_len);
			char r;
			scanf ("%c", &r);
			set_value_to_memory(&memory,&program,&in.param[0],r);
		}
		break;
		case IN_WRITECHAR:
		{
			if (in.param_len != 1)ERROR(ERROR_INTERNAL,"writechar -- wrong number of parameters: %d\n",in.param_len);
			int r = get_value_from_memory(&memory,&program,&in.param[0],1);
			printf ("%c", r);
		}
		break;
		case IN_CALL:
		{
			if (in.param_len != 1)ERROR(ERROR_INTERNAL,"call -- wrong number of parameters: %d\n",in.param_len);
			increment_instruction = 0;
			stack_push (&stack, program.ip);
			program.ip = in.param[0].value;
		}
		break;
		case IN_IF:
		{
			if (in.param_len != 2)ERROR(ERROR_INTERNAL,"if -- wrong number of parameters: %d\n",in.param_len);
			int val=get_value_from_memory(&memory,&program,&in.param[0],1);
			if (val > 0) {
				increment_instruction = 0;
				program.ip = in.param[1].value;
			}
		}
		break;
		case IN_OPERATOR:
		{
			//4 parametre -- co, kam, odkial 1, odkial 2
			if (in.param_len != 4) ERROR(ERROR_INTERNAL,"operator -- wrong number of parameters: %d\n",in.param_len);
			int result = get_value_from_memory(&memory,&program,&in.param[2],1);
			int val = get_value_from_memory(&memory,&program,&in.param[3],1);
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
			set_value_to_memory(&memory,&program,&in.param[1],result);
		}
		break;
		default:
			ERROR(ERROR_INTERNAL,"Unknown instruction with ID: %d\n", in.type);
			exit (1);
			break;
		}
		if (increment_instruction) program.ip++;
		incount++;
	}
	fprintf (stderr, "Program successfully ended after executing %lld instructions.\n", incount);
	return 0;
}


