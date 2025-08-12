#pragma once
#include "service.h"
#include <string>

class UIValidator
{
public:
	bool main_menu_validator(int input) {
		if (input < 1 || input > 3) {
			return false;
		}
		return true;
	}
	bool save_menu_validator(int input) {
		if (input < 1 || input > 3) {
			return false;
		}
		return true;
	}
	bool admin_menu_validator(int input) {
		if (input < 1 || input > 5) {
			return false;
		}
		return true;
	}
	bool add_menu_validator2(int input) {
		if (input < 0) {
			return false;
		}
		return true;
	}
	bool user_menu_validator(int input) {
		if (input < 1 || input > 5) {
			return false;
		}
		return true;
	}
	bool user_menu_validator2(string save_mode) {
		if (save_mode == "N") {
			return false;
		}
		return true;
	}
	bool user_opt1_validator(char input) {
		if (input != 'y' && input != 'n' && input != 'q') {
			return false;
		}
		return true;
	}
	bool user_opt2_validator(int input) {
		if (input < 1) {
			return false;
		}
		return true;
	}
};