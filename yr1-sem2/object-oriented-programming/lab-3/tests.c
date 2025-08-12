#include "tests.h"
#include <assert.h>
#include <stdio.h>
#include <string.h>

void testDynamicArray()
{
	DynamicArray* arr = createDynamicArray(2, &destroyEstate);
	assert(arr != NULL);
	assert(arr->capacity == 2);
	assert(arr->size == 0);
	assert(arr->elems != NULL);
	Estate* e1 = createEstate("house", "address1", 100, 1000);
	Estate* e2 = createEstate("apartment", "address2", 200, 2000);
	assert(arr->capacity == 2);
	Estate* e3 = createEstate("penthouse", "address3", 300, 3000);
	assert(arr->size == 0);
	assert(arr->capacity == 2);
	destroyDynamicArray(arr);
	destroyEstate(e1);
	destroyEstate(e2);
	destroyEstate(e3);
}

void testEstate()
{
	Estate* e = createEstate("house", "address1", 100, 1000);
	assert(e != NULL);
	assert(strcmp(getType(e), "house") == 0);
	assert(strcmp(getAddress(e), "address1") == 0);
	assert(getSurface(e) == 100);
	assert(getPrice(e) == 1000);
	destroyEstate(e);
}

void testRepository()
{
	Repository* repo = createRepository();
	assert(repo != NULL);
	assert(repo->arr != NULL);
	assert(repo->undoCommands != NULL);
	assert(repo->redoCommands != NULL);
	assert(repo->pointOfCommands == 0);
	assert(repo->maxPointOfCommands == 0);
	Estate* e1 = createEstate("house", "address1", 100, 1000);
	Estate* e2 = createEstate("apartment", "address2", 200, 2000);
	assert(addEstateRepo(repo, e1) == 1);
	assert(addEstateRepo(repo, e2) == 1);
	assert(getSize(repo->arr) == 2);
	assert(getElement(repo->arr, 0) == e1);
	assert(getElement(repo->arr, 1) == e2);
	assert(updateElement(repo, e1) == 1);
	assert(updateElement(repo, e2) == 1);
	assert(getSize(repo->arr) == 2);
	assert(getElement(repo->arr, 0) == e1);
	assert(getElement(repo->arr, 1) == e2);
	assert(getPrice(getElement(repo->arr, 0)) == 1000);
	assert(getSurface(getElement(repo->arr, 1)) == 200);
	assert(getPrice(getElement(repo->arr, 1)) == 2000);
	assert(getElement(repo->arr, 0) == e1);
	assert(getElement(repo->arr, 1) == e2);
	destroyRepository(repo);
}

void testService()
{
	Repository* repo = createRepository();
	Service* serv = createService(repo);
	assert(serv != NULL);
	assert(serv->repo != NULL);
	Estate* e1 = createEstate("house", "address1", 100, 1000);
	Estate* e2 = createEstate("apartment", "address2", 200, 2000);
	assert(addEstate(serv, "house", "address1", 100, 1000) == 1);
	assert(addEstate(serv, "apartment", "address2", 200, 2000) == 1);
	assert(getSize(serv->repo->arr) == 2);
	assert(strcmp(getType(getElement(serv->repo->arr, 0)), "house") == 0);
	assert(strcmp(getAddress(getElement(serv->repo->arr, 0)), "address1") == 0);
	assert(getSurface(getElement(serv->repo->arr, 0)) == 100);
	assert(getPrice(getElement(serv->repo->arr, 0)) == 1000);
	assert(strcmp(getType(getElement(serv->repo->arr, 1)), "apartment") == 0);
	assert(strcmp(getAddress(getElement(serv->repo->arr, 1)), "address2") == 0);
	assert(getSurface(getElement(serv->repo->arr, 1)) == 200);
	assert(getPrice(getElement(serv->repo->arr, 1)) == 2000);
	assert(getElement(serv->repo->arr, 0) != e1);
	assert(getElement(serv->repo->arr, 1) != e2);
	assert(strcmp(getType(getElement(serv->repo->arr, 0)), "house") == 0);
	assert(strcmp(getAddress(getElement(serv->repo->arr, 0)), "address1") == 0);
	assert(getSurface(getElement(serv->repo->arr, 0)) == 100);
	assert(getPrice(getElement(serv->repo->arr, 0)) == 1000);

	destroyService(serv);
}
