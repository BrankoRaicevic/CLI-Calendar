#pragma once
#include <string>

void get_date(int& day, int& month, int& year);

bool date_in_range(int day, int month, int year);

int calendar_input(int& day, int& month, int& year);

int calendar_calculate(int dd, int mm, int yy, char c = ' ');

int calendar_draw(int dd, int day, int month, int year, int md, int bc, int n);

std::string get_month(const int n);