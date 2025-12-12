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

// assume that manager is new to the system if "id2" is -1
// otherwise, assume that manager is being loaded from the system. 
BankManager::BankManager(const std::string& user, const std::string& pass, const std::string& fName, const std::string& lName, const int& id, const int& id2)
	: User(user, "Aa11", fName, lName, id)
{
	// if customerID is -1, Manager will be treated as a new manager and assigned a unique managerID
	managerID = id2;
	bool newManager = false;
	if (managerID == -1) {
		std::cout << "First login into the system for current manager. IDs will be assigned" << std::endl;
		newManager = true;
		managerID = getNewID();
	}

	// catch error if managerID was not given a unique ID for some reason. 
	if (managerID == -1) {
		throw std::runtime_error("managerID was not able to be assigned");
	}

	// execute if Manager is new
	if (newManager) {
		// if FALSE, manager ID was not updated in the file system
		if (!setManagerIDs(getUserID(), getManagerID(), user)) {
			throw std::runtime_error("IDs were not able to be updated on Manager file");
		}
	}

	// print after Manager has been loaded successfully. 
	std::cout << "manager (" << getFirstName() << ") has been loaded from file" << std::endl;
}

// use relative path to find file containing a unique manager ID
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

	// if unique ID was retrieved, a new unique ID is inserted into the file. 
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

	// return unique ID. 
	// return -1 if unique ID was not retrieved.
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

// *** DELETED: The entire implementation of BankManager::getUsername() was removed. ***
// *** DELETED: The entire implementation of BankManager::getPassword() was removed. ***
// The efficient versions are inherited from User::getUsername() and User::getPassword().

// *** FIX 1: Provide definition for viewAllCustomers ***
void BankManager::viewAllCustomers(const std::vector<User>& users) const
{
	// Placeholder implementation
	std::cout << "--- Manager View: All Customers ---" << std::endl;
	std::cout << "The implementation to display customers is not yet fully implemented." << std::endl;
	std::cout << "-----------------------------------" << std::endl;
}

// *** FIX 2: Provide definition for viewBankSystemInfo ***
void BankManager::viewBankSystemInfo() const
{
	// Placeholder implementation
	std::cout << "--- Manager View: System Info ---" << std::endl;
	std::cout << "The implementation to view bank statistics is not yet fully implemented." << std::endl;
	std::cout << "---------------------------------" << std::endl;
}


BankManager::~BankManager()
{
	std::cout << "WARNING: Manager instance removed from program: " << getFirstName() << " " << getLastName() << std::endl;
}

int BankManager::getManagerID() const { return managerID; }

void BankManager::viewAllCustomers(const std::vector<User>& users) const
{
	std::cout << "\n--- Viewing All Customers (Simulated) ---\n";
	std::cout << "TO DO: Implement logic to read and display data from customers.txt\n";
	std::cout << "Number of user objects passed: " << users.size() << std::endl;
}

// FIX 3: Definition for viewBankSystemInfo
void BankManager::viewBankSystemInfo() const
{
	std::cout << "\n--- Viewing Bank System Info (Simulated) ---\n";
	std::cout << "TO DO: Implement logic to read data from totalCustomers.txt and totalBankAccounts.txt\n";
}