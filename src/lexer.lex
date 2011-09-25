%{
#include <string.h>
#include <ctype.h>
#include "semantics.h"
#include "parser.h"
#include "error.h"

void copy_string(char **dest,char *src,int del_begin,int del_end){
	int len = strlen(src);
	if(len<del_begin+del_end)ERROR(ERROR_INTERNAL,"I want to delete too much from token\n");
	*dest=(char*)malloc(sizeof(char)*len+1-del_begin-del_end);
	strncpy(*dest,src+del_begin,len-del_begin-del_end);
	(*dest)[len-del_begin-del_end]=0;
}


%}

LABEL 			(?i:[a-z][a-zA-Z0-9]*)+":"
FUNCTION   		(?-i:pop|return)
UNARYFUNCTION 		(?-i:delete|jump|readint|writeint|readchar|writechar|top|push|call)
BINARYFUNCTION 		(?-i:if|new)
VARIABLE   		[a-zA-Z][a-zA-Z0-9]*
CONSTANT   		[0-9]*
POINTER    		@{VARIABLE}
ASSIGN     		=
OPERATOR   		"+"|"-"|"*"|"/"|"%"|"&"|"|"|"^"|"<<"|">>"
WHITESPACE 		[ \t]*
NEWLINE 		"\n"

%%
{LABEL}			{
				copy_string(&yylval.identifier.text,yytext,0,1);
				yylval.identifier.type=SEM_L;
				return LABEL;
			}
{FUNCTION}		{
				copy_string(&yylval.identifier.text,yytext,0,0);
				yylval.identifier.type=SEM_F;
				return FUNCTION;
			}
{UNARYFUNCTION}		{
				copy_string(&yylval.identifier.text,yytext,0,0);
				yylval.identifier.type=SEM_F;
				return UNARYFUNCTION;
			}
{BINARYFUNCTION}	{
				copy_string(&yylval.identifier.text,yytext,0,0);
				yylval.identifier.type=SEM_F;
				return BINARYFUNCTION;
			}
{VARIABLE}		{
				copy_string(&yylval.identifier.text,yytext,0,0);
				yylval.identifier.type=SEM_V;
				return VARIABLE;
			}
{CONSTANT}		{
				copy_string(&yylval.identifier.text,yytext,0,0);
				yylval.identifier.type=SEM_C;
				yylval.identifier.num = atoi(yytext);
				return CONSTANT;
			}
{POINTER}		{
				copy_string(&yylval.identifier.text,yytext,1,0);
				yylval.identifier.type=SEM_P;
				return POINTER;
			}
{ASSIGN}		{
				yylval.chr=yytext[0];
				return ASSIGN;
			}
{OPERATOR}		{
				copy_string(&yylval.identifier.text,yytext,1,0);
				yylval.identifier.type=SEM_O;
				return OPERATOR;
			}
{WHITESPACE}		;
{NEWLINE}		{
				yylval.chr='\n';
				return NEWLINE;
			}
%%
