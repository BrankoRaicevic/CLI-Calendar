#include <iostream>
#include <string>
#include <ctime>
#include <fstream>
#include <cstring>

#include "CalendarLib.h"


int main(int argc, char* argv[]) {
	if (argc > 3) {
		std::cout << "--Too many argument - all will be ignored" << std::endl;
	}
	else if (argc == 3) {
		int argv_month = std::stoi(argv[1]);
		int argv_year = std::stoi(argv[2]);
		if (argv_month && argv_year) {
			calendar_calculate(0, argv_month, argv_year, ' ');
		}
		return 0;
	}
	else if (argc == 2) {
		if (!strcmp(argv[1], "-clroff")) { //turn off clear screen function
			calendar::clrscr = false;
		}
		else if (std::stoi(argv[1]) != 0) {
			int argv_year = std::stoi(argv[1]);
			calendar::clrscr = false;

			for (int i = 1; i <= 12; ++i) {
				calendar_calculate(0, i, argv_year);
			}
			return 0;
		}
		else {
			std::cout << "--Invalid argument" << std::endl;
			return 1;
		}
	}

	interactive_mode();
	return 0;
}