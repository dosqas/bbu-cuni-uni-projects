// Real estate agency
/* Evelyn owns a real estate agency.Being also the only employee, she needs an application to help her manage all the real estates 
of her clients.Each Estate has a type(one of house, apartment or penthouse), an address, a surface and a price.Evelyn needs the 
application to help her in the following ways :
(a)Add, delete or update an estate.An estate is uniquely identified by its address.
(b)Display all estates whose address contains a given string(if the string is empty, all estates will be considered), 
sorted ascending by their price. */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct Estate 
{
	char type[15];
	char adress[50];
	int surface;
	int price;
}Estates[100];


void choice1_1(int* estate_count) 
{
	char type[15], adress[200];
	printf("Enter the type of the estate: ");
	(void)scanf("%s", &type);
	if (strcmp(type, "house") != 0 && strcmp(type, "apartment") != 0 && strcmp(type, "penthouse") != 0) 
	{
		printf("Invalid type\n");
		return;
	}
	int c;
	while ((c = getchar()) != '\n' && c != EOF) {}
	printf("Enter the adress of the estate: ");
	fgets(adress, sizeof(adress), stdin);
	adress[strlen(adress) - 1] = '\0';
	for (int i = 0; i < *estate_count; i++)
{
		if (strcmp(Estates[i].adress, adress) == 0) 
		{
			printf("Estate already exists\n");
			return;
		}
	}
	strcpy(Estates[*estate_count].type, type);
	strcpy(Estates[*estate_count].adress, adress);
	printf("Enter the surface of the estate: ");
	(void)scanf("%d", &Estates[*estate_count].surface);
	printf("Enter the price of the estate: ");
	(void)scanf("%d", &Estates[*estate_count].price);
	(*estate_count)++;
	printf("Successfully added estate.\n");
}

void choice1_2(estate_count) 
{
	printf("Enter the adress of the estate you want to delete: ");
	char adress[50];
	(void)scanf("%s", adress);
	for (int i = 0; i < estate_count; i++) 
		{
			if (strcmp(Estates[i].adress, adress) == 0) 
				while (i < estate_count) 
				{
					Estates[i] = Estates[i + 1];
					i++;
				}
		}
}

void choice1_3() 
{
printf("Enter the adress of the estate you want to update: ");
	char adress[50], type[200];
	(void)scanf("%s", adress);
	for (int i = 0; i < 100; i++) 
	{
		if (strcmp(Estates[i].adress, adress) == 0) 
		{
			printf("Enter the new type of the estate: ");
			(void)scanf("%s", type);
			if (strcmp(type, "house") != 0 && strcmp(type, "apartment") != 0 && strcmp(type, "penthouse") != 0) 
				{
					printf("Invalid type\n");
					return;
				}
			strcpy(Estates[i].type, type);
			printf("Enter the new adress of the estate: ");
			(void)scanf("%s", &Estates[i].adress);
			printf("Enter the new surface of the estate: ");
			(void)scanf("%d", &Estates[i].surface);
			printf("Enter the new price of the estate: ");
			(void)scanf("%d", &Estates[i].price);
			break;
		}
	}
}

void menu_choice2(int* count_est) 
{
	int estate_count = *count_est;
	printf("Enter the string: ");
	char string[100];
	int count = 1;
	int c;
	while ((c = getchar()) != '\n' && c != EOF) {}
	fgets(string, sizeof(string), stdin);
	string[strlen(string) - 1] = '\0';
	printf("\nFound estates:\n");
	int isOnlySpaces = 1;
	for (int i = 0; i < strlen(string); i++) 
	{
		if (string[i] != ' ') 
		{
			isOnlySpaces = 0;
			break;
		}
	}
	if (isOnlySpaces)
	{
		for (int i = 0; i < estate_count - 1; i++)
			for (int j = 0; j < estate_count - i - 1; j++)
				if (Estates[j].price > Estates[j + 1].price) 
				{
					struct Estate temp = Estates[j];
					Estates[j] = Estates[j + 1];
					Estates[j + 1] = temp;
				}
		for (int i = 0; i < estate_count; i++) 
		{
				printf("[%d]\nType: %s\nAdress: %s\nSurface: %d\nPrice: %d\n", count, Estates[i].type, Estates[i].adress, Estates[i].surface, Estates[i].price);
				count++;
		}
		return;
	}
	else
	{
		struct Estate temp[100];
		char temp_count = 0;
		for (int i = 0; i < estate_count; i++)
		{
			if (strstr(Estates[i].adress, string) != NULL) 
			{
				temp[temp_count] = Estates[i];
				temp_count++;
			}
		}
		for (int i = 0; i < temp_count - 1; i++)
			for (int j = 0; j < temp_count - i - 1; j++)
				if (temp[j].price > temp[j + 1].price) 
				{
					struct Estate temp2 = temp[j];
					temp[j] = temp[j + 1];
					temp[j + 1] = temp2;
				}
		for (int i = 0; i < temp_count; i++)
		{
				printf("[%d]\nType: %s\nAdress: %s\nSurface: %d\nPrice: %d\n", count, temp[i].type, temp[i].adress, temp[i].surface, temp[i].price);
				count++;
		}
	}
}

int main() {
	int estate_count = 0;
	while (1)
	{
		printf("\n[MENU]\n");
		printf("1. Modify an estate\n2. Find an estate by a string\n3. Quit");
		int choice;
		printf("\nEnter your choice: ");
		(void)scanf("%d", &choice);


		switch (choice)
		{
		case 1:
			printf("\n1. Add an estate\n2. Delete an estate\n3. Update an estate\n");
			int choice2;
			printf("Enter your choice: ");
			(void)scanf("%d", &choice2);
			switch (choice2) 
			{
			case 1:
				printf("\nAdd an estate\n");
				choice1_1(&estate_count);
				break;
			case 2:
				printf("\nDelete an estate\n");
				choice1_2(estate_count);
				estate_count--;
				printf("Successfully deleted estate.\n");
				break;
			case 3:
				printf("\nUpdate an estate\n");
				choice1_3();
				printf("Successfully updated estate.\n");
				break;
			default:
				printf("\nInvalid choice\n");
				break;
			}
			break;
		case 2:
			printf("\nFind an estate by a string\n");
			menu_choice2(&estate_count);
			break;
		case 3:
			printf("\nGoodbye!\n");
			return 0;
			break;
		default:
			printf("Invalid choice\n");
			break;
		}
	}
	return 0;

}