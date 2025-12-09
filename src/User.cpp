#include "User.h"
#include <iostream>
#include <fstream>
#include <stdexcept>
#include <cctype>
#include <regex>

// if 'id' is -1, the user already is a user, which means this constructor is being used to load users from the data folder. 
// if 'id' is not -1, constructor is creating a new user.
User::User(const std::string& user, const std::string& pass, const std::string& fName, const std::string& lName, const int& id) 
{
	// determine if user already has a unique id or not. 
	// if 'id' is -1, user does not have unique id,so user is given a unqiue id.  
	userID = id;
	if (userID == -1) {
		userID = getNewID();
	}
	
	// if program failed to give user a unique id, it will throw an error and cancel the creation of the user.
	if (userID == -1) {
		throw std::runtime_error("userID was was not able to be assigned");
	}

	// if user-given password and username pass validation check, assign user-given values to class values. 
	// otherwise, throws an error and warns the user. cancels creation of user
	if (validateUsernamePassword(pass, user)) {
		password = pass;
		username = user;
	}
	else {
		throw std::runtime_error("Username/Password do not meet requirements");
	}

	// if user-given first/last name pass validatrion check, assign user-given values to class values. 
	// otherwise, throws an error and warns the user. cancels creation of user. 
	if (validateName(fName, lName)) {
		firstName = fName;
		lastName = lName;
	}
	else {
		throw std::runtime_error("First/last name are not valid");
	}

	// Only runs if all validation checks succeed and user is able to be created. 
	std::cout << "new user has been created!" << std::endl;
}

int User::getNewID() const
{
	// relative path to the location of userID file.
	std::ifstream inputFile("../../../data/IDtrackers/userID.txt");

	// default value is -1 & it only changes if inputFile is able to be opened. 
	int id = -1;

	if (inputFile.is_open()) {
		//retrieve 1 number from file. (only 1 number exists in the txt file).
		inputFile >> id;
		inputFile.close();
	}
	else {
		std::cout << "ERROR: Unable to open \"userID.txt\" for READING." << std::endl;
	}

	// if we retrieved number from file successfully, the file is updated (it will increase by 1).
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
	//username & password are valid by default unless we find something wrong with them.
	bool valid = true;

	std::regex hasLowercase("[a-z]");
	std::regex hasUppercase("[A-Z]");
	std::regex hasNumber("[0-9]+");
	
	// if password & username length are not between 4 and 12 characters, they are not valid.
	if (password.length() < 4 || username.length() < 4) {
		valid = false;
		std::cout << "ERROR: Username & Password must be 4 or more characters, each" << std::endl;
	}

	if (password.length() > 12 || username.length() > 12) {
		valid = false;
		std::cout << "ERROR: Username & Password must be 12 or less characters, each" << std::endl;
	}

	// if password contains anything other than alphabet letters or numbers, it is not valid.
	for (char ch : password) {
		if (!isalnum(static_cast<unsigned char>(ch))) {
			valid = false;
			std::cout << "ERROR: password must only have letters (a-z, A-Z) and numbers (0-9)" << std::endl;
			break;
		}
	}
	// if username contains anything other than alphabet letters, it is not valid.
	for (char ch : username) {
		if (!isalnum(static_cast<unsigned char>(ch))) {
			valid = false;
			std::cout << "ERROR: username must only have letters (a-z, A-Z) and numbers (0-9)" << std::endl;
			break;
		}
	}

	// if password does not have at least 1 lowercase letter, it is not valid.
	if (!std::regex_search(password, hasLowercase)) {
		valid = false;
		std::cout << "ERROR: password must have at least 1 lowercase letter" << std::endl;
	}

	// if password does not have at least 1 uppercase letter, it is not vald. 
	if (!std::regex_search(password, hasUppercase)) {
		valid = false;
		std::cout << "ERROR: password must have at least 1 uppercase letter" << std::endl;
	}

	// if password does not have at least 1 number, it is not valid. 
	if (!std::regex_search(password, hasNumber)) {
		valid = false;
		std::cout << "ERROR: password must have at least 1 number" << std::endl;
	}

	return valid;
}

bool User::validateName(const std::string& fname, const std::string& lname) const
{
	// first/last name are valid by default unless we find something wrong with them.
	bool valid = true;;

	// if first/last name are not between 2 and 20 characters, they are not valid. 
	if (fname.length() < 2 || lname.length() < 2) {
		valid = false;
		std::cout << "ERROR: first/last name must be 2 or more characters, each" << std::endl;
	}
	if (fname.length() > 20 || lname.length() > 20) {
		valid = false;
		std::cout << "ERROR: first/last name must be 20 or less characters, each" << std::endl;
	}

	// if first/last name have anything other than just alphabet letters, it is not vald. 
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
	std::cout << "WARNING: user instance has been deleted from program: " << firstName << " " << lastName << std::endl;
}

int User::getUserID() const { return userID; }

std::string User::getFirstName() const { return firstName; }

std::string User::getLastName() const { return lastName; }