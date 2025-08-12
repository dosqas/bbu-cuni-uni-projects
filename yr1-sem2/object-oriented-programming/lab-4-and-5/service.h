#pragma once
#include "repository.h"
#include "domain.h"

class Service {

	private:
		Repository repo = Repository();

	public:
		Service() {}
		~Service() {}

		DynamicArray<class Dog>& getServiceDogs() {
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
			DynamicArray<class Dog>& dogs = repo.getRepoDogs();
			for (int i = 0; i < dogs.getSize(); i++)
				if (dogs[i].get_photograph() == photo)
					return false;
			return true;
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
			DynamicArray<class Dog>& dogs = repo.getRepoDogs();
			for (int i = 0; i < dogs.getSize(); i++)
				if (dogs[i].get_photograph() == photograph)
					return dogs[i].get_adopted();
			return false;
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
			DynamicArray<class Dog>& dogs = repo.getRepoDogs();
			for (int i = 0; i < dogs.getSize(); i++)
				if (dogs[i].get_photograph() == photograph) {
					this->repo.updateRepoDog(dogs[i], name, breed, age);
				}
			return true;
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
			DynamicArray<class Dog>& dogs = repo.getRepoDogs();
			for (int i = 0; i < dogs.getSize(); i++)
				if (dogs[i].get_photograph() == photograph) {
					dogs[i].set_adopted(true);
					return true;
				}
			return false;
		}
};