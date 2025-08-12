#pragma once
#include "repository.h"

typedef struct {
	Repository* repo;
}Service;

Service* createService(Repository* repository);
void destroyService(Service* service);
int addEstate(Service* serv, char* type, char* address, int surface, int price);
int isUnique(Service* serv, char* address);
int removeEstate(Service* serv, char* address);
int updateEstate(Service* serv, char* type, char* address, int surface, int price);
DynamicArray* getAllEstatesByAddress(Service* serv, char* address);
DynamicArray* getAllEstatesByLength(Service* serv, int length);
DynamicArray* getAllEstatesByType(Service* serv, char* type, int surface);
int undoService();
int redoService();