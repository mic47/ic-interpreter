#ifndef MAP_H
#define MAP_H

#define MAP_MIN_SIZE 10

#include "stack.h"

struct _Node{
	char *key;
	int value;
};

typedef struct _Node Node;

struct _Map{
	Node *nodes; 		
	int count,size;
};

typedef struct _Map Map;

void map_init(Map *map);
void map_destroy(Map *map);
void map_set(Map *map, char *key, int value);
int  map_get(Map *map, char *key);
char* map_reverse_get(Map *map,int value);
int  map_exists(Map *map,char *key); /*vrati index alebo -1 ak nie je*/

#endif
