#pragma once
#include <Windows.h>
#include <vector>
#include <algorithm>
#include <iostream>
#include <fstream>
#include <string.h>
#include "domain.h"
#include "exceptions.h"
#include <sqltypes.h>
#include <sql.h>
#include <sqlext.h>
#include <codecvt>

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

inline std::wstring ConvertToWideString(const std::string& narrowString) {
	// Create a locale with UTF-8 encoding
	std::locale utf8Locale(std::locale(), new std::codecvt_utf8<wchar_t>());

	// Use std::wstring_convert for conversion
	std::wstring_convert<std::codecvt_utf8<wchar_t>> converter;

	// Convert the narrow string to wide string
	return converter.from_bytes(narrowString);
}

inline void PrintSQLError(SQLHANDLE hHandle) {
	SQLWCHAR sqlState[6];
	SQLINTEGER nativeError;
	SQLWCHAR errMsg[SQL_MAX_MESSAGE_LENGTH];
	SQLSMALLINT errMsgLength;

	// Retrieve and print error information
	SQLRETURN rc;
	SQLSMALLINT i = 1;
	while ((rc = SQLGetDiagRec(SQL_HANDLE_STMT, hHandle, i, sqlState, &nativeError,
		errMsg, sizeof(errMsg), &errMsgLength)) != SQL_NO_DATA) {
		std::wcerr << L"SQL Error: " << errMsg << std::endl;
		i++;
	}
}


class TrueDBRepository : public TextRepository {
private:
	std::wstring connectionString = L"DRIVER={SQL Server};SERVER=.\\SQLEXPRESS;DATABASE=DogDb;Trusted_Connection=Yes;";
	SQLWCHAR* connectionStringPtr = const_cast<SQLWCHAR*>(connectionString.c_str());

public:
	TrueDBRepository() {
		dogs = this->loadFromFile();
		for_each(dogs.begin(), dogs.end(), [this](class Dog& dog) {
			if (dog.get_adopted())
				this->adoptedDogs.push_back(dog);
			});

		this->loadFromFile();
	}

	std::vector<class Dog> loadFromFile() override {
		std::vector<class Dog> dogs;

		// Initialize environment handle
		SQLHENV hEnv;
		SQLAllocHandle(SQL_HANDLE_ENV, SQL_NULL_HANDLE, &hEnv);
		SQLSetEnvAttr(hEnv, SQL_ATTR_ODBC_VERSION, (SQLPOINTER)SQL_OV_ODBC3, SQL_IS_INTEGER);

		// Initialize connection handle
		SQLHDBC hDbc;
		SQLAllocHandle(SQL_HANDLE_DBC, hEnv, &hDbc);

		// Connect to SQL Server
		SQLRETURN retcode = SQLDriverConnectW(hDbc, NULL, connectionStringPtr, SQL_NTS, NULL, 0, NULL, SQL_DRIVER_COMPLETE);

		if (retcode == SQL_SUCCESS || retcode == SQL_SUCCESS_WITH_INFO) {
			// Connection successful
			// std::wcout << L"Connected to SQL Server successfully" << std::endl;
			SQLHSTMT hStmt;
			SQLAllocHandle(SQL_HANDLE_STMT, hDbc, &hStmt);
			SQLWCHAR* query = (SQLWCHAR*)L"SELECT * FROM Dogs";
			retcode = SQLExecDirectW(hStmt, query, SQL_NTS);

			if (retcode == SQL_SUCCESS || retcode == SQL_SUCCESS_WITH_INFO) {
				// Fetch and process results
				while (SQLFetch(hStmt) == SQL_SUCCESS) {
					// Retrieve data from the result set
					SQLCHAR name[100];
					SQLCHAR breed[100];
					SQLINTEGER age;
					SQLCHAR photoLink[100];
					SQLINTEGER adopted;

					SQLGetData(hStmt, 1, SQL_C_CHAR, name, 100, NULL);
					SQLGetData(hStmt, 2, SQL_C_CHAR, breed, 100, NULL);
					SQLGetData(hStmt, 3, SQL_C_LONG, &age, sizeof(SQLINTEGER), NULL);
					SQLGetData(hStmt, 4, SQL_C_CHAR, photoLink, 100, NULL);
					SQLGetData(hStmt, 5, SQL_C_LONG, &adopted, sizeof(SQLINTEGER), NULL);

					// Create a Dog object and add it to the vector
					Dog dog(std::string((const char*)name), std::string((const char*)breed), age, std::string((const char*)photoLink), adopted);
					dogs.push_back(dog);
				}

				// Free the statement handle
				SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
			}
			else {
				// Connection failed
				std::wcerr << L"Failed to connect to SQL Server" << std::endl;

				// Error handling
				SQLWCHAR sqlState[6];
				SQLINTEGER nativeError;
				SQLWCHAR errMsg[SQL_MAX_MESSAGE_LENGTH];
				SQLSMALLINT errMsgLength;

				// Retrieve and print error information
				SQLRETURN rc;
				SQLSMALLINT i = 1;
				while ((rc = SQLGetDiagRec(SQL_HANDLE_DBC, hDbc, i, sqlState, &nativeError,
					errMsg, sizeof(errMsg), &errMsgLength)) != SQL_NO_DATA) {
					std::wcerr << L"SQL Error: " << errMsg << std::endl;
					i++;
				}
			}
		}

		SQLDisconnect(hDbc);
		SQLFreeHandle(SQL_HANDLE_DBC, hDbc);
		SQLFreeHandle(SQL_HANDLE_ENV, hEnv);


		return dogs;
	}

	void saveToFile() override {
		// Initialize environment handle
		SQLHENV hEnv;
		SQLAllocHandle(SQL_HANDLE_ENV, SQL_NULL_HANDLE, &hEnv);
		SQLSetEnvAttr(hEnv, SQL_ATTR_ODBC_VERSION, (SQLPOINTER)SQL_OV_ODBC3, SQL_IS_INTEGER);

		// Initialize connection handle
		SQLHDBC hDbc;
		SQLAllocHandle(SQL_HANDLE_DBC, hEnv, &hDbc);

		// Connect to SQL Server
		SQLRETURN retcode = SQLDriverConnectW(hDbc, NULL, connectionStringPtr, SQL_NTS, NULL, 0, NULL, SQL_DRIVER_COMPLETE);

		if (retcode == SQL_SUCCESS || retcode == SQL_SUCCESS_WITH_INFO) {
			// Connection successful
			//std::wcout << L"Connected to SQL Server successfully" << std::endl;
			SQLHSTMT hStmt;
			SQLAllocHandle(SQL_HANDLE_STMT, hDbc, &hStmt);

			// Execute the SELECT query to fetch data (optional)
			SQLWCHAR* selectQuery = (SQLWCHAR*)L"SELECT * FROM Dogs";
			retcode = SQLExecDirectW(hStmt, selectQuery, SQL_NTS);
			if (retcode != SQL_SUCCESS && retcode != SQL_SUCCESS_WITH_INFO) {
				std::wcerr << L"Error executing SELECT query" << std::endl;
				// Handle error
			}
			SQLCloseCursor(hStmt); // Reset the statement handle
			// Clear the Dogs table
			SQLWCHAR* deleteQuery = (SQLWCHAR*)L"DELETE FROM Dogs";
			retcode = SQLExecDirectW(hStmt, deleteQuery, SQL_NTS);
			if (retcode != SQL_SUCCESS && retcode != SQL_SUCCESS_WITH_INFO) {
				std::wcerr << L"Error clearing Dogs table" << std::endl;
				// Handle error
				PrintSQLError(hStmt);
			}

			// Execute the INSERT statements to add dogs
			for (const auto& dog : dogs) {
				// Construct the INSERT statement
				std::wstring insertStatement = L"INSERT INTO Dogs (Name, Breed, Age, Photo, Is_Adopted) VALUES ('";
				insertStatement += ConvertToWideString(dog.get_name()) + L"', '";
				insertStatement += ConvertToWideString(dog.get_breed()) + L"', ";
				insertStatement += std::to_wstring(dog.get_age()) + L", '";
				insertStatement += ConvertToWideString(dog.get_photograph()) + L"', ";
				insertStatement += dog.get_adopted() ? L"1)" : L"0)";

				// Execute the INSERT statement
				SQLWCHAR* insertQuery = const_cast<SQLWCHAR*>(insertStatement.c_str());
				retcode = SQLExecDirectW(hStmt, insertQuery, SQL_NTS);
				if (retcode != SQL_SUCCESS && retcode != SQL_SUCCESS_WITH_INFO) {
					std::wcerr << L"Error inserting dog data" << std::endl;
					// Handle error
				}
			}

			// Free the statement handle
			SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
		}
	}

	void outputToCSV();
	void outputToHTML();
	void loadFromCSV();
	void loadFromHTML();
};

class CSVOutput : public TrueDBRepository {

	public:
		void saveToFile() override  {
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

class HTMLOutput : public TrueDBRepository {

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

inline void TrueDBRepository::outputToCSV() {
	TrueDBRepository* repo = new CSVOutput();
	repo->saveToFile();
}

inline void TrueDBRepository::outputToHTML() {
	TrueDBRepository* repo = new HTMLOutput();
	repo->saveToFile();
}

inline void TrueDBRepository::loadFromCSV() {
	CSVOutput* repo = new CSVOutput();
	repo->loadFromFile();
}

inline void TrueDBRepository::loadFromHTML() {
	HTMLOutput* repo = new HTMLOutput();
	repo->loadFromFile();
}
