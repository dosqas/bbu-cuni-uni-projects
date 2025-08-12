#pragma once
#include "domain.h"
#include <stdlib.h>

typedef void (*DestroyFunction)(Estate*);

typedef struct {
	int capacity, size;
	void** elems;
	DestroyFunction destroyFct;
} DynamicArray;

DynamicArray* createDynamicArray(int maxCap, DestroyFunction destroyFct);
void destroyDynamicArray(DynamicArray* arr);
void addElement(DynamicArray* arr, void* elem);
void removeElement(DynamicArray* arr, int pos);
int getSize(DynamicArray* arr);
int getCapacity(DynamicArray* arr);
void* getElement(DynamicArray* arr, int pos);
void* setElement(DynamicArray* arr, int pos, void* elem);