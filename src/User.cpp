#include "User.h"
#include <iostream>
#include <fstream>
#include <stdexcept>
#include <cctype>
#include <regex>

User::User(const std::string& user, const std::string& pass, const std::string& fName, const std::string& lName, const int& id) 
{
	userID = id;
	if (userID == -1) {
		userID = getNewID();
	}
	
	if (userID == -1) {
		throw std::runtime_error("userID was was not able to be assigned");
	}

	if (validateUsernamePassword(pass, user)) {
		password = pass;
		username = user;
	}
	else {
		throw std::runtime_error("Username/Password do not meet requirements");
	}

	if (validateName(fName, lName)) {
		firstName = fName;
		lastName = lName;
	}
	else {
		throw std::runtime_error("First/last name are not valid");
	}

	std::cout << "new user has been created!" << std::endl;
}

int User::getNewID() const
{
	std::ifstream inputFile("../../../data/IDtrackers/userID.txt");
	int id = -1;

	if (inputFile.is_open()) {
		inputFile >> id;
		inputFile.close();
	}
	else {
		std::cout << "ERROR: Unable to open \"userID.txt\" for READING." << std::endl;
	}

	if (id != -1) {
		std::ofstream outputFile("../../../data/IDtrackers/userID.txt");

		if (outputFile.is_open()) {
			outputFile << id + 1;
			outputFile.close();
		}
		else {
			std::cout << "ERROR: unable to open \"userID.txt\" for WRITING" << std::endl;
		}
	}

	return id;
}

bool User::validateUsernamePassword(const std::string& password, const std::string& username) const
{
	bool valid = true;
	std::regex hasLowercase("[a-z]");
	std::regex hasUppercase("[A-Z]");
	std::regex hasNumber("[0-9]+");
	
	if (password.length() < 4 || username.length() < 4) {
		valid = false;
		std::cout << "ERROR: Username & Password must be 4 or more characters, each" << std::endl;
	}

	if (password.length() > 12 || username.length() > 12) {
		valid = false;
		std::cout << "ERROR: Username & Password must be 12 or less characters, each" << std::endl;
	}

	for (char ch : password) {
		if (!isalnum(static_cast<unsigned char>(ch))) {
			valid = false;
			std::cout << "ERROR: password must only have letters (a-z, A-Z) and numbers (0-9)" << std::endl;
			break;
		}
	}
	for (char ch : username) {
		if (!isalnum(static_cast<unsigned char>(ch))) {
			valid = false;
			std::cout << "ERROR: username must only have letters (a-z, A-Z) and numbers (0-9)" << std::endl;
			break;
		}
	}

	if (!std::regex_search(password, hasLowercase)) {
		valid = false;
		std::cout << "ERROR: password must have at least 1 lowercase letter" << std::endl;
	}

	if (!std::regex_search(password, hasUppercase)) {
		valid = false;
		std::cout << "ERROR: password must have at least 1 uppercase letter" << std::endl;
	}

	if (!std::regex_search(password, hasNumber)) {
		valid = false;
		std::cout << "ERROR: password must have at least 1 number" << std::endl;
	}

	return valid;
}

bool User::validateName(const std::string& fname, const std::string& lname) const
{
	bool valid = true;;

	if (fname.length() < 2 || lname.length() < 2) {
		valid = false;
		std::cout << "ERROR: first/last name must be 2 or more characters, each" << std::endl;
	}

	if (fname.length() > 20 || lname.length() > 20) {
		valid = false;
		std::cout << "ERROR: first/last name must be 20 or less characters, each" << std::endl;
	}

	for (char ch : fname) {
		if (!isalpha(static_cast<unsigned char>(ch))) {
			valid = false;
			std::cout << "ERROR: first name must only have letters (a-z, A-Z)" << std::endl;
			break;
		}
	}

	for (char ch : lname) {
		if (!isalpha(static_cast<unsigned char>(ch))) {
			valid = false;
			std::cout << "ERROR: last name must only have letters (a-z, A-Z)" << std::endl;
			break;
		}
	}

	return valid;
}

User::~User() 
{
	std::cout << "WARNING: user info has been deleted from program: " << firstName << " " << lastName << std::endl;
}

int User::getUserID() const { return userID; }

std::string User::getFirstName() const { return firstName; }

std::string User::getLastName() const { return lastName; }