%code requires{
#include "semantics.h"
}

%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "semantics.h"
#include "bytecode.h"
#include "instructions.h"
#include "map.h"
#include "error.h"

int  yylex(void);
void yyerror (char const *s)
{
	printf ("%s\n", s);
}

extern Map keywords;
extern Map operators;
extern Program program;

Map labelmap;
int labelcount=0;
Map variablemap;

int getLabelID(char *t){
	if(map_exists(&labelmap,t)>=0)return map_get(&labelmap,t);
	map_set(&labelmap,t,labelcount);
	return labelcount++;
}

int getVariableID(char *t){
	if(map_exists(&variablemap,t)>=0)return map_get(&variablemap,t);
	int new=memory_new(&program.memory,1);
	map_set(&variablemap,t,new);
	return new;
}

Parameter create_parameter_from_identifier(Identifier id){
	Parameter ret;
	switch(id.type){
		case SEM_V: ret.type=VAR_V; ret.value=getVariableID(id.text); break;
		case SEM_P: ret.type=VAR_P; ret.value=getVariableID(id.text); break;
		case SEM_C: ret.type=VAR_C; ret.value=id.num; break;
		default: exit(1);
	}
	return ret;
}

Parameter create_parameter_from_jump_identifier(Identifier id){
	Parameter ret;
	ret.type=VAR_C;
	ret.value=getLabelID(id.text);
	return ret;
}


void checkInstruction(Instruction I){
	switch(I.type){
		case IN_NEW:
		{
			if (I.param[0].type!=VAR_V && I.param[0].type!=VAR_P)exit(1);
		}
		break;
		case IN_DELETE:
		{
			if (I.param[0].type!=VAR_V && I.param[0].type!=VAR_P)exit(1);
		}
		break;
		case IN_TOP: {
			if (I.param[0].type!=VAR_V && I.param[0].type!=VAR_P)exit(1);
		}
		break;
		case IN_READINT:
		{
			if (I.param[0].type!=VAR_V && I.param[0].type!=VAR_P)exit(1);
		}
		break;
		case IN_READCHAR:
		{
			if (I.param[0].type!=VAR_V && I.param[0].type!=VAR_P)exit(1);
		}
		break;
		case IN_OPERATOR:
		{
			if (I.param[1].type!=VAR_V && I.param[1].type!=VAR_P)exit(1);
		}

	}
}

%}

%union {
	Identifier identifier;
	char chr;
	Programpiece programpiece;
	Instruction instruction;
}

%token <identifier> 	FUNCTION
%token <identifier>	UNARYFUNCTION
%token <identifier>	BINARYFUNCTION
%token <identifier>	VARIABLE
%token <identifier>	CONSTANT
%token <identifier>	POINTER
%token <chr> 		ASSIGN
%token <identifier> 	OPERATOR
%token <chr> 		NEWLINE
%token <identifier> 	LABEL
%type  <programpiece> 	program
%type  <instruction>	line
%type  <identifier>     memorycell
%type  <identifier>	parameter
%type  <identifier>	label
%%
init:  program  {
			program.instructions=$1.instructions;
			program.len=$1.len;
			int *Labels=(int*)malloc(sizeof(int)*labelcount);
			for(int i=0;i<labelcount;i++)Labels[i]=-1;
			for(int i=0;i<program.len;i++){
				if(program.instructions[i].label==NULL)continue;
				if(map_exists(&labelmap,program.instructions[i].label)<=0){
					free(program.instructions[i].label);
					continue;
				}
				int poi=map_get(&labelmap,program.instructions[i].label);
				if(Labels[poi]!=-1){
					exit(1);//taleb uz existuje
				}
				Labels[poi]=i;
				free(program.instructions[i].label);
			}
			for(int i=0;i<program.len;i++){
				if(program.instructions[i].type==IN_JUMP||program.instructions[i].type==IN_CALL){
					if(Labels[program.instructions[i].param[0].value]==-1)exit(1);//neexistuje taky label
					program.instructions[i].param[0].value = Labels[program.instructions[i].param[0].value];
				}else if(program.instructions[i].type==IN_IF){
					if(Labels[program.instructions[i].param[1].value]==-1)exit(1);//neexistuje taky label
					program.instructions[i].param[1].value = Labels[program.instructions[i].param[1].value];
				}
			}
		}

program: 	  /* empty */     { 
					$$.len=$$.capacity=0;
					$$.instructions=NULL;
				  }
		| program line    {
					if($2.type>=0){
						int newcapacity=$1.capacity;
						int newlen = $1.len+1;
						if (newcapacity<=0) newcapacity=10;
						while(newlen>=newcapacity)newcapacity*=2;
						if(newcapacity!=$1.capacity)
							$1.instructions=(Instruction*)realloc($1.instructions,sizeof(Instruction)*newcapacity);
						$1.instructions[$1.len++]=$2;
						$1.capacity=newcapacity;
					}
					$$=$1;
				  }
;

line: 		  NEWLINE { $$.type=-1; $$.param=NULL; $$.param_len=0;} 
		| label FUNCTION NEWLINE {
			        $$.label=$1.text;	
				$$.type=map_get(&keywords,$2.text);
				$$.param_len=0;
				$$.param=NULL;
				free($2.text); 
				checkInstruction($$);
			}
		| label UNARYFUNCTION parameter NEWLINE {
				$$.label=$1.text;
				$$.type=map_get(&keywords,$2.text);
				$$.param_len=1;
				$$.param=malloc(sizeof(Parameter));
				if(!strcmp($3.text,"jump"))
					$$.param[0]=create_parameter_from_jump_identifier($3);
				else
					$$.param[0]=create_parameter_from_identifier($3);
				free($2.text);
				free($3.text);
				checkInstruction($$);

		}
		| label BINARYFUNCTION parameter parameter NEWLINE {
				$$.label=$1.text;
				$$.type=map_get(&keywords,$2.text);
				$$.param_len=2;
				$$.param=malloc(sizeof(Parameter)*$$.param_len);
				$$.param[0]=create_parameter_from_identifier($3);
				if(!strcmp($4.text,"if"))
					$$.param[1]=create_parameter_from_jump_identifier($4);
				else
					$$.param[1]=create_parameter_from_identifier($4);
				free($2.text);
				free($3.text);
				free($4.text);
				checkInstruction($$);
		}
		| label memorycell ASSIGN parameter OPERATOR parameter NEWLINE {
				$$.label=$1.text;
				$$.type=IN_OPERATOR;
				$$.param_len=4;
				$$.param=malloc(sizeof(Parameter)*$$.param_len);
				$$.param[0].value=map_get(&operators,$5.text);
				$$.param[1]=create_parameter_from_identifier($2);
				$$.param[2]=create_parameter_from_identifier($4);
				$$.param[3]=create_parameter_from_identifier($6);
				free($2.text);
				free($4.text);
				free($6.text);
				checkInstruction($$);
		}
;

memorycell: 	  VARIABLE
		| POINTER
;

label: 		  /* empty */ {$$.text=NULL;}
		| LABEL
;

parameter:	  memorycell
		| CONSTANT
;

%%
