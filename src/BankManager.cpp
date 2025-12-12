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

// retrieve username for Manager using their unique ID
std::string BankManager::getUsername() const 
{
	std::string user = "-1";
	std::string folderPath = "../../../data/managers";

	try {
		bool fileFound = false;

		// loop through the text files found inside of the "managers" folder
		for (const auto& managerFile : std::filesystem::directory_iterator(folderPath)) {
			// retrieve the name of the text file
			std::string indexedManager = managerFile.path().filename().string();

			// open file and initialize string array to input file rows
			std::ifstream managerInfo(managerFile.path());
			std::string* lines = new std::string[6];

			// loop through the file and store rows in the string array. 
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

			// check if manager ID in file matches the file of the calling Manager. 
			if (std::to_string(getManagerID()) == *(lines + 1)) {
				// if match, retrieve the username from the file and remove the string array. 
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

	// return "-1" if username was not found.
	return user;
}

// retrieve Password for Manager using their unique ID
std::string BankManager::getPassword() const 
{
	std::string pass = "-1";
	std::string folderPath = "../../../data/managers";

	try {
		bool fileFound = false;

		// loop through the text files found inside of the "managers" folder
		for (const auto& managerFile : std::filesystem::directory_iterator(folderPath)) {
			// retrieve the name of the text file
			std::string indexedManager = managerFile.path().filename().string();

			// open file and initialize string array to input file rows
			std::ifstream managerInfo(managerFile.path());
			std::string* lines = new std::string[6];

			// loop through the file and store rows in the string array. 
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

			// check if manager ID in file matches the file of the calling Manager. 
			if (std::to_string(getManagerID()) == *(lines + 1)) {
				// if match, retrieve the password from the file and remove the string array. 
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

	// return "-1" if password was not found. 
	return pass;
}

void BankManager::viewAllCustomers(const std::vector<Customer>& customers) const
{
	if (customers.empty()) {
		std::cout << "\nNo customers found in the system.\n";
		return;
	}

	std::cout << "\n============ ALL CUSTOMERS ===========\n";

	for (const Customer& c : customers) {
		std::cout << "\nCustomer ID: " << c.getCustomerID() << "\n";
		std::cout << "Name: " << c.getFirstName() << " " << c.getLastName() << "\n";
		std::cout << "Username: " << c.getUsername() << "\n";

		// load and display account
		BankAccount account = BankAccount::loadAccount(c.getCustomerID());

		std::cout << "Bank Account ID: " << account.getBankAccountID() << "\n";
		std::cout << "Account Type: " << (account.getAccountType() == BankAccount::BankAccountType::CHECKINGS ? "Checkings" : "Savings") << "\n";
		std::cout << "Balance: $" << account.getBalance() << "\n";

		std::cout << "--------------------------------------\n";
	}

	std::cout << "\n============ END CUSTOMER LIST ==========\n";
}

BankManager::~BankManager() { }

int BankManager::getManagerID() const { return managerID; }