/* dArray.h :: Dünaamiline massiiv array'de otsas.
 * Autor: Kristjan Kaitsa
 * 2011 A.D. */
#pragma once
#include <stdlib.h>
#include <math.h>
//#define DEBUG
#ifdef DEBUG
#include <stdio.h>
#endif

typedef struct tagDArray {
	void **data;
	unsigned int size;
	unsigned int capacity;
} dArray;

dArray* dArray_new(unsigned int capacity);
unsigned int dArray_append(dArray *arr, void *data);
void dArray_set(dArray *arr, unsigned int index, void *data);
void* dArray_get(dArray *arr, unsigned int index);
void* dArray_popLast(dArray *arr);
void dArray_swap(dArray *arr, int index1, int index2);
void dArray_truncateBy(dArray *arr, unsigned int removeFromTail);
