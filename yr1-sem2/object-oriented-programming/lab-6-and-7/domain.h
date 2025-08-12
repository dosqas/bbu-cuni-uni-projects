#pragma once
#include <iostream>
#include <string>
using namespace std;

class Dog
{
	private:
		string name;
		string breed;
		int age;
		string photograph;
		bool adopted;

	public:
		Dog() {
			this->name = "";
			this->breed = "";
			this->age = 0;
			this->photograph = "";
			this->adopted = false;
		}
		Dog(const string& name, const string& breed, const int& age, const string& photograph) {
			this->name = name;
			this->breed = breed;
			this->age = age;
			this->photograph = photograph;
			this->adopted = false;
		}
		Dog(const string& name, const string& breed, const int& age, const string& photograph, const bool& adopted) {
			this->name = name;
			this->breed = breed;
			this->age = age;
			this->photograph = photograph;
			this->adopted = adopted;
		}


		~Dog() {
		}

		string get_name() const { return name; }
		string get_breed() const { return breed; }
		int get_age() const { return age; }
		string get_photograph() const { return photograph; }
		bool get_adopted() const { return adopted; }

		void set_name(const string& name) { this->name = name; }
		void set_breed(const string& breed) { this->breed = breed; }
		void set_age(const int& age) { this->age = age; }
		void set_photograph(const string& photograph) { this->photograph = photograph; }
		void set_adopted(const bool& adopted) { this->adopted = adopted; }

		string dog_to_string() const { return "NAME: " + name + "\nBREED: " + breed + "\nAGE: " + to_string(age) + "\nPHOTOGRAPH LINK: https://kcaaap.com/dogs-photos/" + photograph + "\n\n"; }

		bool operator==(const Dog& d) { return this->name == d.name && this->age == d.age && this->breed == d.breed && this->photograph == d.photograph; }

		bool operator!=(const Dog& d) { return this->name != d.name || this->age != d.age || this->breed != d.breed || this->photograph != d.photograph; }
};