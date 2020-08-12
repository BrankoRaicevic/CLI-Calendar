#include <iostream>
#include <string>
#include <ctime>
#include <fstream>
#include <cstring>

#include "CalendarDateFunctions.h"

int calendar_input(int& day, int& month, int& year) {
	std::string sday, smonth, syear;
	int dd, mm, yy;
	char c = ' ';
	do {
		std::cout << "Enter date | day - month - year: ";
		std::cin >> sday >> smonth >> syear;
		dd = atoi(sday.c_str());
		mm = atoi(smonth.c_str());
		yy = atoi(syear.c_str());
		if (std::cin.peek() != '\n') {
			std::cin.ignore();
			std::cin >> c;
			if (c == 'b' && std::cin.peek() != 'c') { c = ' '; }
		}
		std::cin.ignore(128, '\n');
	} while (!date_in_range(dd, mm, yy));
	day = dd;
	month = mm;
	year = yy;
	if (c == 'b') {
		year *= -1;
	}
	calendar_calculate(dd, mm, yy, c);
	return 0;
}

/*date_in_range: checks if entered date is valid*/
bool date_in_range(int day, int month, int year) {
	int leap_year;

	if (year % 400 == 0) {
		leap_year = 1;
	}
	else if (year % 4 == 0 && year % 100 != 0) {
		leap_year = 1;
	}
	else {
		leap_year = 0;
	}

	switch (month) {
	case 1:
	case 3:
	case 5:
	case 7:
	case 8:
	case 10:
	case 12:
		if (day >= 0 && day <= 31) {
			return true;
		}
		break;
	case 2:
		if (day >= 0 && day <= (28 + leap_year)) {
			return true;
		}
		break;
	case 4:
	case 6:
	case 9:
	case 11:
		if (day >= 0 && day <= 30) {
			return true;
		}
		break;
	default:
		return false;
		break;
	}
	return false;
}

int calendar_calculate(int dd, int mm, int yy, char c) {
	int day = 6, month = 1, year = 2000, md, bc = 0, n = 0;

	if (yy < 0) {   //used in case bc was entered
		yy *= -1;
		c = 'b';
	}

	if (yy == 0) {
		std::cout << "Year 0 isn't a valid year\n";
		return 0;
	}

	if (c == 'b') {
		bc = yy;
		yy--;
		yy = yy * -1;
	}
	while (yy > year) {
		year++;
		day++;
		if (day > 7) { day = 1; }
		if ((year - 1) % 4 == 0) {
			if ((year - 1) % 100 == 0 && (year - 1) % 400 != 0) { /*Do nothing*/ }
			else { day++; }
			if (day > 7) { day = 1; }
		}
	}
	while (yy < year) {
		year--;
		day--;
		if (day < 1) { day = 7; }
		if ((year) % 4 == 0) {
			if ((year) % 100 == 0 && (year) % 400 != 0) { /*Do nothing*/ }
			else { day--; }
			if (day < 1) { day = 7; }
		}
	}
	while (month != mm) {
		if (month == 2) {
			if (year % 400 == 0) { md = 29; }
			else if (year % 4 == 0 && year % 100 != 0) { md = 29; }
			else { md = 28; }
		}
		else if (month == 1 || month == 3 || month == 5 || month == 7 || month == 8 || month == 10 || month == 12) { md = 31; }
		else { md = 30; }
		for (; md > 0; md--) {
			day++;
			if (day > 7) { day = 1; }
		}
		month++;
	}
	if (month == 2) {
		if (year % 400 == 0) { md = 29; }
		else if (year % 4 == 0 && year % 100 != 0) { md = 29; }
		else { md = 28; }
	}
	else if (month == 1 || month == 3 || month == 5 || month == 7 || month == 8 || month == 10 || month == 12) { md = 31; }
	else { md = 30; }

	calendar_draw(dd, day, month, year, md, bc, n);
	return 0;
}

int calendar_draw(int dd, int day, int month, int year, int md, int bc, int n) {
	int red = 1, i;
	if (bc == 0) {
		std::cout << "|" << get_month(month) << "|" << year << '|';
	}
	else {
		std::cout << "|" << get_month(month) << "|" << bc << " BC|";
	}
	std::cout << "\n    Mo. Tu. We. Th. Fr. Sa. Su.\n";
	std::cout << "--------------------------------\n";
	std::cout << red << ":| ";
	for (i = 0; i < (day - 1); i++) { std::cout << "    "; n++; }
	day = 1;
	while (1) {
		if (day < 10) { putwchar(' '); }
		day != dd ? std::cout << day << "  " : std::cout << "\b{" << day << "} ";
		n++;
		day++;
		if (day > md) { break; }
		if (n % 7 == 0) {
			std::cout << "\b|\n";
			red++;
			std::cout << red << ":| ";
		}
	}
	while (n % 7 != 0) {
		n++;
		std::cout << "    ";
	}
	std::cout << "\b|\n";
	std::cout << "--------------------------------\n";
	return 0;
}

/*get_month: accepts numbers and returns corresponding month in std::string form*/
std::string get_month(const int n) {
	switch (n) {
	case 1:
		return "January";
		break;
	case 2:
		return "February";
		break;
	case 3:
		return "March";
		break;
	case 4:
		return "April";
		break;
	case 5:
		return "May";
		break;
	case 6:
		return "Jun";
		break;
	case 7:
		return "July";
		break;
	case 8:
		return "August";
		break;
	case 9:
		return "September";
		break;
	case 10:
		return "October";
		break;
	case 11:
		return "November";
		break;
	case 12:
		return "December";
		break;
	}
	return "Invalid_month";
}



#ifdef _WIN32
/*get_date: returns current date - used for when first entering the calendar*/
void get_date(int& day, int& month, int& year) {
	time_t now = time(0);
	tm ltm;

	localtime_s(&ltm, &now);

	year = 1900 + ltm.tm_year;
	month = 1 + ltm.tm_mon;
	day = ltm.tm_mday;
}
#else
/*get_date: returns current date - used for when first entering the calendar*/
void get_date(int& day, int& month, int& year) {
	time_t now = time(0);
	tm* ltm = localtime(&now);

	year = 1900 + ltm->tm_year;
	month = 1 + ltm->tm_mon;
	day = ltm->tm_mday;
}
#endif