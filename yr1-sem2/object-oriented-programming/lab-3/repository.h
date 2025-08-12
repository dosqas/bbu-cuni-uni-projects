#pragma once
#include "dynamicarray.h"
#include "domain.h"

typedef Estate* TElement;

typedef struct
{
	DynamicArray* arr;

	DynamicArray* arrRepoState;
	int pointOfRepoState;
	int maxPointOfRepoState;

	DynamicArray* undoCommands;
	DynamicArray* redoCommands;
	int pointOfCommands;
	int maxPointOfCommands;

} Repository;


Repository* createRepository();
void destroyRepository(Repository* repo);
int addEstateRepo(Repository* repo, TElement elem);
int removeEstateRepo(Repository* repo, char* address);
int updateElement(Repository* repo, TElement elem);
int isUniqueRepo(Repository* repo, char* address);
DynamicArray* getAll(Repository* repo);
void copyRepoState(Repository* repo);
int undoRepo(Repository* repo);
int redoRepo(Repository* repo);