%{
#include <string.h>
#include <ctype.h>
#include "semantics.h"
#include "parser.h"
#include "error.h"

#define YY_USER_ACTION yylloc.first_line = yylloc.last_line = yylineno;

extern int FatalError;

void copy_string(char **dest,char *src,int del_begin,int del_end){
	int len = strlen(src);
	if(len<del_begin+del_end)ERROR(ERROR_INTERNAL,"I want to delete too much from token\n");
	*dest=(char*)malloc(sizeof(char)*len+1-del_begin-del_end);
	strncpy(*dest,src+del_begin,len-del_begin-del_end);
	(*dest)[len-del_begin-del_end]=0;
}


%}
%option yylineno

FUNCTION   		(?-i:pop|return)
UNARYFUNCTION 		(?-i:delete|jump|readint|writeint|readchar|writechar|top|push|call)
BINARYFUNCTION 		(?-i:if|ifeq|ifgeq|ifleq|ifl|ifg|ifneq|new)
VARIABLE   		[a-zA-Z][a-zA-Z0-9]*
CONSTANT   		[0-9]*|~[0-9]*
POINTER    		@{VARIABLE}
STACKPOINTER            "@^"+[0-9][0-9]*
STACKREF 		"^"+[0-9][0-9]*
ASSIGN     		=
OPERATOR   		"+"|"-"|"*"|"/"|"%"|"&"|"|"|"^"|"<<"|">>"
WHITESPACE 		[ \t]*
COMMENT 		"#"+([^\n]*)+"\n"
LABEL 			(?i:[a-z][a-zA-Z0-9]*)+":"+((([ \t\n]*)|{COMMENT})*)
NEWLINE 		"\n"

%%
{LABEL}			{
				//printf("label: %s\n",yytext);
				copy_string(&yylval.identifier.text,yytext,0,strlen(strstr(yytext,":")));
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
				if (yytext[0]=='~') yytext[0]='-';
				yylval.identifier.type=SEM_C;
				yylval.identifier.num = atoi(yytext);
				return CONSTANT;
			}

{STACKREF} 		{
				copy_string(&yylval.identifier.text,yytext,1,0);
				yylval.identifier.type=SEM_S;
				yylval.identifier.num = atoi(yytext+1);
				return STACKREF;
			}

{STACKPOINTER} 		{
				copy_string(&yylval.identifier.text,yytext,2,0);
				yylval.identifier.type=SEM_SP;
				yylval.identifier.num = atoi(yytext+2);
				return STACKPOINTER;
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
				copy_string(&yylval.identifier.text,yytext,0,0);
				yylval.identifier.type=SEM_O;
				return OPERATOR;
			}
{WHITESPACE}		;
{COMMENT}		{unput('\n');}
{NEWLINE}		{
				yylval.chr='\n';
				return NEWLINE;
			}
.			{
				fprintf(stderr,"Warning: Code at line %d contains weird character: \"%s\". Trying to ignore them.\n",yylloc.first_line,yytext);
			}
%%
