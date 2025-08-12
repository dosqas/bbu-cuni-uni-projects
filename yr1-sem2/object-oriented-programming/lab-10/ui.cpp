#pragma once
#include "ui.h"

void UI::question()
{
	int option;
	while (1) {
		system("cls");
		cout << "How should we save the adoption list?\n";
		cout << "|1| CSV\n";
		cout << "|2| HTML\n";
		cout << "|3| NO SAVING\n\n";

		cout << "Input: ";
		try
		{
			cin >> option;
			if (!this->val.save_menu_validator(option)) {
				cin.clear();
				char c;
				while ((c = cin.get()) != '\n' && c != EOF) {}
				throw UIException("\nInvalid input! Must be between 1 and 3.");
			}
			break;
		}
		catch (UIException& e)
		{
			cout << e.what() << endl;
			Sleep(1500);
		}
	}

	if (option == 3)
	{
		this->save_mode = "N";
		cout << "\nNo saving then...";

		Sleep(1500);
		return;
	}

	switch (option)
	{
	case 1:
	{
		this->save_mode = "C";
		cout << "\nAdoption list will be saved as CSV!";

		this->serv.saveAdoptions(this->save_mode);
		Sleep(1500);
		break;
	}
	case 2:
	{
		this->save_mode = "H";
		cout << "\nAdoption list will be saved as HTML!";

		this->serv.saveAdoptions(this->save_mode);
		Sleep(1500);
		break;
	}
	}

	return;
}

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
		if (!this->val.main_menu_validator(mode))
			throw UIException("\n\nInvalid input! Must be between 1 and 3.");
	}
	catch (UIException& e)
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
		cout << "SHUTTING DOWN. ";
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
			if (!this->val.admin_menu_validator(option))
				throw UIException("\n\nInvalid input! Must be between 1 and 5.");
		}
		catch (UIException& e)
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
	try {
		cin >> age;
		if (cin.fail()) {
			cin.clear();
			char c;
			while ((c = cin.get()) != '\n' && c != EOF) {}
			throw UIException("\n\nInvalid input! Age must be an integer.");
		}
	}
	catch (UIException& e)
	{
		cout << e.what() << endl;
		Sleep(1500);
		return;
	}

	try {
		if (!this->val.add_menu_validator2(age))
			throw UIException("\n\nInvalid input! Age must be a positive integer.");
	}
	catch (UIException& e)
	{
		cout << e.what() << endl;
		Sleep(1500);
		return;
	}

	cout << "Input photograph link (just the part after https://kcaaap.com/dogs-photos/): ";
	cin >> photograph;
	try {
		if (!serv.photoIsUnique(photograph))
			throw UIException("\n\nInvalid input! Photograph link is not unique!");
	}
	catch (UIException& e)
	{
		cout << e.what() << endl;
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
	try {
		if (res)
			throw UIException("\n\nInvalid input! Photograph link does not exist.");
	}
	catch (UIException& e)
	{
		cout << e.what() << endl;
		Sleep(1500);
		return;
	}
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
	try {
		if (res)
			throw UIException("\n\nInvalid input! Photograph link does not exist.");
	}
	catch (UIException& e)
	{
		cout << e.what() << endl;
		Sleep(1500);
		return;
	}

	cout << "Input new name: ";
	cin >> name;
	cout << "Input new breed: ";
	cin >> breed;
	cout << "Input new age: ";
	cin >> age;
	try {
		if (!val.add_menu_validator2(age))
			throw UIException("\n\nInvalid input! Age must be a positive integer.");
	}
	catch (UIException& e)
	{
		cout << e.what() << endl;
		Sleep(1500);
		return;
	}

	int returnValue = serv.adminUpdateService(photograph, name, breed, age);
	if (returnValue)
		cout << "\nSuccessfully updated the dog!";
	else
		cout << "\nSomething went wrong while trying to update..";

	Sleep(1500);
	return;
}

void UI::adm_op4()
{
	system("cls");

	cout << "   [[USER: admin]]\n";
	cout << "   [SEE ALL DOGS]\n\n";
	cout << "ALL DOGS FOUND:\n";

	vector<class Dog>& dogs = serv.getServiceDogs();
	int index = 1;
	for_each(dogs.begin(), dogs.end(), [&index](const Dog& dog) {
		cout << '[' << index++ << ']' << '\n' << dog.dog_to_string();
		});

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
		cout << "|4| SEE ADOPTION LIST USING CSV/HTML\n";
		cout << "|5| LOG OUT\n\n";

		cout << "Input: ";
		try
		{
			cin >> option;
			if (!this->val.user_menu_validator(option))
				throw UIException("\n\nInvalid input! Must be between 1 and 5.");
		}
		catch (UIException& e)
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
			try {
				if (!this->val.user_menu_validator2(this->save_mode))
					throw UIException("\n\nInvalid choice! No saving mode selected.");
			}
			catch (UIException& e)
			{
				cout << e.what() << endl;
				Sleep(1500);
				break;
			}
			user_op4();
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

void UI::user_op1()
{
	vector<class Dog>& dogs = serv.getServiceDogs();
	int i = 0, nr_of_adoptions = dogs.size(), count = 0;

	nr_of_adoptions -= count_if(dogs.begin(), dogs.end(), [](const Dog& dog) {
		return dog.get_adopted();
		});

	while (1)
	{
		i = i % dogs.size();
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
			try {
				if (!this->val.user_opt1_validator(answer))
					throw UIException("\n\nInvalid input! Must be 'y', 'n' or 'q.");
			}
			catch (UIException& e)
			{
				cout << e.what() << endl;
				Sleep(1500);
				return;
			}
			if (answer == 'y')
			{
				dogs[i].set_adopted(true);
				this->serv.saveToFileServ();
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
	getline(cin, breed);
	cout << "Input age: ";
	cin >> age;
	try {
		if (!this->val.user_opt2_validator(age))
			throw UIException("\n\nInvalid input! Age must be a non-zero, positive integer.");
	}
	catch (UIException& e)
	{
		cout << e.what() << endl;
		Sleep(1500);
		return;
	}

	vector<class Dog>& dogs = serv.getServiceDogs();
	int count = 0;

	cout << "\nALL DOGS FOUND:\n";
	if (breed == "")
	{
		for (int i = 0; i < dogs.size(); i++)
		{
			if (dogs[i].get_age() < age)
			{
				count++;
				cout << '[' << count << ']' << '\n' << dogs[i].dog_to_string();
			}
		}
	}
	else
		for (int i = 0; i < dogs.size(); i++)
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

	vector<class Dog>& dogs = serv.getServiceDogs();
	int count = 0;
	for_each(dogs.begin(), dogs.end(), [&count](const Dog& dog) {
		if (dog.get_adopted()) {
			cout << '[' << ++count << ']' << '\n' << dog.dog_to_string();
		}
		});

	cout << "\nThat's all!";
	Sleep(5000);
	return;
}

void UI::user_op4()
{
	system("cls");

	cout << "   [[USER: user]]\n";
	cout << "   [ADOPTION LIST]\n\n";

	cout << "All adopted dogs will be shown in the corresponding opened window.\n";
	Sleep(1250);

	if (this->save_mode == "H")
	{
		cout << "\nOpening HTML file in browser...";
	}
	else
	{
		cout << "\nOpening CSV file in NotePad...";
	}

	Sleep(1500);
	this->serv.loadFromType(this->save_mode);

	Sleep(2000);
	return;
}