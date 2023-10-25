/*
 *-----------------------------------
 * The Array List Data Structure
 *-----------------------------------
 * To use the ArrayList, you must first specify the type of
 * Array by specifying the size of the element. This is used
 * to calculate the index in memory and allows for this list
 * to be reused with multiple data types.
 *
 *
 *  Created on: 26.01.2022
 *  Author: lucad
 */

#ifndef DART_UTIL_ARRAYLIST_H_
#define DART_UTIL_ARRAYLIST_H_

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

typedef enum{
	ALSUCCESS,
    RMNODATA,
    ALIDXOOR
} globals_t;

typedef struct{
    size_t data_size;   /* the size of the data being stored */
    size_t max_length;  /* the current max length of the list */
    size_t length;      /* the current length of the list */
    void *array;
    int (*add)();
    int (*remove)();
    void *(*get)();
    int (*set)();
    void (*free)();
} ArrayList_t;

ArrayList_t *newArrayList( size_t data_size );
ArrayList_t *newArrayListSized( size_t data_size, size_t initial_length );
int ArrayList__getoffset( ArrayList_t *list, int index );
int ArrayList_add( ArrayList_t *list, void *data );
int ArrayList_remove( ArrayList_t *list, int index );
void ArrayList__realloc( ArrayList_t *list );
void *ArrayList_get( ArrayList_t *list, int index );
int ArrayList_set( ArrayList_t *list, int index, void *data );
void ArrayList_free( ArrayList_t *list );
char *ArrayList_toString( ArrayList_t *list );

#endif
