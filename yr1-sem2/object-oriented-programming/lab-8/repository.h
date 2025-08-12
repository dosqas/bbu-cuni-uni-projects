#pragma once
#include "domain.h"
#include "exceptions.h"
#include <algorithm>
#include <fstream>
#include <iostream>
#include <string.h>
#include <vector>

using namespace std;

class MemRepository
{
protected:
	vector<class Dog> dogs = vector<class Dog>();
	vector<class Dog> adoptedDogs = vector<class Dog>();

public:
	MemRepository() {
		/*
		This function initializes the repository with 10 dogs.
		Input:
			- none
		*/
		this->dogs.push_back(Dog("Azorel", "Saint Bernard", 3, "azorel.jpg"));
		this->dogs.push_back(Dog("Patrocle", "German Shepherd", 2, "patrocle.jpg"));
		this->dogs.push_back(Dog("Grivei", "Labrador", 1, "grivei.jpg"));
		this->dogs.push_back(Dog("Zdreanta", "Golden Retriever", 4, "zdreanta.jpg"));
		this->dogs.push_back(Dog("Bobita", "Husky", 5, "bobita.jpg"));
		this->dogs.push_back(Dog("Pufi", "Beagle", 4, "pufi.jpg"));
		this->dogs.push_back(Dog("Mircea", "Teckel", 6, "mircea.jpg"));
		this->dogs.push_back(Dog("Ion", "Spanish Cocker", 6, "ion.jpg"));
		this->dogs.push_back(Dog("Elvis", "Dobermann", 8, "elvis.jpg"));
		this->dogs.push_back(Dog("Sebastian", "Dalmatian", 2, "sebastian.jpg"));
	}

	virtual vector<class Dog>& getRepoDogs() {
		/*
		This function returns the dogs from the repository.
		Input:
			- none
		Output:
		- dogs: DynamicArray<class Dog>&
		*/
		return this->dogs;
	}

	virtual bool addRepoDog(class Dog& dog) {
		/*
		This function adds a dog to the repository.
		Input:
			- dog: class Dog&
		Output:
		- true: bool, if the dog was successfully added to the repository
		*/
		this->dogs.push_back(dog);
		return true;
	}

	virtual bool removeRepoDog(const string& photograph) {
		/*
		This function removes a dog from the repository.
		Input:
			- photograph: string
		Output:
		- true: bool, if the dog was successfully removed from the repository
		*/
		this->dogs.erase(remove_if(this->dogs.begin(), this->dogs.end(),
			[&photograph](const Dog& dog) { return dog.get_photograph() == photograph; }), this->dogs.end());
		return true;
	}

	virtual bool updateRepoDog(class Dog& dog, const string& name, const string& breed, int age) {
		/*
		This function updates a dog from the repository.
		Input:
			- dog: class Dog&
			- name: string
			- breed: string
			- age: int
		Output:
		- true: bool, if the dog was successfully updated in the repository
		*/
		dog.set_name(name);
		dog.set_breed(breed);
		dog.set_age(age);
		return true;
	}
};

inline ostream& operator<<(ostream& os, const Dog& dog) {
	os << dog.get_name() << ";" << dog.get_breed() << ";" << dog.get_age() << ";" << dog.get_photograph() << ";";
	if (dog.get_adopted())
		os << "T\n";
	else
		os << "F\n";
	return os;
}

inline istream& operator>>(istream& is, Dog& dog) {
	string name, breed, age, photograph, adopted;
	getline(is, name, ';');
	getline(is, breed, ';');
	getline(is, age, ';');
	getline(is, photograph, ';');
	getline(is, adopted);
	dog.set_name(name);
	dog.set_breed(breed);
	dog.set_age(stoi(age));
	dog.set_photograph(photograph);
	dog.set_adopted(adopted == "T");
	return is;
}

class CSVOutput;
class HTMLOutput;

class TextRepository : public MemRepository {
public:
	TextRepository() {
		dogs = this->loadFromFile();
		for_each(dogs.begin(), dogs.end(), [this](class Dog& dog) {
			if (dog.get_adopted())
				this->adoptedDogs.push_back(dog);
			});
	}

	bool addRepoDog(class Dog& dog) {
		this->dogs.push_back(dog);
		this->saveToFile();
		return true;
	}

	bool removeRepoDog(const string& photograph) {
		this->dogs.erase(remove_if(this->dogs.begin(), this->dogs.end(),
			[&photograph](const Dog& dog) { return dog.get_photograph() == photograph; }), this->dogs.end());
		this->saveToFile();
		return true;
	}

	bool updateRepoDog(class Dog& dog, const string& name, const string& breed, int age) {
		dog.set_name(name);
		dog.set_breed(breed);
		dog.set_age(age);
		this->saveToFile();
		return true;
	}

	virtual vector<class Dog> loadFromFile() {
		/*
		This function loads data from a file.
		Input:
			- none
		Output:
			- none
		*/
		vector<class Dog> dogs = vector<class Dog>();

		ifstream file("repoData.txt");
		try {
			if (!file.is_open())
				throw RepoException("File could not be opened!");
		}
		catch (RepoException& e) {
			cout << e.what() << endl;
			exit(0);
		}

		while (!file.eof()) {
			Dog dog;
			file >> dog;
			dogs.push_back(dog);
		}

		file.close();
		return dogs;
	}

	virtual void saveToFile() {
		ofstream file("repoData.txt", ios::out);
		try {
			if (!file.is_open())
				throw RepoException("File could not be opened!");
		}
		catch (RepoException& e) {
			cout << e.what() << endl;
			exit(0);
		}

		for_each(this->dogs.begin(), this->dogs.end() - 1, [&file](const Dog& dog) {
			file << dog; });
		file << this->dogs.back().get_name() << ";" << this->dogs.back().get_breed() << ";" << this->dogs.back().get_age() << ";" << this->dogs.back().get_photograph() << ";";
		if (this->dogs.back().get_adopted())
			file << "T";
		else
			file << "F";
		file.close();
	}

	void outputToCSV();
	void outputToHTML();
	void loadFromCSV();
	void loadFromHTML();
};


class CSVOutput : public TextRepository {
public:
	void saveToFile() override {
		ofstream file("adoptionsCSV.csv", ios::out);
		try {
			if (!file.is_open())
				throw RepoException("File could not be opened!");
		}
		catch (RepoException& e) {
			cout << e.what() << endl;
			exit(0);
		}

		for_each(this->dogs.begin(), this->dogs.end() - 1, [&file](const Dog& dog) {
			if (dog.get_adopted())
				file << dog.get_name() << "," << dog.get_breed() << "," << dog.get_age() << "," << dog.get_photograph() << "\n";
			});

		if (this->dogs.back().get_adopted())
			file << this->dogs.back().get_name() << "," << this->dogs.back().get_breed() << "," << this->dogs.back().get_age() << "," << this->dogs.back().get_photograph() << "\n";
		file.close();
	}

	vector<class Dog> loadFromFile() override {
		system("start adoptionsCSV.csv");
		vector<class Dog> dogs;
		return dogs;
	}
};

class HTMLOutput : public TextRepository {
public:
	void saveToFile() override {
		ofstream file("adoptionsHTML.html", ios::out);
		try {
			if (!file.is_open())
				throw RepoException("File could not be opened!");
		}
		catch (RepoException& e) {
			cout << e.what() << endl;
			exit(0);
		}

		file << "<!DOCTYPE html><html><head><title>Adoption List</title></head>";
		file << "<body><table border=\"1\"><tr><td>Name</td><td>Breed</td><td>Age</td><td>Photograph</td></tr>";
		for_each(this->dogs.begin(), this->dogs.end(), [&file](const Dog& dog) {
			if (dog.get_adopted())
				file << "<tr><td> " << dog.get_name() << " </td><td> " << dog.get_breed() << " </td><td> " << dog.get_age() << " </td><td><a href=\https://kcaaap.com/dogs-photos/" << dog.get_photograph() << "\>Link</a></td></tr> ";
			});
		file << "</table></body></html>";
		file.close();
	}

	vector<class Dog> loadFromFile() override {
		system("start adoptionsHTML.html");
		vector<class Dog> dogs;
		return dogs;
	}
};

inline void TextRepository::outputToCSV() {
	TextRepository* repo = new CSVOutput();
	repo->saveToFile();
}

inline void TextRepository::outputToHTML() {
	TextRepository* repo = new HTMLOutput();
	repo->saveToFile();
}

inline void TextRepository::loadFromCSV() {
	CSVOutput* repo = new CSVOutput();
	repo->loadFromFile();
}

inline void TextRepository::loadFromHTML() {
	HTMLOutput* repo = new HTMLOutput();
	repo->loadFromFile();
}
