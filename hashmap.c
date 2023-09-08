#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <ctype.h>
#include "hashmap.h"


typedef struct HashMap HashMap;
int enlarge_called=0;

struct HashMap {
    Pair ** buckets;
    long size; //cantidad de datos/pairs en la tabla
    long capacity; //capacidad de la tabla
    long current; //indice del ultimo dato accedido
};

Pair * createPair( char * key,  void * value) {
    Pair * new = (Pair *)malloc(sizeof(Pair));
    new->key = key;
    new->value = value;
    return new;
}

long hash( char * key, long capacity) {
    unsigned long hash = 0;
     char * ptr;
    for (ptr = key; *ptr != '\0'; ptr++) {
        hash += hash*32 + tolower(*ptr);
    }
    return hash%capacity;
}

int is_equal(void* key1, void* key2){
    if(key1==NULL || key2==NULL) return 0;
    if(strcmp((char*)key1,(char*)key2) == 0) return 1;
    return 0;
}


void insertMap(HashMap * map, char * key, void * value) 
{
    if (map == NULL || key == NULL || value == NULL) 
        return;

    if ((double)map->size / map->capacity >= 0.7)
        enlarge(map);

    long position = hash(key, map->capacity);
    while (map->buckets[position] != NULL && map->buckets[position]->key != NULL) 
    {
        position = (position + 1) % map->capacity;
    }
    
    if (searchMap(map, key) != NULL) 
        return;

    Pair * newPair = createPair(strdup(key), value);

    map->buckets[position] = newPair;
    map->size++;
    map->current = position;
}



void enlarge(HashMap * map) {
    enlarge_called = 1; //no borrar (testing purposes)


}


HashMap * createMap(long capacity) 
{
    HashMap * map = (HashMap *)malloc(sizeof(HashMap));
    
    if (map == NULL) 
        return NULL;
    
    map->buckets = (Pair **)calloc(capacity, sizeof(Pair *));
    if (map->buckets == NULL) 
    {
        free(map);
        return NULL;
    }
    
    map->capacity = capacity;
    map->size = 0;
    map->current = -1;
    
    return map;
}

void eraseMap(HashMap * map,  char * key) {    


}


Pair * searchMap(HashMap * map, char * key) 
{
    if (map == NULL || key == NULL)
        return NULL;
    
    long position = hash(key, map->capacity);
    while (map->buckets[position] != NULL) 
    {
        if (is_equal(map->buckets[position]->key, key)) 
        {
            map->current = position;
            return map->buckets[position];
        }
        
        position = (position + 1) % map->capacity;
        if (position == hash(key, map->capacity)) 
            break;
    }

    return NULL;
}

Pair * firstMap(HashMap * map) 
{
    if (map == NULL) {
        return NULL;
    }
    
    long position = 0;
    while (position < map->capacity && map->buckets[position] == NULL) 
        position++;
    
    if (position < map->capacity) 
    {
        map->current = position;
        return map->buckets[position];
    }
    
    return NULL;
}

Pair * nextMap(HashMap * map) 
{
    if (map == NULL)
        return NULL;
    

    long position = (map->current + 1) % map->capacity;
    while (position != map->current && (position < map->capacity || map->buckets[position] == NULL))
        position = (position + 1) % map->capacity;

    if (position != map->current && position < map->capacity) 
    {
        map->current = position;
        return map->buckets[position];
    }

    return NULL;
}

