#ifndef ERROR_H
#define ERROR_H

#define ERROR_INTERNAL 0
#define ERROR_USER     1

void ERROR(int type,char *text); 
void WARNING(int type,char *text);

#endif
