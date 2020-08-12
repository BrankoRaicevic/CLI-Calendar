#pragma once
#include <string>

class User {
	std::string m_user_name;
	unsigned char m_rn;
	unsigned long long m_password;
public:
	User(std::string& username, unsigned char rn);
	std::string username() const;
	unsigned char cypher_key() const;
};