#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "map.h"
#include "error.h"

void map_init (Map *map) {
	map->nodes = (Node*)malloc (sizeof (Node) * MAP_MIN_SIZE);
	map->count = 0;
	map->size = MAP_MIN_SIZE;
}

void map_destroy (Map *map) {
	free (map->nodes);
	map->nodes = NULL;
	map->count = map->size = 0;
}

void map_force_add (Map *map, char *key, int value) {
	if(map->size==0) ERROR(ERROR_INTERNAL,"Map has zero size!\n");
	int newsize = map->size;
	while (newsize <= map->count) newsize *= 2;
	if (newsize != map->size) {
		map->nodes = (Node*)realloc (map->nodes,sizeof (Node) * newsize);
		map->size = newsize;
	}
	map->nodes[map->count].key = malloc (strlen (key) + 1);
	strcpy (map->nodes[map->count].key, key);
	map->nodes[map->count].value = value;
	map->count++;
}

void map_set (Map *map, char *key, int value) {
	int index = map_exists (map, key);
	if (index >= 0) map->nodes[index].value = value;
	else
		map_force_add (map, key, value);
}

int  map_get (Map *map, char *key) {
	int index = map_exists (map, key);
	if (index >= 0) return map->nodes[index].value;
	map_force_add (map, key, 0);
	return 0;
}

int  map_exists (Map *map, char *key) {
	for (int i = 0; i < map->count; i++) {
		if (!strcmp (key, map->nodes[i].key) ) {
			return i;
		}
	}
	return -1;
}
