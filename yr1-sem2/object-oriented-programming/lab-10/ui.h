#pragma once
#include <windows.h>
#include "exceptions.h"
#include "service.h"
#include "validator.h"
#include <algorithm>
#include <exception>
#include <iostream>
#include <string.h>
#include <vector>
using namespace std;

class UI
{
private:
	Service serv = Service();
	UIValidator val = UIValidator();
	string save_mode;

public:
	void question();
	int main_menu();

	void admin_menu();
	void adm_op1();
	void adm_op2();
	void adm_op3();
	void adm_op4();

	void user_menu();
	void user_op1();
	void user_op2();
	void user_op3();
	void user_op4();
};