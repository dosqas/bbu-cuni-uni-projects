#include "dynamicarray.h"
#include <stdlib.h>

DynamicArray* createDynamicArray(int maxCap, DestroyFunction destroyFct)
{
	/*
	This function creates a new dynamic array
	Input: maxCap - integer
		   destroyFct - pointer to a function
	Output: arr - pointer to a dynamic array

	returns NULL if the memory for the dynamic array could not be allocated
	*/
	DynamicArray* arr = malloc(sizeof(DynamicArray));
	if (arr == NULL)
		return NULL;
	arr->capacity = maxCap;
	arr->size = 0;

	arr->elems = malloc(maxCap * sizeof(void*));
	if (arr->elems == NULL) {
		free(arr);
		return NULL;
	}
	arr->destroyFct = destroyFct;
	return arr;
}


void destroyDynamicArray(DynamicArray* arr)
{
	/*
	This function destroys a dynamic array
	Input: arr - pointer to a dynamic array
	Output: none

	returns if the array is NULL
	*/
	if (arr == NULL)
		return;
	for (int i = 0; i < arr->size; i++)
		arr->destroyFct(arr->elems[i]);
	free(arr->elems);
	free(arr);
}

static void resize(DynamicArray* arr)
{
	/*
	This function resizes a dynamic array
	Input: arr - pointer to a dynamic array
	Output: none

	returns if the array is NULL
	*/
	if (arr == NULL)
		return;
	arr->capacity *= 2;
	void** aux = realloc(arr->elems, arr->capacity * sizeof(void*));
	if (aux == NULL)
		return;
	arr->elems = aux;
}

void addElement(DynamicArray* arr, void* elem)
{
	/*
	This function adds an element to a dynamic array
	Input: arr - pointer to a dynamic array
		   elem - pointer to an element
	Output: none
	*/
	if (arr->capacity <= arr->size)
		resize(arr);
	arr->elems[arr->size++] = elem;
}

void removeElement(DynamicArray* arr, int pos)
{
	/*
	This function removes an element from a dynamic array
	Input: arr - pointer to a dynamic array
		   pos - integer
	Output: none

	returns if the array is NULL or if the position is invalid
	*/
	if (arr == NULL || pos < 0 || pos >= arr->size)
		return;
	arr->destroyFct(arr->elems[pos]);
	for (int i = pos; i < arr->size - 1; i++)
		arr->elems[i] = arr->elems[i + 1];
	arr->size--;
}

int getSize(DynamicArray* arr)
{
	/*
	This function gets the size of a dynamic array
	Input: arr - pointer to a dynamic array
	Output: integer

	returns 0 if the array is NULL
	*/
	if (arr == NULL)
		return 0;
	return arr->size;
}

int getCapacity(DynamicArray* arr)
{
	/*
	This function gets the capacity of a dynamic array
	Input: arr - pointer to a dynamic array
	Output: integer

	returns 0 if the array is NULL
	*/
	if (arr == NULL)
		return 0;
	return arr->capacity;
}

void* getElement(DynamicArray* arr, int pos)
{
	/*
	This function gets an element from a dynamic array
	Input: arr - pointer to a dynamic array
		   pos - integer
	Output: pointer to an element

	returns NULL if the array is NULL or if the position is invalid
	*/
	if (arr == NULL || pos < 0 || pos >= arr->size)
		return NULL;
	return arr->elems[pos];
}

void* setElement(DynamicArray* arr, int pos, void* elem)
{
	/*
	This function sets an element in a dynamic array
	Input: arr - pointer to a dynamic array
		   pos - integer
		   elem - pointer to an element
	Output: pointer to an element

	returns NULL if the array is NULL or if the position is invalid
	*/
	if (arr == NULL || pos < 0 || pos >= arr->size)
		return NULL;
	arr->elems[pos] = elem;
}
