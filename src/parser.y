%{
#include <stdio.h>
#include <stdlib.h>
	int  yylex(void);
void
     yyerror (char const *s)
     {
       printf ("%s\n", s);
     }
%}

%union {
	char *str;
	char chr;
	int  num;
}

%token <str> FUNCTION
%token <str> UNARYFUNCTION
%token <str> BINARYFUNCTION
%token <str> VARIABLE
%token <num> CONSTANT
%token <str> POINTER
%token <chr> ASSIGN
%token <chr> OPERATOR
%token <str> NEWLINE
%token <str> LABEL
%%
program: 	  /* empty */
		| program line
;

line: 		  NEWLINE
		| label FUNCTION NEWLINE
		| label UNARYFUNCTION parameter NEWLINE
		| label BINARYFUNCTION parameter parameter NEWLINE
		| label memorycell ASSIGN parameter OPERATOR parameter NEWLINE
;

memorycell: 	  VARIABLE
		| POINTER
;

label: 		  /* empty */
		| LABEL
;

parameter:	  memorycell
		| CONSTANT
;

%%

int main(int argc,char **argv){

}
