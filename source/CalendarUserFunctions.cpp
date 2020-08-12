#include <iostream>
#include <string>
#include <ctime>
#include <fstream>
#include <cstring>

#include "CalendarUserFunctions.h"
#include "CalendarDateFunctions.h"
#include "CalendarEventFunctions.h"
#include "CalendarClrscrn.h"
#include "CalendarClassUser.h"


void interactive_mode() {
	std::string username = "";
	unsigned char rn = 0;
	int login_success = 0;
	bool repeat = true;

	while (1) {
		switch (main_menu()) {
		case 1:
			login_success = login(username, rn);
			if (login_success) {
				repeat = false;
			}
			break;
		case 2:
			register_user();
			repeat = true;
			break;
		case 0:
			return;
			break;
		default:
			std::cout << "Choose a valid option\n";
			repeat = true;
			break;
		}

		if (repeat == true) {
			continue;
		}
		else {
			repeat = true;
		}

		bool logout = false;
		int day = 0, month = 0, year = 0;
		int options_result = 0;
		bool calendar_is_active = false;

		if (login_success) {
			User newUser(username, rn);
			while (1) {
				switch (user_menu(newUser)) {
				case 1:
					clrscrn(calendar::clrscr);
					get_date(day, month, year);
					calendar_calculate(day, month, year);
					event_display(newUser, day, month, year);
					calendar_is_active = true;
					break;
				case 2:
					clrscrn(calendar::clrscr);
					options_result = options_menu();
					if (options_result == 1) {
						change_password(newUser);
					}
					else if (options_result == 2) {
						clrscrn(calendar::clrscr);
						std::cout << "Are you sure? [y]" << std::endl;
						std::string check = "";
						std::cin >> check;
						if (check != "y") {
							clrscrn(calendar::clrscr);
							std::cout << "Aborted" << std::endl;
						}
						else {
							clrscrn(calendar::clrscr);
							delete_user_data(newUser);
						}
					}
					else if (options_result == 0) {
						clrscrn(calendar::clrscr);
					}
					else {
						std::cout << "Choose a valid option\n";
					}
					break;
				case 0:
					clrscrn(calendar::clrscr);
					logout = true;
					break;
				default:
					clrscrn(calendar::clrscr);
					std::cout << "Choose a valid option\n";
					break;
				}
				if (logout == true) {
					break;
				}
				while (calendar_is_active == true) {
					switch (calendar_menu()) {
					case 1:
						clrscrn(calendar::clrscr);
						calendar_input(day, month, year);
						event_display(newUser, day, month, year);
						break;
					case 2:
						event_input(newUser, day, month, year);
						calendar_calculate(day, month, year);
						event_display(newUser, day, month, year);
						break;
					case 3:
						event_display(newUser, day, month, year);
						remove_event(newUser, day, month, year);
						calendar_calculate(day, month, year);
						event_display(newUser, day, month, year);
						break;
					case 0:
						clrscrn(calendar::clrscr);
						calendar_is_active = false;
						break;
					default:
						clrscrn(calendar::clrscr);
						std::cout << "Choose a valid option\n";
						break;
					}
				}
			}
		}
	}
}

int main_menu() {
	std::cout << "------------------------------\n";
	std::cout << "| Calendar\n";
	std::cout << "------------------------------\n";
	std::cout << "[1] Login\n" << "[2] Sign up\n" << "[0] Exit\n";
	std::string option_string;
	std::cin >> option_string;
	std::cin.ignore();
	for (std::size_t i = 0; i < option_string.size(); i++) {
		if (isalpha(option_string[i])) {
			clrscrn(calendar::clrscr);
			return 0xff;
		}
	}
	int option = std::stoi(option_string);
	clrscrn(calendar::clrscr);
	return option;
}

/*register_user: takes input and if its valid writes a new user into "nusers.bin"*/
int register_user() {
	char* username = new char[65];
	char* password = new char[91];
	std::string salt;
	unsigned long long hashed_password;
	do {
		std::cout << "Username (min 2 / max 64)\n";
		std::cin.getline(username, 64);
	} while (strlen(username) < 2 || strlen(username) > 64);
	while (1) {
		std::cout << "Password (min 6 / max 64):\n";
		std::cin.getline(password, 64);
		hashed_password = MurmurOAAT64(password);
		std::cout << "Confirm: \n";
		std::cin.getline(password, 64);
		bool display_option = false;
		clrscrn(calendar::clrscr);

		if (hashed_password == MurmurOAAT64(password) && strlen(password) >= 6 && strlen(password) <= 64) {
			while (1) {
				std::cout << "\n-----Sign up-------------\n";
				std::cout << " Username: " << username << '\n';
				std::cout << " Password: ";

				if (display_option == false) {
					for (unsigned int i = 0; i < strlen(password); i++) {
						std::cout << '*';
					}
					std::cout << '\n' << "-------------------------" << std::endl;
				}
				else {
					std::cout << password << '\n' << "-------------------------" << std::endl;
				}

				std::cout << "Confirm?\n" << "[1] Yes\n" << "[2] Show/hide password\n" << "[0] Quit" << std::endl;
				int choice;
				std::cin >> choice;
				std::cin.ignore();

				if (choice == 1) {
					unsigned char rn = hashed_password % 255;
					salt = generate_salt(rn);
					std::string stringClassPassword = password;
					hashed_password = MurmurOAAT64((stringClassPassword + salt).c_str());
					delete[] password;

					std::fstream fph("nusers.bin", std::ios::out | std::ios::binary | std::ios::app);
					fph.close();

					std::fstream fp("nusers.bin", std::ios::in | std::ios::binary);

					if (!fp) {
						std::cerr << "Error - Failed to open file\n";
						delete[] username;
						return 2;
					}

					char* username_check = new char[65];
					unsigned long long hashed_password_check;
					while (!fp.eof()) {	//check if username already taken
						fp.read(&username_check[0], 65);
						fp.read((char*)&hashed_password_check, sizeof(unsigned long long));
						if (!strcmp(username_check, username)) {
							std::cout << "Username already taken" << std::endl;
							delete[] username;
							delete[] username_check;
							clrscrn(calendar::clrscr);
							return 1;
						}
					}
					delete[] username_check;
					fp.close();

					fp.open("nusers.bin", std::ios::out | std::ios::binary | std::ios::app);
					if (!fp) {
						std::cerr << "Error - Failed to open file\n";
						delete[] username;
						return 2;
					}
					fp.write(username, 65 * sizeof(char));
					fp.write((char*)&hashed_password, sizeof(unsigned long long));
					delete[] username;
					fp.close();
					clrscrn(calendar::clrscr);
					return 0;
				}

				else if (choice == 2) {
					display_option = display_option == true ? false : true;
					clrscrn(calendar::clrscr);
					continue;
				}

				else if (choice == 0) {
					delete[] username;
					delete[] password;
					clrscrn(calendar::clrscr);
					return 1;
				}

				else {
					std::cout << "Choose a valid option\n";
				}
			}
		}
		else {
			clrscrn(calendar::clrscr);
			std::cout << "Password confirmation failed\n";
			continue;
		}
	}
}

int login(std::string& user, unsigned char& pw) {
	char* username = new char[65];
	char* password = new char[91];
	std::string salt;
	unsigned long long hashed_password;

	std::cout << "-----Login-----" << std::endl;
	while (1) {
		std::cout << " Username: ";
		std::cin.getline(username, 64);
		std::cout << " Password: ";
		std::cin.getline(password, 64);

		hashed_password = MurmurOAAT64(password);
		unsigned char rn = hashed_password % 255;
		salt = generate_salt(rn);
		std::string stringClassPassword = password;
		hashed_password = MurmurOAAT64((stringClassPassword + salt).c_str());

		std::fstream fph("nusers.bin", std::ios::in | std::ios::app);
		if (!fph) {
			clrscrn(calendar::clrscr);
			std::cout << "Error - Failed to open file\n";
			delete[] username;
			delete[] password;
			return 0;
		}
		fph.close();

		std::fstream fp("nusers.bin", std::ios::in | std::ios::binary);
		if (!fp) {
			clrscrn(calendar::clrscr);
			std::cerr << "Error - Failed to open file\n";
			delete[] username;
			delete[] password;
			return 0;
		}
		char* username_check = new char[65];
		unsigned long long hashed_password_check;

		clrscrn(calendar::clrscr);

		while (!fp.eof()) {
			fp.read(&username_check[0], 65);
			fp.read((char*)&hashed_password_check, sizeof(unsigned long long));
			if (!strcmp(username_check, username) && hashed_password == hashed_password_check) {
				std::cout << " \nLogin successful\n" << std::endl;
				user = username;
				pw = rn;
				return 1;
			}
		}
		std::cout << " \nLogin failed\n" << std::endl;
		return 0;
	}
}

/*generate_salt: generates salt by using number extracted from hash*/
std::string generate_salt(unsigned char rn) {
	std::string salt("kale@3d$ajdh!@#21123/';';s");
	for (unsigned int i = 0; i < salt.size(); i++) {
		salt[i] ^= rn;
	}
	return salt;
}

int user_menu(User& user) {
	std::cout << "------------------------------\n";
	std::cout << "| User:  " << user.username() << "\n";
	std::cout << "------------------------------\n\n";
	std::cout << "[1] Calendar\n" << "[2] Options\n" << "[0] Logout\n";
	std::string option_string;
	std::cin >> option_string;
	std::cin.ignore();
	for (std::size_t i = 0; i < option_string.size(); i++) {
		if (isalpha(option_string[i])) {
			clrscrn(calendar::clrscr);
			return 0xff;
		}
	}
	int option = std::stoi(option_string);
	clrscrn(calendar::clrscr);
	return option;
}



int calendar_menu() {
	std::cout << "[1] Enter date\n" << "[2] New event\n" << "[3] Delete event\n" << "[0] Leave\n";
	std::string option_string;
	std::cin >> option_string;
	std::cin.ignore();
	for (std::size_t i = 0; i < option_string.size(); i++) {
		if (isalpha(option_string[i])) {
			clrscrn(calendar::clrscr);
			return 0xff;
		}
	}
	int option = std::stoi(option_string);
	clrscrn(calendar::clrscr);
	return option;
}

int options_menu() {
	std::cout << "[1] Change password\n" << "[2] Delete user data\n" << "[0] Leave\n";
	std::string option_string;
	std::cin >> option_string;
	std::cin.ignore();
	int option = std::stoi(option_string);
	clrscrn(calendar::clrscr);
	return option;
}

void delete_user_data(const User& newUser) {
	if (remove((newUser.username() + ".txt").c_str()) != 0) {
		std::cout << "File deletion failed - couldn't find the file" << std::endl;
	}
	else {
		std::cout << "Data has been deleted" << std::endl;
	}
}

int change_password(const User& newUser) {
	char* password = new char[91];
	std::string salt;
	unsigned long long hashed_password;
	std::fstream fp;
	char* username_check = new char[65];
	unsigned long long hashed_password_check = 15;

	clrscrn(calendar::clrscr);
	std::cout << "Warning - changing your password will delete your user data" << std::endl;

	fp.open("nusers.bin", std::ios::in | std::ios::out | std::ios::binary);
	if (!fp) {
		std::cerr << "Error - failed to open file\n";
		delete[] password;
		delete[] username_check;
		return 2;
	}

	while (!fp.eof()) {
		fp.read(&username_check[0], 65);
		fp.read((char*)&hashed_password_check, sizeof(unsigned long long));
		if (username_check == newUser.username()) {
			int reverse = (sizeof(unsigned long long));
			reverse = 0 - reverse;
			fp.seekg(reverse, fp.cur);
			break;
		}
	}

	int failed_password = 0;

	while (1) {
		std::cout << "Current password: ";
		std::cin.getline(password, 64);
		hashed_password = MurmurOAAT64(password);
		unsigned char rn = hashed_password % 255;
		salt = generate_salt(rn);
		std::string stringClassPassword = password;
		hashed_password = MurmurOAAT64((stringClassPassword + salt).c_str());
		if (hashed_password != hashed_password_check) {
			failed_password++;
			if (failed_password >= 3) {
				fp.close();
				std::cout << "Canceled - 3 times incorrect password" << std::endl;
				delete[] password;
				delete[] username_check;
				return 0;
			}
			continue;
		}
		else {
			break;
		}
	}

	while (1) {
		do {
			std::cout << "New password (min. 6): " << std::endl;
			std::cin.getline(password, 64);
		} while (strlen(password) < 6 || strlen(password) > 64);

		hashed_password = MurmurOAAT64(password);
		unsigned char rn = hashed_password % 255;
		salt = generate_salt(rn);
		hashed_password = MurmurOAAT64((password + salt).c_str());

		std::cout << "Confirm: " << std::endl;
		std::cin.getline(password, 64);
		std::string stringClassPassword = password;
		hashed_password_check = MurmurOAAT64((stringClassPassword + salt).c_str());

		if (hashed_password != hashed_password_check) {
			continue;
		}
		else {
			fp.write((char*)&hashed_password, sizeof(unsigned long long));
			fp.close();
			break;
		}
	}
	delete[] password;
	delete[] username_check;
	clrscrn(calendar::clrscr);
	std::cout << "Password changed" << std::endl;
	delete_user_data(newUser);
	return 1;
}

/*MurmurOAAT64: hash function*/
unsigned long long MurmurOAAT64(const char* key) {
	unsigned long long hash = 525201411107845655ull;
	for (; *key; ++key) {
		hash ^= *key;
		hash *= 0x5bd1e9955bd1e995;
		hash ^= hash >> 47;
	}
	return hash;
}