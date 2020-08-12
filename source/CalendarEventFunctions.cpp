#include <iostream>
#include <string>
#include <ctime>
#include <fstream>
#include <cstring>

#include "CalendarEventFunctions.h"
#include "CalendarClrscrn.h"
#include "CalendarUserFunctions.h"

/*event_input: writes event for the selected day into the users file - [username].txt*/
int event_input(const User& newUser, int day, int month, int year) {
	std::fstream fp;
	std::string event_text;

	std::cout << "New event:\n";
	std::getline(std::cin, event_text);

	fp.open((newUser.username() + ".txt").c_str(), std::ios::out | std::ios::app);
	if (!fp) {
		std::cerr << "Error - Failed to open file\n";
		return 2;
	}

	text_cypher(event_text, newUser.cypher_key());
	fp << day << month << year << '|' << event_text.c_str() << std::endl;
	fp.close();
	clrscrn(calendar::clrscr);
	return 1;
}

int event_display(const User& newUser, int day, int month, int year) {

	std::fstream fp((newUser.username() + ".txt").c_str(), std::ios::in | std::ios::app);
	if (!fp) {
		std::cerr << "Error - Failed to open file\n";
		return 2;
	}
	fp.seekg(0, fp.beg);

	std::string event_text;
	std::string compare_year = std::to_string(day) + std::to_string(month) + std::to_string(year) + '|';
	int row = 1;

	std::cout << "Events:\n";

	while (!fp.eof()) {
		std::getline(fp, event_text);
		if (!strncmp(event_text.c_str(), compare_year.c_str(), compare_year.size())) {
			std::size_t position = event_text.find('|') + 1;
			std::string encrypted_text = event_text.substr(position, event_text.size());
			text_cypher(encrypted_text, newUser.cypher_key());
			std::cout << row << ". " << encrypted_text << std::endl;
			row++;
		}
	}
	std::cout << "\n\n" << std::endl;
	fp.close();
	return 1;
};

/*text_cypher: encrypts/decrypts event text using the number extracted from the hash - homebrew symmetrical encryption*/
void text_cypher(std::string& text, unsigned char rn) {
	unsigned char rn_num = rn == 0u ? 33u : rn;
	for (std::size_t i = 0; i < text.size(); i++) {
		rn = generate_new_rn(rn, rn_num);
		text[i] ^= rn;
		if (text[i] < 32 || text[i] == 127) { //fixes bugs caused by certain values
			text[i] ^= rn;
		}
	}
}

/*generate_new_rn: generetes new rn - the number used for the simple encryption*/
unsigned char generate_new_rn(unsigned char rn, unsigned char& rn_num) {
	if (rn == 0) {
		rn = rn_num;
	}
	rn_num = rn % 10;
	if (rn_num == 0) {
		rn_num = rn;
	}
	return rn * rn_num;
}

int remove_event(const User& newUser, int day, int month, int year) {
	std::cout << "Choose number tied to the event - 0 to quit\n";
	std::string option_string;
	std::cin >> option_string;
	std::cin.ignore();
	int option = atoi(option_string.c_str());

	if (option == 0) {
		return 0;
	}

	std::fstream fp((newUser.username() + ".txt").c_str(), std::ios::in | std::ios::out);
	if (!fp) {
		clrscrn(calendar::clrscr);
		std::cerr << "Error - Failed to open file\n";
		return 2;
	}
	std::string event_text;
	std::string compare_year = std::to_string(day) + std::to_string(month) + std::to_string(year) + '|';
	int row = 1;

	while (!fp.eof()) {
		std::getline(fp, event_text);
		if (!strncmp(event_text.c_str(), compare_year.c_str(), compare_year.size() - 1)) {
			if (row == option) {
				std::streamoff reverse = fp.tellg();
				reverse -= (event_text.size() + 2ll);
				reverse += CORRECTION;
				fp.seekg(reverse, fp.beg);
				for (unsigned int i = 0; i < compare_year.size(); i++) {
					fp << '0';
					fp.flush();
				}
				fp.close();
				clrscrn(calendar::clrscr);
				return 1;
			}
			row++;
		}
	}
	std::cout << "\nEvent not found\n" << std::endl;
	fp.close();
	return 2;
}