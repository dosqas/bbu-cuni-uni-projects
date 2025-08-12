#pragma once

#include "service.h"
#include <assert.h>
#include "repository.h"
#include "domain.h"
#include "dynamicArray.h"

class Tests
{
	private:

	public:
		Tests() {}
		void test_service()
		{
			Service service = Service();
			assert(service.getServiceDogs().getSize() == 10);
			assert(service.photoIsUnique("photo") == true);
			assert(service.adminAddService("name", "breed", 1, "photo") == true);
			assert(service.isAdopted("photo") == false);
			assert(service.userAdoptService("photo") == true);
			assert(service.adminUpdateService("photo", "newName", "newBreed", 2) == true);
			assert(service.isAdopted("photo") == true);
			assert(service.getServiceDogs().getSize() == 11);
			assert(service.photoIsUnique("photo") == false);
			assert(service.adminRemoveService("photo") == true);
			assert(service.getServiceDogs().getSize() == 10);
			assert(service.userAdoptService("photo") == false);
			assert(service.isAdopted("photo") == false);
		}

		void test_repo()
		{
			Repository repo = Repository();
			assert(repo.getRepoDogs().getSize() == 10);
			assert(repo.removeRepoDog("photo") == true);
			assert(repo.getRepoDogs().getSize() == 10);
			Dog dog = Dog("name", "breed", 1, "photo");
			assert(repo.updateRepoDog(dog, "newName", "newBreed", 2) == true);
		}

		void test_dog()
		{
			Dog dog = Dog("name", "breed", 1, "photo");
			assert(dog.get_name() == "name");
			assert(dog.get_breed() == "breed");
			assert(dog.get_age() == 1);
			assert(dog.get_photograph() == "photo");
			assert(dog.get_adopted() == false);
			dog.set_name("newName");
			dog.set_breed("newBreed");
			dog.set_age(2);
			dog.set_photograph("newPhoto");
			dog.set_adopted(true);
			assert(dog.get_name() == "newName");
			assert(dog.get_breed() == "newBreed");
			assert(dog.get_age() == 2);
			assert(dog.get_photograph() == "newPhoto");
			assert(dog.get_adopted() == true);
			dog.dog_to_string();
		}

		void test_dynamicArray()
		{
			DynamicArray<int> array = DynamicArray<int>();
			assert(array.getSize() == 0);
			array.addElem(1);
			assert(array.getSize() == 1);
			assert(array[0] == 1);
			array.removeElem(0);
			assert(array.getSize() == 0);
		}

		void test_all()
		{
			test_service();
			test_repo();
			test_dog();
			test_dynamicArray();
		}
};