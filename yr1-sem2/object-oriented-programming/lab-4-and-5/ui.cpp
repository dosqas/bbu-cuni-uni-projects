#include "ui.h"
#include "service.h"
#include "dynamicArray.h"
#include <string.h>
#include <algorithm>

int UI::main_menu()
{
	int mode;

	system("cls");
	cout << "   [[KEEP CALM AND ADOPT A PET]]\n\n";
	cout << "CHOOSE USER OR SHUT DOWN SESSION:\n";
	cout << "|1| admin\n";
	cout << "|2| user\n";
	cout << "|3| SHUT DOWN\n\n";

	cout << "Input: ";
	try
	{
		cin >> mode;
		if (mode < 1 || mode > 3)
			throw exception("\n\nInvalid input! Must be between 1 and 3.");
	}
	catch (exception& e)
	{
		cout << e.what() << endl;
		Sleep(1500);
		return -1;
	}

	switch (mode)
	{
	case 1:
	{
		admin_menu();
		break;
	}
	case 2:
	{
		user_menu();
		break;
	}
	case 3:
	{
		system("cls");
		cout<< "SHUTTING DOWN. ";
		Sleep(750);
		cout << ". ";
		Sleep(750);
		cout << ".";

		Sleep(1500);
		return 0;
		break;
	}
	}
	return -1;
}





void UI::admin_menu()
{
	while (1)
	{
		int option;

		system("cls");
		cout << "   [[USER: admin]]\n";
		cout << "   [MAIN MENU]\n\n";
		cout << "CHOOSE OPTION:\n";
		cout << "|1| ADD DOG\n";
		cout << "|2| DELETE DOG\n";
		cout << "|3| UPDATE DOG\n";
		cout << "|4| SEE ALL DOGS\n";
		cout << "|5| LOG OUT\n\n";

		cout << "Input: ";
		try
		{
			cin >> option;
			if (option < 1 || option > 5)
				throw exception("\n\nInvalid input! Must be between 1 and 5.");
		}
		catch (exception& e)
		{
			cout << e.what() << endl;
			Sleep(1500);
		}

		switch (option)
		{
		case 1:
		{
			adm_op1();
			break;
		}
		case 2:
		{
			adm_op2();
			break;
		}
		case 3:
		{
			adm_op3();
			break;
		}
		case 4:
		{
			adm_op4();
			break;
		}
		case 5:
		{
			system("cls");
			cout << "LOGGING OUT. ";
			Sleep(750);
			cout << ". ";
			Sleep(750);
			cout << ".";

			Sleep(1500);
			return;
			break;
		}
		}

	}
}



void UI::adm_op1()
{
	system("cls");
	string breed, name, photograph;
	int age;

	cout << "   [[USER: admin]]\n";
	cout << "   [ADD DOG]\n\n";
	cout << "PLEASE PROVIDE THE FOLLOWING INFORMATION:\n";
	cout << "Input breed: ";
	cin >> breed;
	cout << "Input name: ";
	cin >> name;
	cout << "Input age: ";
	cin >> age;
	if (cin.fail()) {
		cin.clear();
		char c;
		while ((c = cin.get()) != '\n' && c != EOF) {}
		cout << "\nInvalid input! That was not an integer.";

		Sleep(1500);
		return;
	}
		
	if (age < 0)
	{
		cout << "\n\nInvalid input! Age must be a positive integer.\n";
		Sleep(1500);
		return;
	}
	cout << "Input photograph link (just the part after https://kcaaap.com/dogs-photos/): ";
	cin >> photograph;
	if (!serv.photoIsUnique(photograph))
	{
		cout << "\nInvalid input! Photograph link is not unique!";
		Sleep(1500);
		return;
	}
	bool returnValue = serv.adminAddService(breed, name, age, photograph);
	if (returnValue)
		cout << "\nSuccessfully added the dog!";
	else
		cout << "\nSomething went wrong while trying to add..";


	Sleep(1500);
	return;
}

void UI::adm_op2()
{
	system("cls");
	string photograph;

	cout << "   [[USER: admin]]\n";
	cout << "   [REMOVE DOG]\n\n";
	cout << "PLEASE PROVIDE THE FOLLOWING INFORMATION:\n";
	cout << "Input photograph link of the dog you wish deleted (just the part after https://kcaaap.com/dogs-photos/): ";
	cin >> photograph;
	bool res = serv.photoIsUnique(photograph);
	if (res)
		cout << "\nInvalid input! Photograph link does not exist";
	else
	{
		if (!serv.isAdopted(photograph))
		{
			cout << "\nWait! This dog has not been adopted yet! Are you sure you want to remove it? Enter (y/n): ";
			char answer;
			cin >> answer;
			if (answer != 'y' && answer != 'n')
			{
				cout << "\n\nInvalid input! Must be 'y' or 'n'.";
				Sleep(1500);
				return;
			}
			if (answer == 'n') {
				cout << "\n\nOperation cancelled.";
				Sleep(1500);
				return;
			}
		}
		bool returnValue = serv.adminRemoveService(photograph);
		if (returnValue)
			cout << "\nSuccessfully removed the dog!";
		else
			cout << "\nSomething went wrong while trying to remove..";
	}

	Sleep(1500);
	return;
}

void UI::adm_op3()
{
	system("cls");
	string photograph, name, breed;
	int age;

	cout << "   [[USER: admin]]\n";
	cout << "   [UPDATE DOG]\n\n";
	cout << "PLEASE PROVIDE THE FOLLOWING INFORMATION:\n";
	cout << "Input photograph link of the dog you wish to update (just the part after https://kcaaap.com/dogs-photos/): ";
	cin >> photograph;
	bool res = serv.photoIsUnique(photograph);
	if (res)
		cout << "Invalid input! Photograph link does not exist";
	else
	{
		cout << "Input new name: ";
		cin >> name;
		cout << "Input new breed: ";
		cin >> breed;
		cout << "Input new age: ";
		cin >> age;
		if (age < 0)
		{
			cout << "\n\nInvalid input! Age must be a positive integer.\n";
			Sleep(1500);
			return;
		}
		int returnValue = serv.adminUpdateService(photograph, name, breed, age);
		if (returnValue)
			cout << "\nSuccessfully updated the dog!";
		else
			cout << "\nSomething went wrong while trying to update..";
	}

	Sleep(1500);
	return;
}

void UI::adm_op4()
{
	system("cls");

	cout << "   [[USER: admin]]\n";
	cout << "   [SEE ALL DOGS]\n\n";
	cout << "ALL DOGS FOUND:\n";

	DynamicArray<class Dog>& dogs = serv.getServiceDogs();
	for (int i = 0; i < dogs.getSize(); i++)
	{
		cout << '[' << i + 1 << ']' <<'\n' << dogs[i].dog_to_string();
	}

	cout << "\nThat's all!";
	Sleep(5000);
}





void UI::user_menu()
{
	while (1)
	{
		int option;

		system("cls");
		cout << "   [[USER: user]]\n";
		cout << "   [MAIN MENU]\n\n";
		cout << "CHOOSE OPTION:\n";
		cout << "|1| SEE DOGS ONE BY ONE\n";
		cout << "|2| SEE ALL DOGS OF A GIVEN BREED YOUNGER THAN AN AGE\n";
		cout << "|3| SEE ADOPTION LIST\n";
		cout << "|4| LOG OUT\n\n";

		cout << "Input: ";
		try
		{
			cin >> option;
			if (option < 1 || option > 4)
				throw exception("\n\nInvalid input! Must be between 1 and 4.");
		}
		catch (exception& e)
		{
			cout << e.what() << endl;
			Sleep(1500);
		}

		switch (option)
		{
		case 1:
		{
			user_op1();
			break;
		}
		case 2:
		{
			user_op2();
			break;
		}
		case 3:
		{
			user_op3();
			break;
		}
		case 4:
		{
			system("cls");
			cout << "LOGGING OUT. ";
			Sleep(750);
			cout << ". ";
			Sleep(750);
			cout << ".";

			Sleep(1500);
			return;
			break;
		}
		}
	}
}



void UI::user_op1()
{
	DynamicArray<class Dog>& dogs = serv.getServiceDogs();
	int i = 0, nr_of_adoptions = dogs.getSize(), count = 0;

	for (int i = 0; i < dogs.getSize(); i++)
		if (dogs[i].get_adopted())
			nr_of_adoptions--;

	while (1)
	{
		i = i % dogs.getSize();
		count = count % nr_of_adoptions;
		if (!dogs[i].get_adopted())
		{
			system("cls");

			cout << "   [[USER: user]]\n";
			cout << "   [SEE UNADOPTED DOGS]\n\n";
			cout << "DOG NO. " << count + 1 << '\n';

			cout << dogs[i].dog_to_string();
			cout << "\nIf you wish to exit seeing the dogs, type q.";
			cout << "\nDo you want to adopt this dog? Enter (y/n): ";
			char answer;
			cin >> answer;
			if (answer != 'y' && answer != 'n' && answer != 'q')
			{
				cout << "\n\nInvalid input! Must be 'y', 'n' or 'q'.";
				Sleep(1500);
				return;
			}
			if (answer == 'y')
			{
				dogs[i].set_adopted(true);
				cout << "\nSuccessfully adopted the dog!";
				cout << "\nGoing to next dog...";

				nr_of_adoptions--;
				Sleep(1500);
			}
			else if (answer == 'n')
			{
				cout << "\nGoing to next dog...";
				Sleep(1500);
			}
			else
			{
				break;
			}
			count++;
		}
		i++;
	}
	return;
}

void UI::user_op2()
{
	system("cls");

	cout << "   [[USER: user]]\n";
	cout << "   [SEE DOGS OF A GIVEN BREED YOUNGER THAN AN AGE]\n\n";
	cout << "PLEASE PROVIDE THE FOLLOWING INFORMATION:\n";
	string breed;
	int age;

	cout << "Input breed: ";
	cin.ignore();
	getline(cin, breed);
	cout << "Input age: ";
	cin >> age;
	if (age < 1)
	{
		cout << "\n\nInvalid input! Age must be a non-zero, positive integer.\n";
		Sleep(1500);
		return;
	}
	DynamicArray<class Dog>& dogs = serv.getServiceDogs();
	int count = 0;

	cout << "\nALL DOGS FOUND:\n";
	if (breed == "")
	{
		for (int i = 0; i < dogs.getSize(); i++)
		{
			if (dogs[i].get_age() < age)
			{
				count++;
				cout << '[' << count << ']' << '\n' << dogs[i].dog_to_string();
			}
		}
	}
	else
		for (int i = 0; i < dogs.getSize(); i++)
		{
			if (dogs[i].get_breed() == breed && dogs[i].get_age() < age)
			{
				count++;
				cout << '[' << count << ']' << '\n' << dogs[i].dog_to_string();
			}
		}

	cout << "\nThat's all!";
	Sleep(5000);
	return;
}

void UI::user_op3()
{
	system("cls");

	cout << "   [[USER: user]]\n";
	cout << "   [ADOPTION LIST]\n\n";
	cout << "ALL ADOPTED DOGS:\n";

	DynamicArray<class Dog>& dogs = serv.getServiceDogs();
	int count = 0;
	for (int i = 0; i < dogs.getSize(); i++)
	{
		if (dogs[i].get_adopted())
		{
			count++;
			cout << '[' << count << ']' << '\n' << dogs[i].dog_to_string();
		}
	}

	cout << "\nThat's all!";
	Sleep(5000);
	return;
}