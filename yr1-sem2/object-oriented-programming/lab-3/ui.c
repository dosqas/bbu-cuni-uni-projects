#include "ui.h"
#include <stdio.h>
#include <stdlib.h>
#include <windows.h>

static void functionality_one_menu(UI* ui);
static void add_estate(UI* ui);
static void remove_estate(UI* ui);
static void update_estate(UI* ui);

static void functionality_two_menu(UI* ui);
static void search_by_address(UI* ui);
static void search_by_address_length(UI* ui);

static void functionality_three_menu(UI* ui);
static void display_by_surface_ascending(UI* ui);
static void display_by_surface_descending(UI* ui);

static void functionality_four_menu(UI* ui);
static void undo(UI* ui);
static void redo(UI* ui);

UI* createUI(Service* service)
{
	if (service == NULL) {
		return NULL;
	}
	UI* ui = (UI*)malloc(sizeof(UI));
	if (ui == NULL) {
		return NULL;
	}
	ui->service = service;
	return ui;
}

void destroyUI(UI* ui)
{
	destroyService(ui->service);
	free(ui);
}

int menu(UI* ui)
{
	int choice;
	system("cls");
	printf("[MENU]\n");
	printf("[1] Add/Remove/Update an estate.\n");
	printf("[2] Search for an estate.\n");
	printf("[3] Display all estates of a type having a minimum surface.\n");
	printf("[4] Undo/Redo.\n");
	printf("[5] Exit.\n");

	printf("\nEnter your choice: ");
	(void)scanf("%d", &choice);

	if (choice < 1 || choice > 5) {
		printf("Error: Input must be between 1 and 5.\n\n");
		while (getchar() != '\n')
			continue;

		Sleep(1500);
		return -1;
	}
	switch (choice)
	{
	case 1:
		functionality_one_menu(ui);
		break;
	case 2:
		functionality_two_menu(ui);
		break;
	case 3:
		functionality_three_menu(ui);
		break;
	case 4:
		functionality_four_menu(ui);
		break;
	case 5:
		system("cls");
		printf("Exiting...");
		Sleep(1500);
		return 500;
	}
	return 0;
}

static void functionality_one_menu(UI* ui)
{
	system("cls");
	int choice;
	printf("[OPTION 1]\n");
	printf("[1] Add an estate.\n");
	printf("[2] Remove an estate.\n");
	printf("[3] Update an estate.\n");
	printf("[4] Back.\n");

	printf("\nEnter your choice: ");
	(void)scanf("%d", &choice);

	if (choice < 1 || choice > 4) {
		printf("Error: Input must be between 1 and 4.\n\n");
		while (getchar() != '\n')
			continue;

		Sleep(1500);
		return;
	}

	switch (choice)
	{
	case 1:
		add_estate(ui);
		break;
	case 2:
		remove_estate(ui);
		break;
	case 3:
		update_estate(ui);
		break;
	case 4:
		return;
		break;
	}
}

static void add_estate(UI* ui)
{
	system("cls");
	printf("[ADD ESTATE]\n");
	char address[100], type[100], c;
	int surface, price;

	printf("Enter the type: ");
	(void)scanf("%s", type);
	type[strlen(type)] = '\0';
	if (strcmp(type, "house") != 0 && strcmp(type, "apartment") != 0 && strcmp(type, "penthouse") != 0) {
		printf("Error: type must be house, apartment or penthouse.");
		return;
	}

	printf("Enter the address: ");
	while ((c = getchar()) != '\n' && c != EOF) {}
	fgets(address, sizeof(address), stdin);
	address[strlen(address) - 1] = '\0';
	if (strlen(address) == 0) {
		printf("\nError: Address must not be empty.");
		Sleep(1500);
		return;
	}

	if (!isUnique(ui->service, address)) {
		printf("\nError: Address must be unique. This address already exists.");
		Sleep(1500);
		return;
	}

	printf("Enter the surface: ");
	if (scanf("%d", &surface) != 1) 
	{
		printf("\nError: Invalid input. Please enter a number.");
		while (getchar() != '\n')
			continue;

		Sleep(1500);
		return;
	}
	if (surface < 0) {
		printf("\nError: Surface must be a positive number.");

		Sleep(1500);
		return;
	}

	printf("Enter the price: ");
	if (scanf("%d", &price) != 1)
	{
		printf("\nError: Invalid input. Please enter a number.");
		while (getchar() != '\n')
			continue;

		Sleep(1500);
		return;
	}
	if (price < 0) {
		printf("\nError: Surface must be a positive number.");

		Sleep(1500);
		return;
	}

	int result = addEstate(ui->service, type, address, surface, price);

	if (result == 1)
		printf("\nEstate added successfully!");
	else
		printf("\nError: Estate not added.");

	Sleep(1500);
}

static void remove_estate(UI* ui)
{
	int result = 0;
	system("cls");
	printf("[REMOVE ESTATE]\n");
	char address[100], c;
	printf("Enter the address of the estate you wish removed: ");
	while ((c = getchar()) != '\n' && c != EOF) {}
	fgets(address, sizeof(address), stdin);
	address[strlen(address) - 1] = '\0';
	if (isUnique(ui->service, address))
	{
		printf("\nError: Estate not found.");
		Sleep(1500);
		return;
	}
	else
		result = removeEstate(ui->service, address);

	if (result == 1)
		printf("\nEstate removed successfully!");
	else
		printf("\nError: Estate not removed.");

	Sleep(1500);
}

static void update_estate(UI* ui)
{
	system("cls");
	printf("[UPDATE ESTATE]\n");
	char address[100], type[100], c;
	int surface, price;

	printf("Enter the address of the estate you wish to update: ");
	while ((c = getchar()) != '\n' && c != EOF) {}
	fgets(address, sizeof(address), stdin);
	address[strlen(address) - 1] = '\0';
	if (isUnique(ui->service, address))
	{
		printf("\nError: Estate not found.");

		Sleep(1500);
		return;
	}

	printf("Enter the new type: ");
	(void)scanf("%s", type);
	type[strlen(type)] = '\0';
	if (strcmp(type, "house") != 0 && strcmp(type, "apartment") != 0 && strcmp(type, "penthouse") != 0) {
		printf("\nError: type must be house, apartment or penthouse.");

		Sleep(1500);
		return;
	}

	printf("Enter the new surface: ");
	if (scanf("%d", &surface) != 1)
	{
		printf("\nError: Invalid input. Please enter a number.\n");
		while (getchar() != '\n')
			continue;

		Sleep(1500);
		return;
	}
	if (surface < 0) {
		printf("\nError: Surface must be a positive number.\n\n");

		Sleep(1500);
		return;
	}

	printf("Enter the new price: ");
	if (scanf("%d", &price) != 1)
	{
		printf("\nError: Invalid input. Please enter a number.\n");
		while (getchar() != '\n')
			continue;

		Sleep(1500);
		return;
	}
	if (price < 0) {
		printf("\nError: Surface must be a positive number.\n\n");

		Sleep(1500);
		return;
	}

	int result = updateEstate(ui->service, type, address, surface, price);

	if (result == 1)
		printf("\nEstate updated successfully!");
	else
		printf("\nError: Estate not updated.");
	Sleep(1500);
}


static void functionality_two_menu(UI* ui)
{
	system("cls");
	int choice;
	printf("[OPTION 2]\n");
	printf("[1] Search for an estate by address.\n");
	printf("[2] Search for an estate having an adress longer than or equal to a value.\n");
	printf("[3] Back.\n");

	printf("\nEnter your choice: ");
	(void)scanf("%d", &choice);

	if (choice < 1 || choice > 3) {
		printf("Error: Input must be between 1 and 3.\n\n");
		while (getchar() != '\n')
			continue;

		Sleep(1500);
		return;
	}
	switch (choice)
	{
	case 1:
		search_by_address(ui);
		break;
	case 2:
		search_by_address_length(ui);
		break;
	case 3:
		return;
		break;
	}
}

static void search_by_address(UI* ui)
{
	system("cls");
	printf("[SEARCH BY ADDRESS]\n");
	char address[100];
	int isOnlySpaces = 1, count = 1, c;

	printf("Enter the string you wish to search after:");
	while ((c = getchar()) != '\n' && c != EOF) {}
	fgets(address, sizeof(address), stdin);
	address[strlen(address) - 1] = '\0';
	for (int i = 0; i < strlen(address); i++)
	{
		if (address[i] != ' ')
		{
			isOnlySpaces = 0;
			break;
		}
	}

	printf("\nFound estates:\n");
	DynamicArray* (*getAllEstatesByAdress_ptr)(Service*, char*) = &getAllEstatesByAddress;

	if (isOnlySpaces == 1)
		strcpy(address, "");

	DynamicArray* arr = (*getAllEstatesByAdress_ptr)(ui->service, address);

	for (int i = 0; i < getSize(arr) - 1; i++)
	{
		for (int j = i + 1; j < getSize(arr); j++)
		{
			if (getPrice(getElement(arr, j)) < getPrice(getElement(arr, i)))
			{
				Estate* aux = getElement(arr, i);
				setElement(arr, i, getElement(arr, j));
				setElement(arr, j, aux);
			}
		}
	}

	for (int i = 0; i < getSize(arr); i++)
	{
		Estate* e = getElement(arr, i);
		printf("[%d]\n[TYPE] %s\n[ADDRESS] %s\n[SURFACE] %d\n[PRICE] %d\n\n", count, getType(e), getAddress(e), getSurface(e), getPrice(e));
		count++;
	}

	printf("\nThat's all!");


	Sleep(5000);
}

static void search_by_address_length(UI* ui)
{
	system("cls");
	printf("SEARCH BY ADDRESS LENGTH]\n");
	printf("Enter the minimum length of the address: ");
	int length, count = 1;
	if (scanf("%d", &length) != 1)
	{
		printf("Error: Invalid input. Please enter a number.\n");
		while (getchar() != '\n')
			continue;

		Sleep(1500);
		return;
	}
	if (length < 0) {
		printf("Error: Surface must be a positive number.\n\n");

		Sleep(1500);
		return;
	}

	printf("\nFound estates:\n");
	DynamicArray* (*getAllEstatesByLength_ptr)(Service*, int) = &getAllEstatesByLength;

	DynamicArray* arr = (*getAllEstatesByLength_ptr)(ui->service, length);

	for (int i = 0; i < getSize(arr) - 1; i++)
	{
		for (int j = i + 1; j < getSize(arr); j++)
		{
			if (getPrice(getElement(arr, j)) < getPrice(getElement(arr, i)))
			{
				Estate* aux = getElement(arr, i);
				setElement(arr, i, getElement(arr, j));
				setElement(arr, j, aux);
			}
		}
	}

	for (int i = 0; i < getSize(arr); i++)
	{
		Estate* e = getElement(arr, i);
		printf("[%d]\n[TYPE] %s\n[ADDRESS] %s\n[SURFACE] %d\n[PRICE] %d\n\n", count, getType(e), getAddress(e), getSurface(e), getPrice(e));
		count++;
	}

	printf("\nThat's all!");


	Sleep(5000);
}

static void functionality_three_menu(UI* ui)
{
	system("cls");
	int choice;
	printf("[OPTION 3]\n");
	printf("[1] Display type by surface ascending.\n");
	printf("[2] Display type by surface descending.\n");
	printf("[3] Back.\n");

	printf("\nEnter your choice: ");
	(void)scanf("%d", &choice);

	if (choice < 1 || choice > 3) {
		printf("Error: Input must be between 1 and 3.\n\n");
		while (getchar() != '\n')
			continue;

		Sleep(1500);
		return;
	}
	
	switch (choice)
	{
	case 1:
		display_by_surface_ascending(ui);
		break;
	case 2:
		display_by_surface_descending(ui);
		break;
	case 3:
		return;
		break;
	}
}

static void display_by_surface_ascending(UI* ui)
{
	system("cls");
	printf("[DISPLAY TYPE BY SURFACE ASCENDING]\n");
	char type[100];
	int surface, count = 1;
	printf("Enter the estate type:");
	(void)scanf("%s", type);
	type[strlen(type)] = '\0';
	if (strcmp(type, "house") != 0 && strcmp(type, "apartment") != 0 && strcmp(type, "penthouse") != 0) {
		printf("Error: type must be house, apartment or penthouse.\n\n");

		Sleep(1500);
		return;
	}
	printf("Enter the value to be greater than for the surface of the estates: ");
	if (scanf("%d", &surface) != 1)
	{
		printf("Error: Invalid input. Please enter a number.\n");
		while (getchar() != '\n')
			continue;

		Sleep(1500);
		return;
	}
	if (surface < 0) {
		printf("Error: Surface must be a positive number.\n\n");

		Sleep(1500);
		return;
	}
	printf("\nFound estates:\n");

	DynamicArray* (*getAllEstatesByType_ptr)(Service*, char*, int) = &getAllEstatesByType;

	DynamicArray* arr = (*getAllEstatesByType_ptr)(ui->service, type, surface);

	for (int i = 0; i < getSize(arr) - 1; i++)
	{
		for (int j = i + 1; j < getSize(arr); j++)
		{
			if (getSurface(getElement(arr, j)) < getSurface(getElement(arr, i)))
			{
				Estate* aux = getElement(arr, i);
				setElement(arr, i, getElement(arr, j));
				setElement(arr, j, aux);
			}
		}
	}

	for (int i = 0; i < getSize(arr); i++)
	{
		Estate* e = getElement(arr, i);
		printf("[%d]\n[TYPE] %s\n[ADDRESS] %s\n[SURFACE] %d\n[PRICE] %d\n\n", count, getType(e), getAddress(e), getSurface(e), getPrice(e));
		count++;
	}


	printf("\nThat's all!");
	Sleep(5000);
}

static void display_by_surface_descending(UI* ui)
{
	system("cls");
	printf("[DISPLAY TYPE BY SURFACE DESCENDING]\n");
	char type[100];
	int surface, count = 1;
	printf("Enter the estate type:");
	(void)scanf("%s", type);
	type[strlen(type)] = '\0';
	if (strcmp(type, "house") != 0 && strcmp(type, "apartment") != 0 && strcmp(type, "penthouse") != 0) {
		printf("Error: type must be house, apartment or penthouse.\n\n");

		Sleep(1500);
		return;
	}
	printf("Enter the value to be greater than for the surface of the estates: ");
	if (scanf("%d", &surface) != 1)
	{
		printf("Error: Invalid input. Please enter a number.\n");
		while (getchar() != '\n')
			continue;

		Sleep(1500);
		return;
	}
	if (surface < 0) {
		printf("Error: Surface must be a positive number.\n\n");

		Sleep(1500);
		return;
	}
	printf("\nFound estates:\n");

	DynamicArray* (*getAllEstatesByType_ptr)(Service*, char*, int) = &getAllEstatesByType;

	DynamicArray* arr = (*getAllEstatesByType_ptr)(ui->service, type, surface);

	for (int i = 0; i < getSize(arr) - 1; i++)
	{
		for (int j = i + 1; j < getSize(arr); j++)
		{
			if (getSurface(getElement(arr, j)) > getSurface(getElement(arr, i)))
			{
				Estate* aux = getElement(arr, i);
				setElement(arr, i, getElement(arr, j));
				setElement(arr, j, aux);
			}
		}
	}

	for (int i = 0; i < getSize(arr); i++)
	{
		Estate* e = getElement(arr, i);
		printf("[%d]\n[TYPE] %s\n[ADDRESS] %s\n[SURFACE] %d\n[PRICE] %d\n\n", count, getType(e), getAddress(e), getSurface(e), getPrice(e));
		count++;
	}


	printf("\nThat's all!");
	Sleep(5000);
}

static void functionality_four_menu(UI* ui)
{
	system("cls");
	int choice;
	printf("[OPTION 4]\n");
	printf("[1] Undo the LPO (if possible).\n");
	printf("[2] Redo the LPO (if possible).\n");
	printf("[3] Back.\n");

	printf("\nEnter your choice: ");
	(void)scanf("%d", &choice);

	if (choice < 1 || choice > 3) {
		printf("Error: Input must be between 1 and 3.\n\n");
		while (getchar() != '\n')
			continue;
		return;
	}
	switch (choice)
	{
	case 1:
		undo(ui);
		break;
	case 2:
		redo(ui);
		break;
	case 3:
		return;
		break;
	}
}

static void undo(UI* ui)
{
	system("cls");
	printf("[UNDO]\n");
	int result = undoService(ui->service);
	if (result == 1)
		printf("\nSuccessfully undone the LPO.");
	else
		printf("\nError: Nothing to undo!");
	Sleep(1500);
}

static void redo(UI* ui)
{
	system("cls");
	printf("[REDO]\n");
	int result = redoService(ui->service);
	if (result == 1)
		printf("\nSuccessfully redone the LPO.");
	else
		printf("\nError: Nothing to redo!");
	Sleep(1500);
}