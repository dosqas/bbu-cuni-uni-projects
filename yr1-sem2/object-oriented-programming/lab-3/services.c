#include "services.h"
#include "domain.h"
#include "repository.h"


Service* createService(Repository* repo)
{
	/*
	This function creates a new service
	Input: repo - pointer to a repository
	Output: serv - pointer to a service

	returns NULL if the memory for the service could not be allocated
	*/
	if (repo == NULL) {
		return NULL;
	}
	Service* serv = (Service*)malloc(sizeof(Service));
	if (serv == NULL) {
		return NULL;
	}
	serv->repo = repo;
	return serv;
}

void destroyService(Service* serv)
{
	/*
	This function destroys a service
	Input: serv - pointer to a service
	Output: none
	*/
	destroyRepository(serv->repo);
	free(serv);
}

int isUnique(Service* serv, char* address)
{
	/*
	This function checks if an estate is unique
	Input: serv - pointer to a service
		   address - pointer to a string
	Output: 1 if the estate is unique, 0 otherwise
	*/
	return isUniqueRepo(serv->repo, address);
}

int addEstate(Service* serv, char* type, char* address, int surface, int price)
{
	/*
	This function adds an estate to the repository
	Input: serv - pointer to a service
		   type - pointer to a string
		   address - pointer to a string
		   surface - integer
		   price - integer
	Output: 1 if the estate was added, 0 otherwise
	*/
	Estate* e = createEstate(type, address, surface, price);
	return addEstateRepo(serv->repo, e);
}

int removeEstate(Service* serv, char* address)
{
	/*
	This function removes an estate from the repository
	Input: serv - pointer to a service
		   address - pointer to a string
	Output: 1 if the estate was removed, 0 otherwise
	*/
	return removeEstateRepo(serv->repo, address);
}

int updateEstate(Service* serv, char* type, char* address, int surface, int price)
{
	/*
	This function updates an estate from the repository
	Input: serv - pointer to a service
		   type - pointer to a string
		   address - pointer to a string
		   surface - integer
		   price - integer
	Output: 1 if the estate was updated, 0 otherwise
	*/
	Estate* e = createEstate(type, address, surface, price);
	return updateElement(serv->repo, e);
}

DynamicArray* getAllEstatesByAddress(Service* serv, char* address)
{
	/*
	This function gets all the estates from the repository by address
	Input: serv - pointer to a service
		   address - pointer to a string
	Output: a dynamic array with all the estates that have the given address

	returns NULL if the memory for the dynamic array could not be allocated
	*/
	DynamicArray* arr = getAll(serv->repo);
	DynamicArray* result = createDynamicArray(10, destroyEstate);

	if (arr == NULL)
		return NULL;

	if (strcmp(address, "") == 0)
		return arr;

	for (int i = 0; i < getSize(arr); i++)
	{
		if (strstr(getAddress(getElement(arr, i)), address) != NULL)
		{
			addElement(result, getElement(arr, i));
		}
	}
	return result;
}

DynamicArray* getAllEstatesByLength(Service* serv, int length)
{
	/*
	This function gets all the estates from the repository by address length
	Input: serv - pointer to a service
		   length - integer
	Output: a dynamic array with all the estates that have the given address length

	returns NULL if the memory for the dynamic array could not be allocated
	*/
	DynamicArray* arr = getAll(serv->repo);
	DynamicArray* result = createDynamicArray(10, destroyEstate);

	if (arr == NULL)
		return NULL;

	if (length == 0)
		return arr;

	for (int i = 0; i < getSize(arr); i++)
	{
		if (strlen(getAddress(getElement(arr, i))) >= length)
		{
			addElement(result, getElement(arr, i));
		}
	}
	return result;
}

DynamicArray* getAllEstatesByType(Service* serv, char* type, int surface)
{
	/*
	This function gets all the estates from the repository by type and surface
	Input: serv - pointer to a service
		   type - pointer to a string
		   surface - integer
	Output: a dynamic array with all the estates that have the given type and surface

	returns NULL if the memory for the dynamic array could not be allocated
	*/
	DynamicArray* arr = getAll(serv->repo);
	DynamicArray* result = createDynamicArray(10, destroyEstate);

	if (arr == NULL)
		return NULL;

	for (int i = 0; i < getSize(arr); i++)
	{
		if (strcmp(getType(getElement(arr, i)), type) == 0)
		{
			if (getSurface(getElement(arr, i)) > surface)
				addElement(result, getElement(arr, i));
		}
	}
	return result;
}

int undoService(Service* serv)
{
	/*
	This function undoes the last operation
	Input: serv - pointer to a service
	Output: 1 if the operation was undone, 0 otherwise
	*/
	return undoRepo(serv->repo);
}

int redoService(Service* serv)
{	
	/*
	This function redoes the last operation
	Input: serv - pointer to a service
	Output: 1 if the operation was redone, 0 otherwise
	*/
	return redoRepo(serv->repo);
}
