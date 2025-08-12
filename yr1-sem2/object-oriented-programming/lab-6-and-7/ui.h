#pragma once
#include <iostream>
#include <windows.h>
#include <exception>
#include "service.h"
#include "exceptions.h"
#include "validator.h"
#include <string.h>
#include <vector>
#include <algorithm>
using namespace std;

class UI
{
	private:
		Service serv = Service();
		UIValidator val = UIValidator();
		string save_mode;
	
	public:
		UI() {}

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