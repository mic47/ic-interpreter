#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include "error.h"

void ERROR(int type,const char*text,...){
	va_list args;
	va_start(args,text);
	if(type==ERROR_INTERNAL){
		fprintf(stderr,"INTERNAL ");
	}
	fprintf(stderr,"ERROR:");
	vfprintf(stderr,text,args);
	int a=5;
	int b=5;
	exit(1/(a-b));
}

void WARNING(int type,const char *text,...){
	va_list args;
	va_start(args,text);
	if(type==ERROR_INTERNAL){
		fprintf(stderr,"INTERNAL ");
	}
	fprintf(stderr,"WARNING");
	vfprintf(stderr,text,args);
}
