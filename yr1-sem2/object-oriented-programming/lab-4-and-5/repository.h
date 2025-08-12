#pragma once
#include "dynamicArray.h"

class Repository
{
	private:
		DynamicArray<class Dog> dogs = DynamicArray<class Dog>();
		DynamicArray<class Dog> adoptedDogs = DynamicArray<class Dog>();

	public:
		Repository() {
			/*
			This function initializes the repository with 10 dogs.
			Input:
				- none
			*/
			this->dogs.addElem(Dog("Azorel", "Saint Bernard", 3, "azorel.jpg"));
			this->dogs.addElem(Dog("Patrocle", "German Shepherd", 2, "patrocle.jpg"));
			this->dogs.addElem(Dog("Grivei", "Labrador", 1, "grivei.jpg"));
			this->dogs.addElem(Dog("Zdreanta", "Golden Retriever", 4, "zdreanta.jpg"));
			this->dogs.addElem(Dog("Bobita", "Husky", 5, "bobita.jpg"));
			this->dogs.addElem(Dog("Pufi", "Beagle", 4, "pufi.jpg"));
			this->dogs.addElem(Dog("Mircea", "Teckel", 6, "mircea.jpg"));
			this->dogs.addElem(Dog("Ion", "Spanish Cocker", 6, "ion.jpg"));
			this->dogs.addElem(Dog("Elvis", "Dobermann", 8, "elvis.jpg"));
			this->dogs.addElem(Dog("Sebastian", "Dalmatian", 2, "sebastian.jpg"));
		}
		~Repository() {}
		
		DynamicArray<class Dog>& getRepoDogs() {
			/*
			This function returns the dogs from the repository.
			Input:
				- none
			Output:
			- dogs: DynamicArray<class Dog>&
			*/
			return this->dogs;
		}

		bool addRepoDog(class Dog& dog) {
			/*
			This function adds a dog to the repository.
			Input:
				- dog: class Dog&
			Output:
			- true: bool, if the dog was successfully added to the repository
			*/
			this->dogs.addElem(dog);
			return true;
		}

		bool removeRepoDog(const string& photograph) {
			/*
			This function removes a dog from the repository.
			Input:
				- photograph: string
			Output:
			- true: bool, if the dog was successfully removed from the repository
			*/
			for (int i = 0; i < this->dogs.getSize(); i++)
				if (this->dogs[i].get_photograph() == photograph) {
					this->dogs.removeElem(i);
					break;
				}
			return true;
		}

		bool updateRepoDog(class Dog& dog, const string& name, const string& breed, int age) {
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