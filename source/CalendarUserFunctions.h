#pragma once
#include "CalendarClassUser.h"

#ifdef _WIN32
#define clrscr() system("cls");
#define CORRECTION 0
#else
#define clrscr() std::cout << "\e[1;1H\e[2J";
#define CORRECTION 1
#endif

void interactive_mode();

int main_menu();

int user_menu(User& user);

int calendar_menu();

int options_menu();

void delete_user_data(const User& newUser);

int change_password(const User& newUser);

unsigned long long MurmurOAAT64(const char* key);

std::string generate_salt(unsigned char rn);

int register_user();

int login(std::string& username, unsigned char& pw);