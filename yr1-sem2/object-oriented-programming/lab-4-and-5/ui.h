#pragma once
#include <iostream>
#include <windows.h>
#include <exception>
#include "service.h"
using namespace std;

class UI
{
	private:
		Service serv = Service();
	
	public:
		UI() {}
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
};