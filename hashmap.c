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
    if (map == NULL)
        return;

    Pair ** old_buckets = map->buckets;
    long old_capacity = map->capacity;
    map->capacity *= 2;
    map->buckets = (Pair **)calloc(map->capacity, sizeof(Pair *));
    
    if (map->buckets == NULL) {
        map->buckets = old_buckets;
        map->capacity = old_capacity;
        return;
    }

    // Reinicializar el tamaño a 0
    map->size = 0;
    for (long i = 0; i < old_capacity; i++) {
        if (old_buckets[i] != NULL && old_buckets[i]->key != NULL) {
            insertMap(map, old_buckets[i]->key, old_buckets[i]->value);
        }
    }

    free(old_buckets);
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

void eraseMap(HashMap * map, char * key) {
    if (map == NULL || key == NULL)
        return;

    long position = hash(key, map->capacity);
    while (map->buckets[position] != NULL) {
        if (is_equal(map->buckets[position]->key, key)) {
            free(map->buckets[position]->key);
            free(map->buckets[position]->value);
            free(map->buckets[position]);
            map->buckets[position] = NULL;
            map->size--;
            return;
        }
        position = (position + 1) % map->capacity;
        if (position == hash(key, map->capacity))
            break;
    }
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

Pair * firstMap(HashMap * map) {
    if (map == NULL || map->size == 0) {
        return NULL;
    }

    long position = 0;
    while (position < map->capacity) {
        if (map->buckets[position] != NULL && map->buckets[position]->key != NULL) {
            map->current = position;
            return map->buckets[position];
        }
        position++;
    }

    return NULL;
}

Pair * nextMap(HashMap * map) {
    if (map == NULL)
        return NULL;

    long position = (map->current + 1) % map->capacity;
    long initial_position = position;

    while (position != map->current) 
    {
        if (position >= map->capacity) 
            position = 0;

        if (map->buckets[position] != NULL && map->buckets[position]->key != NULL) 
        {
            map->current = position;
            return map->buckets[position];
        }

        position++;

        if (position == initial_position)
            return NULL;
    }

    return NULL;
}
