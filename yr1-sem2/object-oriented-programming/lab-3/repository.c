#include "repository.h"
#include "dynamicarray.h"

Repository* createRepository()
{
	/*
	This function creates a new repository
	Input: none
	Output: repo - pointer to a repository

	returns NULL if the memory for the repository could not be allocated
	*/
	Repository* repo = (Repository*)malloc(sizeof(Repository));
	if (repo == NULL)
		return NULL;
	
	repo->arr = createDynamicArray(10, destroyEstate);
	if (repo->arr == NULL) {
		free(repo);
		return NULL;
	}
	
	repo->arrRepoState = createDynamicArray(10, destroyEstate);
	if (repo->arrRepoState == NULL) {
		free(repo);
		return NULL;
	}
	repo->pointOfRepoState = 0;
	repo->maxPointOfRepoState = 0;

	repo->undoCommands = createDynamicArray(10, destroyEstate);
	if (repo->undoCommands == NULL)
	{
		free(repo);
		return NULL;
	}
	repo->redoCommands = createDynamicArray(10, destroyEstate);
	if (repo->redoCommands == NULL)
	{
		free(repo);
		return NULL;
	}
	repo->pointOfCommands = 0;
	repo->maxPointOfCommands = 0;

	return repo;
}

void destroyRepository(Repository* repo)
{
	/*
	This function destroys a repository
	Input: repo - pointer to a repository
	Output: none
	*/
	destroyDynamicArray(repo->arr);
	free(repo);
}

int isUniqueRepo(Repository* repo, char* address)
{
	/*
	This function checks if an estate is unique
	Input: repo - pointer to a repository
		   address - pointer to a string
	Output: 1 if the estate is unique, 0 otherwise
	*/
	for (int i = 0; i < getSize(repo->arr); i++)
	{
		if (strcmp(getAddress(getElement(repo->arr, i)), address) == 0)
			return 0;
	}
	return 1;
}

int addEstateRepo(Repository* repo, TElement elem)
{
	/*
	This function adds an estate to the repository
	Input: repo - pointer to a repository
		   elem - pointer to an estate
	Output: 1 if the estate was added, 0 otherwise

	returns 0 if the repository or the dynamic array could not be allocated
	*/
	if (repo == NULL || repo->arr == NULL) {
		return 0; 
	}
	DynamicArray* arr = repo->arr;
	addElement(arr, elem);

	 copyRepoState(repo);

	 /*
	DynamicArray* arrRedo = repo->redoCommands;
	DynamicArray* arrUndo = repo->undoCommands;
	addElement(arrRedo, "add");
	addElement(arrRedo, elem);
	addElement(arrUndo, "remove");
	addElement(arrUndo, elem);
	repo->pointOfCommands+=2;
	repo->maxPointOfCommands = repo->pointOfCommands;
	*/

	return 1;
}

int removeEstateRepo(Repository* repo, char* address)
{
	/*
	This function removes an estate from the repository
	Input: repo - pointer to a repository
		   address - pointer to a string
	Output: 1 if the estate was removed, 0 otherwise

	returns 0 if the repository or the dynamic array could not be allocated
	*/
	if (repo == NULL || repo->arr == NULL) {
		return 0;
	}
	DynamicArray* arr = repo->arr;
	for (int i = 0; i < getSize(arr); i++)
	{
		if (strcmp(getAddress(getElement(arr, i)), address) == 0)
		{
			/*
			char type[100], address[100];
			strcpy(type, getType(getElement(arr, i)));
			strcpy(address, getAddress(getElement(arr, i)));
			int surface = getSurface(getElement(arr, i));
			int price = getPrice(getElement(arr, i));
			Estate* copy = createEstate(type, address, surface, price);

			DynamicArray* arrRedo = repo->redoCommands;
			DynamicArray* arrUndo = repo->undoCommands;
			addElement(arrRedo, "remove");
			addElement(arrRedo, copy);
			addElement(arrUndo, "add");
			addElement(arrUndo, copy);
			repo->pointOfCommands += 2;
			repo->maxPointOfCommands = repo->pointOfCommands;
			*/

			removeElement(arr, i);

			copyRepoState(repo);
			return 1;
		}
	}
	return 0;
}

int updateElement(Repository* repo, TElement elem)
{
	/*
	This function updates an estate from the repository
	Input: repo - pointer to a repository
		   elem - pointer to an estate
	Output: 1 if the estate was updated, 0 otherwise

	returns 0 if the repository or the dynamic array could not be allocated
	*/
	if (repo == NULL || repo->arr == NULL) {
		return 0;
	}
	DynamicArray* arr = repo->arr;
	for (int i = 0; i < getSize(arr); i++)
	{
		if (strcmp(getAddress(getElement(arr, i)), getAddress(elem)) == 0)
		{
			/*
			char type[100], address[100];
			strcpy(type, getType(getElement(arr, i)));
			strcpy(address, getAddress(getElement(arr, i)));
			int surface = getSurface(getElement(arr, i));
			int price = getPrice(getElement(arr, i));
			Estate* copy = createEstate(type, address, surface, price);

			DynamicArray* arrRedo = repo->redoCommands;
			DynamicArray* arrUndo = repo->undoCommands;
			addElement(arrRedo, "update");
			addElement(arrRedo, elem);
			addElement(arrUndo, "update");
			addElement(arrUndo, copy);
			repo->pointOfCommands+=2;
			repo->maxPointOfCommands = repo->pointOfCommands;
			*/

			setAddress(getElement(arr, i), getAddress(elem));
			setSurface(getElement(arr, i), getSurface(elem));
			setPrice(getElement(arr, i), getPrice(elem));

			copyRepoState(repo);
			return 1;
		}
	}

	return 0;
}

DynamicArray* getAll(Repository* repo)
{
	/*
	This function gets all the estates from the repository
	Input: repo - pointer to a repository
	Output: a dynamic array with all the estates from the repository
	*/
	return repo->arr;
}

void copyRepoState(Repository* repo)
{
	/*
	This function copies the state of the repository
	Input: repo - pointer to a repository
	Output: none
	*/
	DynamicArray* aux = createDynamicArray(10, destroyEstate);
	for (int i = 0; i < getSize(repo->arr); i++)
	{
		Estate* original = getElement(repo->arr, i);
		char type[100], address[100];
		strcpy(type, getType(original));
		strcpy(address, getAddress(original));
		int surface = getSurface(original);
		int price = getPrice(original);
		Estate* copy = createEstate(type, address, surface, price);
		addElement(aux, copy);
	}
	addElement(repo->arrRepoState, aux);
	repo->pointOfRepoState++;
	repo->maxPointOfRepoState = repo->pointOfRepoState;
}

int undoRepo(Repository* repo)
{
	//This function undoes the last operation
	//Input: repo - pointer to a repository
	//Output: 1 if the operation was undone, 0 otherwise
	//
	//returns 0 if the repository or the dynamic array could not be allocated

    if (repo->pointOfRepoState == 0)
        return 0;
    repo->pointOfRepoState--;
    repo->arr = getElement(repo->arrRepoState, repo->pointOfRepoState);
    return 1;
}

int redoRepo(Repository* repo)
{
	//This function redoes the last operation
	//Input: repo - pointer to a repository
	//Output: 1 if the operation was redone, 0 otherwise
	//
	//returns 0 if the repository or the dynamic array could not be allocated

	if (repo->pointOfRepoState == repo->maxPointOfRepoState)
		return 0;
	repo->pointOfRepoState++;
	repo->arr = getElement(repo->arrRepoState, repo->pointOfRepoState);
	return 1;
}


/*
int undoRepo(Repository* repo)
{
	//This function undoes the last operation
	//Input: repo - pointer to a repository
	//Output: 1 if the operation was undone, 0 otherwise
	//
	//returns 0 if the repository or the dynamic array could not be allocated

	if (repo->pointOfCommands == 0)
		return 0;
	repo->pointOfCommands-= 2;
	DynamicArray* arr = repo->undoCommands;
	DynamicArray* arrRedo = repo->redoCommands;
	if (strcmp(getElement(arr, repo->pointOfCommands), "add") == 0)
	{
		addEstateRepo(repo, getElement(arr, repo->pointOfCommands + 1));
	}
	else if (strcmp(getElement(arr, repo->pointOfCommands), "remove") == 0)
	{
		removeEstateRepo(repo, getAddress(getElement(arr, repo->pointOfCommands + 1)));
	}
	else if (strcmp(getElement(arr, repo->pointOfCommands), "update") == 0)
	{
		updateElement(repo, getElement(arr, repo->pointOfCommands + 1));
	}

	removeElement(arr, getSize(arr));
	removeElement(arr, getSize(arr));
	removeElement(arrRedo, getSize(arrRedo));
	removeElement(arrRedo, getSize(arrRedo));

	repo->pointOfCommands -= 2;
	return 1;
}

int redoRepo(Repository* repo)
{
	//This function redoes the last operation
	//Input: repo - pointer to a repository
	//Output: 1 if the operation was redone, 0 otherwise
	//
	//returns 0 if the repository or the dynamic array could not be allocated
	
	if (repo->pointOfCommands == repo->maxPointOfCommands)
		return 0;
	DynamicArray* arr = repo->redoCommands;
	DynamicArray* arrUndo = repo->undoCommands;
	if (strcmp(getElement(arr, repo->pointOfCommands), "add") == 0)
	{
		addEstateRepo(repo, getElement(arr, repo->pointOfCommands + 1));
	}
	else if (strcmp(getElement(arr, repo->pointOfCommands), "remove") == 0)
	{
		removeEstateRepo(repo, getAddress(getElement(arr, repo->pointOfCommands + 1)));
	}
	else if (strcmp(getElement(arr, repo->pointOfCommands), "update") == 0)
	{
		updateElement(repo, getElement(arr, repo->pointOfCommands + 1));
	}

	removeElement(arr, getSize(arr));
	removeElement(arr, getSize(arr));
	removeElement(arrUndo, getSize(arrUndo));
	removeElement(arrUndo, getSize(arrUndo));

	repo->pointOfCommands += 2;
	return 1;
	
}
*/
