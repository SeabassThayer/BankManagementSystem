#include "User.h"
#include <iostream>
#include <fstream>
#include <stdexcept>

User::User(const std::string& user, const std::string& pass, const std::string& fName, const std::string& lName) 
	:username(user), password(pass), firstName(fName), lastName(lName)
{
	userID = getNewID();

	if (userID == -1) {
		throw std::runtime_error("userID was was not able to be assigned");
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

User::~User() 
{
	std::cout << "user info has been deleted from program" << std::endl;
}

int User::getUserID() const { return userID; }

std::string User::getUsername() const { return username; }

std::string User::getPassword() const { return password; }

std::string User::getFirstName() const { return firstName; }

std::string User::getLastName() const { return lastName; }