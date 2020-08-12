#include <iostream>
#include "CalendarClrscrn.h"

#ifdef _WIN32
#define clrscr() system("cls");
#define CORRECTION 0
#else
#define clrscr() std::cout << "\e[1;1H\e[2J";
#define CORRECTION 1
#endif

namespace calendar {
	bool clrscr = true;
}

void clrscrn(const bool permission) {
	if (permission == true) {
		clrscr();
	}
}