#include "BankManager.h"
#include "User.h"
#include "Customer.h"
#include "BankAccount.h"

#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <filesystem>

BankManager::BankManager(const std::string& user, const std::string& pass, const std::string& fName, const std::string& lName, const int& id, const int& id2)
	: User(user, "Aa11", fName, lName, id)
{
	managerID = id2;
	bool newManager = false;
	if (managerID == -1) {
		std::cout << "First login into the system for current manager. IDs will be assigned" << std::endl;
		newManager = true;
		managerID = getNewID();
	}

	if (managerID == -1) {
		throw std::runtime_error("managerID was not able to be assigned");
	}

	if (newManager) {
		if (!setManagerIDs(getUserID(), getManagerID(), user)) {
			throw std::runtime_error("IDs were not able to be updated on Manager file");
		}
	}

	std::cout << "manager (" << getFirstName() << ") has been loaded from file" << std::endl;
}

int BankManager::getNewID() const
{
	std::ifstream inputFile("../../../data/IDtrackers/managerID.txt");
	int id = -1;

	if (inputFile.is_open()) {
		inputFile >> id;
		inputFile.close();
	}
	else {
		std::cout << "ERROR: unable to open \"managerID\" for READING" << std::endl;
	}

	if (id != -1) {
		std::ofstream outputFile("../../../data/IDtrackers/managerID.txt");
		if (outputFile.is_open()) {
			outputFile << id + 1;
			outputFile.close();
		} 
		else {
			std::cout << "ERROR: unable to open \"managerID\" for WRITING" << std::endl;
		}
	}

	return id;
}

bool BankManager::setManagerIDs(int userid, int managerid, std::string user) const
{
	bool success = true;
	std::string folderPath = "../../../data/managers";
	std::string targetFile = user + ".txt";

	try {
		bool fileFound = false;

		for (const auto& managerFile : std::filesystem::directory_iterator(folderPath)) {
			std::string indexedManager = managerFile.path().filename().string();

			if (indexedManager == targetFile) {
				fileFound = true;
				std::cout << "Setting IDs in file: " << indexedManager << std::endl;

				std::ifstream managerInfo(managerFile.path());
				std::string* lines = new std::string[6];

				if (managerInfo.is_open()) {
					std::string line;
					int i = 0;
					while (std::getline(managerInfo, line)) {
						*(lines + i) = line;
						i++;
					}
					managerInfo.close();
				}
				else {
					delete[] lines;
					throw std::runtime_error(indexedManager + " was not able to be opened for reading");
				}

				*(lines) = std::to_string(userid);
				*(lines + 1) = std::to_string(managerid);

				std::ofstream newManagerInfo(managerFile.path());
				if (newManagerInfo.is_open()) {
					for (int i = 0; i < 6; i++) {
						newManagerInfo << *(lines + i) << std::endl;
					}
					newManagerInfo.close();
				}
				else {
					delete[] lines;
					throw std::runtime_error(indexedManager + " was not able to be opened for writing");
				}

				delete[] lines;
			}
		}

		if (!fileFound) {
			throw std::runtime_error("manager file was not found");
		}
	}
	catch (const std::runtime_error& e) {
		success = false;
		std::cerr << "ERROR: " << e.what() << std::endl;
	}

	return success;
}

std::string BankManager::getUsername() const 
{
	std::string user = "-1";
	std::string folderPath = "../../../data/managers";

	try {
		bool fileFound = false;
		for (const auto& managerFile : std::filesystem::directory_iterator(folderPath)) {
			std::string indexedManager = managerFile.path().filename().string();

			std::ifstream managerInfo(managerFile.path());
			std::string* lines = new std::string[6];

			if (managerInfo.is_open()) {
				std::string line;
				int i = 0;
				while (std::getline(managerInfo, line)) {
					*(lines + i) = line;
					i++;
				}
				managerInfo.close();
			}
			else {
				delete[] lines;
				throw std::runtime_error(indexedManager + " was not able to be opened for reading");
			}

			if (std::to_string(getManagerID()) == *(lines + 1)) {
				user = *(lines + 2);
				fileFound = true;
				delete[] lines;
				break;
			}

			delete[] lines;
		}
		if (!fileFound) {
			throw std::runtime_error("manager file was not found");
		}
	}
	catch (const std::runtime_error& e) {
		std::cerr << "ERROR: " << e.what() << std::endl;
	}

	return user;
}

std::string BankManager::getPassword() const 
{
	std::string pass = "-1";
	std::string folderPath = "../../../data/managers";

	try {
		bool fileFound = false;
		for (const auto& managerFile : std::filesystem::directory_iterator(folderPath)) {
			std::string indexedManager = managerFile.path().filename().string();

			std::ifstream managerInfo(managerFile.path());
			std::string* lines = new std::string[6];

			if (managerInfo.is_open()) {
				std::string line;
				int i = 0;
				while (std::getline(managerInfo, line)) {
					*(lines + i) = line;
					i++;
				}
				managerInfo.close();
			}
			else {
				delete[] lines;
				throw std::runtime_error(indexedManager + " was not able to be opened for reading");
			}

			if (std::to_string(getManagerID()) == *(lines + 1)) {
				pass = *(lines + 3);
				fileFound = true;
				delete[] lines;
				break;
			}

			delete[] lines;
		}
		if (!fileFound) {
			throw std::runtime_error("manager file was not found");
		}
	}
	catch (const std::runtime_error& e) {
		std::cerr << "ERROR: " << e.what() << std::endl;
	}

	return pass;
}

BankManager::~BankManager()
{
	std::cout << "WARNING: Manager instance removed from program: " << getFirstName() << " " << getLastName() << std::endl;
}

int BankManager::getManagerID() const { return managerID; }