%{
#include "parser.h"

#define RETURNSTRING(R,del) {                      \
  yylval.str=(char*)malloc(strlen(yytext)+1-del);  \
  strcpy(yylval.str,yytext+del);                   \
  return R;                                        \
}

#define RETURNCHAR(R) {                            \
  yylval.chr=yytext[0];                            \
  return R;                                        \
}

#define RETURNNUM(R)  {                            \
  yylval.num=atoi(yytext);                          \
  return R;                                        \
}


%}

LABEL 			(?i:[a-z][a-zA-Z0-9]*)+":"
FUNCTION   		(?i:pop|return)
UNARYFUNCTION 		(?i:new|delete|jump|readint|writeint|readchar|writechar|top|push|call)
BINARYFUNCTION 		(?i:if)
VARIABLE   		[a-zA-Z][a-zA-Z0-9]*
CONSTANT   		[0-9]*
POINTER    		@{VARIABLE}
ASSIGN     		=
OPERATOR   		"+"|"-"|"*"|"/"|"%"|"&"|"|"|"^"|"!"
WHITESPACE 		[ \t]*
NEWLINE 		"\n"

%%
{LABEL}			RETURNSTRING(LABEL,0)
{FUNCTION}		RETURNSTRING(FUNCTION,0)
{UNARYFUNCTION}		RETURNSTRING(UNARYFUNCTION,0)
{BINARYFUNCTION}	RETURNSTRING(BINARYFUNCTION,0)
{VARIABLE}		RETURNSTRING(VARIABLE,0)
{CONSTANT}		RETURNNUM(CONSTANT)
{POINTER}		RETURNSTRING(POINTER,1)
{ASSIGN}		RETURNCHAR(ASSIGN)
{OPERATOR}		RETURNCHAR(OPERATOR)
{WHITESPACE}		;
{NEWLINE}		RETURNSTRING(NEWLINE,0)
%%
