#include <stdio.h>
#include <stdlib.h>
#include "error.h"

void ERROR(int type,char*text){
	if(type==ERROR_INTERNAL){
		fprintf(stderr,"INTERNAL ");
	}
	fprintf(stderr,"ERROR: %s\n",text);
	exit(1);
}

void WARNING(int type,char *text){
	if(type==ERROR_INTERNAL){
		fprintf(stderr,"INTERNAL ");
	}
	fprintf(stderr,"WARNING: %s\n",text);
}
