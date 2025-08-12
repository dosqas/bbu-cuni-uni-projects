// Must not leak memory


#include <crtdbg.h>
#include "tests.h"
#include "ui.h"
#include "services.h"
#include "repository.h"

void tests() {
	/*
	This function runs the tests
	Input: none
	Output: none
	*/
	testDynamicArray();
	testEstate();
	testRepository();
	testService();
}

void generateEntries(DynamicArray* arr) {
	/*
	This function generates 10 entries in the repository
	Input: arr - pointer to a dynamic array
	Output: none
	*/
	addElement(arr, createEstate("house", "address1", 156, 1550));
	addElement(arr, createEstate("apartment", "address2", 244, 2040));
	addElement(arr, createEstate("penthouse", "address3", 35, 1300));
	addElement(arr, createEstate("apartment", "address4", 399, 1560));
	addElement(arr, createEstate("apartment", "address5", 190, 3600));
	addElement(arr, createEstate("penthouse", "address6", 43, 900));
	addElement(arr, createEstate("house", "address7", 298, 450));
	addElement(arr, createEstate("house", "address8", 304, 1800));
	addElement(arr, createEstate("house", "address9", 192, 2670));
	addElement(arr, createEstate("apartment", "address10", 98, 4050));
}

int main() {
	Repository* repo = createRepository();
	Service* servs = createService(repo);
	UI* ui = createUI(servs);

	tests();
	generateEntries(servs->repo->arr);

	copyRepoState(repo);
	repo->maxPointOfRepoState = 0;
	repo->pointOfRepoState = 0;

	while (menu(ui) != 500);

	destroyUI(ui);
	if (_CrtDumpMemoryLeaks())
		printf("\n\nNO MEMORY LEAKED");

	return 0;
}