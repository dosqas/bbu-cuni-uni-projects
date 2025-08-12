#pragma once
#include "domain.h"
#include "action.h"
#include "repository.h"
#include <algorithm>
using namespace std;

class Service {
private:
	TextRepository repo = TextRepository();
	vector<unique_ptr<Action>> undoActions;
	vector<unique_ptr<Action>> redoActions;

public:
	Service() {}
	~Service() {}

	vector<class Dog>& getServiceDogs() {
		/*
		This function returns the dogs from the repository.
		Input:
			- none
		Output:
		- dogs: DynamicArray<class Dog>&
		*/
		return this->repo.getRepoDogs();
	}

	bool photoIsUnique(const string& photo) {
		/*
		This function checks if a dog with the given photograph link exists in the repository.
		Input:
			- photo: string
		Output:
		- true: bool, if the dog with the given photograph link does not exist in the repository
		- false: bool, if the dog with the given photograph link exists in the repository
		*/
		vector<class Dog>& dogs = repo.getRepoDogs();
		return !any_of(dogs.begin(), dogs.end(),
			[&photo](const Dog& dog) { return dog.get_photograph() == photo; });
	}

	bool adminAddService(const string& name, const string& breed, int age, const string& photograph) {
		/*
		This function adds a dog to the repository.
		Input:
			- name: string
			- breed: string
			- age: int
			- photograph: string
		Output:
		- true: bool, if the dog was successfully added to the repository
		- false: bool, if the dog was not successfully added to the repository
		*/
		class Dog dog = Dog(name, breed, age, photograph);
		this->undoActions.push_back(make_unique<ActionAdd>(repo, dog));
		this->redoActions.clear();
		return this->repo.addRepoDog(dog);
	}

	bool adminRemoveService(const string& photograph) {
		/*
		This function removes a dog from the repository.
		Input:
			- photograph: string
		Output:
		- true: bool, if the dog was successfully removed from the repository
		- false: bool, if the dog was not successfully removed from the repository
		*/
		Dog foundDog = Dog();
		vector<class Dog>& dogs = repo.getRepoDogs();
		for (auto& dog : dogs) {
			if (dog.get_photograph() == photograph) {
				foundDog = dog;
				break;
			}
		}
		this->undoActions.push_back(make_unique<ActionRemove>(repo, foundDog));
		this->redoActions.clear();
		return this->repo.removeRepoDog(photograph);
	}

	bool isAdopted(const string& photograph) {
		/*
		This function checks if a dog with the given photograph link is adopted.
		Input:
			- photograph: string
		Output:
		- true: bool, if the dog with the given photograph link is adopted
		- false: bool, if the dog with the given photograph link is not adopted
		*/
		vector<class Dog>& dogs = repo.getRepoDogs();
		return any_of(dogs.begin(), dogs.end(),
			[&photograph](const Dog& dog) { return dog.get_photograph() == photograph && dog.get_adopted(); });
	}

	bool adminUpdateService(const string& photograph, const string& name, const string& breed, int age) {
		/*
		This function updates a dog from the repository.
		Input:
			- photograph: string
			- name: string
			- breed: string
			- age: int
		Output:
		- true: bool, if the dog was successfully updated in the repository
		- false: bool, if the dog was not successfully updated in the repository
		*/
		vector<class Dog>& dogs = repo.getRepoDogs();
		auto it = find_if(dogs.begin(), dogs.end(),
			[&photograph](const Dog& dog) { return dog.get_photograph() == photograph; });

		if (it != dogs.end()) {
			this->undoActions.push_back(make_unique<ActionUpdate>(repo, *it, Dog(name, breed, age, photograph)));
			this->redoActions.clear();
			this->repo.updateRepoDog(*it, name, breed, age);
			return true;
		}

		if (it != dogs.end()) {
			this->repo.updateRepoDog(*it, name, breed, age);
			return true;
		}
		return false;
	}

	bool userAdoptService(const string& photograph) {
		/*
		This function marks a dog with the given photograph link as adopted.
		Input:
			- photograph: string
		Output:
		- true: bool, if the dog was successfully marked as adopted
		- false: bool, if the dog was not successfully marked as adopted
		*/
		vector<class Dog>& dogs = repo.getRepoDogs();
		auto it = find_if(dogs.begin(), dogs.end(),
			[&photograph](const Dog& dog) { return dog.get_photograph() == photograph; });

		if (it != dogs.end()) {
			it->set_adopted(true);
			return true;
		}
		return false;
	}

	void saveAdoptions(const string type) {
		/*
		This function saves the adopted dogs to a file.
		Input:
			- filename: string
			- type: const char*
		Output:
			- none
		*/
		if (type == "C") {
			repo.outputToCSV();
		}
		else if (type == "H") {
			repo.outputToHTML();
		}
	}

	void saveToFileServ() {
		/*
		This function saves the dogs to a file.
		Input:
			- none
		Output:
			- none
		*/
		repo.saveToFile();
	}

	void loadFromType(string type) {
		/*
		This function loads the dogs from a file.
		Input:
			- none
		Output:
			- none
		*/
		if (type == "C") {
			repo.loadFromCSV();
		}
		else if (type == "H") {
			repo.loadFromHTML();
		}
	}

	int undo() {
		if (this->undoActions.empty())
			return 0;

		unique_ptr<Action> action = move(this->undoActions.back());
		this->undoActions.pop_back();
		action->executeUndo();
		this->redoActions.push_back(move(action));
		return 1;
	}

	int redo() {
		if (this->redoActions.empty())
			return 0;

		unique_ptr<Action> action = move(this->redoActions.back());
		this->redoActions.pop_back();
		action->executeRedo();
		this->undoActions.push_back(move(action));
		return 1;
	}
};