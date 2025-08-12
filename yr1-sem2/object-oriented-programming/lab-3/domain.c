#include "domain.h"
#include <stdlib.h>
#include <string.h>

Estate* createEstate(char* type, char* address, int surface, int price)
{
	/*
	This function creates a new estate
	Input: type - pointer to a string
		   address - pointer to a string
		   surface - integer
		   price - integer
	Output: estate - pointer to an estate

	returns NULL if the memory for the estate could not be allocated
	*/
	Estate* estate = malloc(sizeof(Estate));
	if (estate == NULL)
		return NULL;
	estate->type = malloc(sizeof(char) * (strlen(type) + 1));
	if (estate->type == NULL || strlen(estate->type) == 0) {
		free(estate);
		return NULL;
	}
	strcpy(estate->type, type);
	estate->address = malloc(sizeof(char) * (strlen(address) + 1));
	if (estate->address == NULL) {
		free(estate->type);
		free(estate);
		return NULL;
	}
	strcpy(estate->address, address);
	estate->surface = surface;
	estate->price = price;
	return estate;
}


void destroyEstate(Estate* estate)
{
	/*
	This function destroys an estate
	Input: estate - pointer to an estate
	Output: none

	returns if the estate is NULL
	*/
	if (estate == NULL)
		return;
	free(estate->type);
	free(estate->address);
	free(estate);
}

char* getType(Estate* estate)
{
	/*
	This function gets the type of an estate
	Input: estate - pointer to an estate
	Output: type - pointer to a string
	*/
	return estate->type;
}

char* getAddress(Estate* estate)
{
	/*
	This function gets the address of an estate
	Input: estate - pointer to an estate
	Output: address - pointer to a string
	*/
	return estate->address;
}

int getSurface(Estate* estate)
{
	/*
	This function gets the surface of an estate
	Input: estate - pointer to an estate
	Output: surface - integer
	*/
	return estate->surface;
}

int getPrice(Estate* estate)
{
	/*
	This function gets the price of an estate
	Input: estate - pointer to an estate
	Output: price - integer
	*/
	return estate->price;
}

void setType(Estate* estate, char* type)
{
	/*
	This function sets the type of an estate
	Input: estate - pointer to an estate
		   type - pointer to a string
	Output: none

	returns if the estate or the type is NULL
	*/
	free(estate->type);
	estate->type = malloc(sizeof(char) * (strlen(type) + 1));

	if (estate->type == NULL)
		return;
	strcpy(estate->type, type);
}

void setAddress(Estate* estate, char* address)
{
	/*
	This function sets the address of an estate
	Input: estate - pointer to an estate
		   address - pointer to a string
	Output: none

	returns if the estate or the address is NULL
	*/
	free(estate->address);
	estate->address = malloc(sizeof(char) * (strlen(address) + 1));

	if (estate->address == NULL)
		return;
	strcpy(estate->address, address);
}

void setSurface(Estate* estate, int surface)
{
	/*
	This function sets the surface of an estate
	Input: estate - pointer to an estate
		   surface - integer
	Output: none
	*/
	estate->surface = surface;
}

void setPrice(Estate* estate, int price)
{
	/*
	This function sets the price of an estate
	Input: estate - pointer to an estate
		   price - integer
	Output: none
	*/
	estate->price = price;
}
