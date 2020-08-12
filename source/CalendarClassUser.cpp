#include "CalendarClassUser.h"

User::User(std::string& username, unsigned char rn) {
	this->m_user_name = username;
	this->m_rn = rn;
}

std::string User::username() const {
	return m_user_name;
}

unsigned char User::cypher_key() const {
	return m_rn;
}