#include "Customer.h"

#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <stdexcept>
#include <filesystem>
#include "User.h"
#include "BankAccount.h"

int Customer::totalCustomers = 0;

Customer::Customer(const std::string& user, const std::string& pass, const std::string& fName, const std::string& lName, const int& id, const int& id2)
	: User(user, "Aa11", fName, lName, id)
{
	customerID = id2;
	bool newCustomer = false;

	if (customerID == -1) {
		std::cout << "Welcome to Awesome Bank! IDs will be assigned" << std::endl;
		newCustomer = true;
		customerID = getNewID();
	}

	if (customerID == -1) {
		throw std::runtime_error("customerID was not able to be assigned");
	}

	if (newCustomer) {
		if (!createCustomerFile(user, pass)) {
			throw std::runtime_error("Customer file was not able to be created");
		}
		totalCustomers++;
		saveTotalCustomers();
	}

	std::cout << "customer (" << getFirstName() << ") has been loaded from file" << std::endl;
}

bool Customer::createCustomerFile(std::string user, std::string pass) const 
{
	bool success = true;
	std::string folderPath = "../../../data/customers";
	std::string targetFile = std::to_string(getCustomerID()) + ".txt";
	std::string fullPath = folderPath + "/" + targetFile;

	try {
		std::ofstream newCustomerFile(fullPath);
		if (newCustomerFile.is_open()) {
			newCustomerFile << getUserID() << std::endl;
			newCustomerFile << getCustomerID() << std::endl;
			newCustomerFile << user << std::endl;
			newCustomerFile << pass << std::endl;
			newCustomerFile << getFirstName() << std::endl;
			newCustomerFile << getLastName() << std::endl;

			std::cout << "Customer file created:" << fullPath << std::endl;
			newCustomerFile.close();
		}
		else {
			throw std::runtime_error("Unable to create customer file: " + fullPath);
		}
	}
	catch (const std::runtime_error& e) {
		success = false;
		std::cerr << "ERROR: " << e.what() << std::endl;
	}

	return success;
}

//const std::vector<BankAccount>& Customer::getBankAccounts() const {
//	return bankAccounts;
//}


void Customer::loadTotalCustomers() {
	std::ifstream inputFile("../../../data/bankInfo/totalCustomers.txt");
	if (inputFile.is_open()) {
		inputFile >> totalCustomers;
		inputFile.close();
	} 
	else {
		throw std::runtime_error("unable to load totalCustomers.txt");
	}
}

void Customer::saveTotalCustomers() {
	std::ofstream inputFile("../../../data/bankInfo/totalCustomers.txt");
	if (inputFile.is_open()) {
		inputFile << totalCustomers;
		inputFile.close();
	}
	else {
		throw std::runtime_error("Unable to save totalCustomers.txt");
	}
}

int Customer::getTotalCustomers() { return totalCustomers; }

int Customer::getNewID() const
{
	std::ifstream inputFile("../../../data/IDtrackers/customerID.txt");
	int id = -1;

	if (inputFile.is_open()) {
		inputFile >> id;
		inputFile.close();
	}
	else {
		std::cout << "ERROR: unable to open \"customerID\" for READING" << std::endl;
	}

	if (id != -1) {
		std::ofstream outputFile("../../../data/IDtrackers/customerID.txt");
		if (outputFile.is_open()) {
			outputFile << id + 1;
			outputFile.close();
		}
		else {
			std::cout << "ERROR: unable to open \"customerID\" for WRITING" << std::endl;
		}
	}

	return id;
}

std::string Customer::getUsername() const
{
	std::string user = "-1";
	std::string folderPath = "../../../data/customers";

	try {
		bool fileFound = false;
		for (const auto& customerFile : std::filesystem::directory_iterator(folderPath)) {
			std::string indexedCustomer = customerFile.path().stem().string();

			if (indexedCustomer == std::to_string(getCustomerID())) {
				std::ifstream customerInfo(customerFile.path());
				std::string* lines = new std::string[6];

				if (customerInfo.is_open()) {
					std::string line;
					int i = 0;
					while (std::getline(customerInfo, line)) {
						*(lines + i) = line;
						i++;
					}
					customerInfo.close();
				}
				else {
					delete[] lines;
					throw std::runtime_error(indexedCustomer + " was not able to be opened for reading");
				}

				user = *(lines + 2);
				fileFound = true;
				delete[] lines;
				break;
			}
		}
		if (!fileFound) {
			throw std::runtime_error("customer file was not found");
		}
	}
	catch (const std::runtime_error& e) {
		std::cerr << "ERROR: " << e.what() << std::endl;
	}

	return user;
}

std::string Customer::getPassword() const
{
	std::string pass = "-1";
	std::string folderPath = "../../../data/customers";

	try {
		bool fileFound = false;
		for (const auto& customerFile : std::filesystem::directory_iterator(folderPath)) {
			std::string indexedCustomer = customerFile.path().stem().string();

			if (indexedCustomer == std::to_string(getCustomerID())) {
				std::ifstream customerInfo(customerFile.path());
				std::string* lines = new std::string[6];

				if (customerInfo.is_open()) {
					std::string line;
					int i = 0;
					while (std::getline(customerInfo, line)) {
						*(lines + i) = line;
						i++;
					}
					customerInfo.close();
				}
				else {
					delete[] lines;
					throw std::runtime_error(indexedCustomer + " was not able to be opened for reading");
				}

				pass = *(lines + 3);
				fileFound = true;
				delete[] lines;
				break;
			}
		}
		if (!fileFound) {
			throw std::runtime_error("customer file was not found");
		}
	}
	catch (const std::runtime_error& e) {
		std::cerr << "ERROR: " << e.what() << std::endl;
	}

	return pass;
}

Customer::~Customer()
{
	std::cout << "WARNING: Customer instance removed from program: " << getFirstName() << " " << getLastName() << std::endl;
}

int Customer::getCustomerID() const { return customerID; }
