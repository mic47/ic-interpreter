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
	fprintf(stderr,text,args);
	exit(1);
}

void WARNING(int type,const char *text,...){
	va_list args;
	va_start(args,text);
	if(type==ERROR_INTERNAL){
		fprintf(stderr,"INTERNAL ");
	}
	fprintf(stderr,"WARNING");
	fprintf(stderr,text,args);
}
