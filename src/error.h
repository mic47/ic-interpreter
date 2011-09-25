#ifndef ERROR_H
#define ERROR_H

#define ERROR_INTERNAL 1
#define ERROR_USER     2

void ERROR(int type,const char *text,...); 
void WARNING(int type,const char *text,...);

#endif
