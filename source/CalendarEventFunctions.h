#pragma once
#include "CalendarClassUser.h"

int event_input(const User& newUser, int day, int month, int year);

int event_display(const User& newUser, int day, int month, int year);

int remove_event(const User& newUser, int day, int month, int year);

void text_cypher(std::string& text, unsigned char rn);

unsigned char generate_new_rn(unsigned char rn, unsigned char& rn_num);