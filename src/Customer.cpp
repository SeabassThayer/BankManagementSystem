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

// assume that the user is being created during run-time if "id2" is -1
// if "id2" is not -1, then the assumption is that you are loading a Customer from the file system. 
Customer::Customer(const std::string& user, const std::string& pass, const std::string& fName, const std::string& lName, const int& id, const int& id2)
	: User(user, "Aa11", fName, lName, id)
{
	// if customerID is -1, Customer will be treated as a new member and given a unique memberID
	customerID = id2;
	bool newCustomer = false;
	if (customerID == -1) {
		std::cout << "Welcome to Awesome Bank! IDs will be assigned" << std::endl;
		newCustomer = true;
		customerID = getNewID();
	}

	// catch error is customerID was not given a unique ID for some reason. 
	if (customerID == -1) {
		throw std::runtime_error("customerID was not able to be assigned");
	}

	// execute if Customer is new. 
	if (newCustomer) {
		// if FALSE, customer was not added to file
		// if TRUE, the last 2 lines are exeuted to update the total number of Customer in the system. 
		if (!createCustomerFile(user, pass)) {
			throw std::runtime_error("Customer file was not able to be created");
		}
		totalCustomers++;
		saveTotalCustomers();
	}

	// print after Customer has been created successfully. 
	std::cout << "customer (" << getFirstName() << ") has been loaded from file" << std::endl;
}

// attempt to add Customer to the file system 
// return true if success, otherwise, false. 
bool Customer::createCustomerFile(std::string user, std::string pass) const 
{
	bool success = true;

	// built a relative path & a name for the Customer file to be added. 
	std::string folderPath = "../../../data/customers";
	std::string targetFile = std::to_string(getCustomerID()) + ".txt";
	std::string fullPath = folderPath + "/" + targetFile;

	try {
		// if relative path and file are able to be created, the Customer info is inserted into the file. 
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

	// return true if nothing went wrong. 
	return success;
}

// return the accounts that are owned by the Customer
const std::vector<BankAccount>& Customer::getBankAccounts() const {
	return bankAccounts;
}

// use relative path to get total Customer data from a file. 
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

// use relative path to save total Customer data to a file. 
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

// use relative path to find file containing a unique Customer ID
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

	// if unique ID was retrieved, a new unique ID is inserted into the file. 
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

	// return unique ID. 
	// return -1 if uniqueID was not retrieved.
	return id;
}

// retrieve Username from Customer using Customers unique ID
std::string Customer::getUsername() const
{
	std::string user = "-1";
	std::string folderPath = "../../../data/customers";

	try {
		bool fileFound = false;

		// loop through the text files found inside of the "customers" folder
		for (const auto& customerFile : std::filesystem::directory_iterator(folderPath)) {
			// retrieve the name of the text file, which also happens to be the unique ID of the Customer inside that file. 
			std::string indexedCustomer = customerFile.path().stem().string();

			// check if file name matches the Customer ID of the Customer who called this function
			if (indexedCustomer == std::to_string(getCustomerID())) {

				// start reading data from inside the folder if filename and CustomerID match.
				// create string array that will temporarily hold data that is retrieved from the file. 
				std::ifstream customerInfo(customerFile.path());
				std::string* lines = new std::string[6];

				// loop through the file and store rows in the string array. 
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

				// retrieve the position where Username is found and save it to 'user'
				user = *(lines + 2);
				fileFound = true;

				// delete pointer to string array
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

	// return customer username.
	// return "-1" if customers username was not retrieved. 
	return user;
}


// retrieve Password from Customer using Customers unique ID
std::string Customer::getPassword() const
{
	std::string pass = "-1";
	std::string folderPath = "../../../data/customers";

	try {
		bool fileFound = false;

		// loop through the text files found inside of the "customers" folder
		for (const auto& customerFile : std::filesystem::directory_iterator(folderPath)) {
			// retrieve the name of the text file, which also happens to be the unique ID of the Customer inside that file. 
			std::string indexedCustomer = customerFile.path().stem().string();

			// check if file name matches the Customer ID of the Customer who called this function
			if (indexedCustomer == std::to_string(getCustomerID())) {

				// start reading data from inside the folder if filename and CustomerID match.
				// create string array that will temporarily hold data that is retrieved from the file. 
				std::ifstream customerInfo(customerFile.path());
				std::string* lines = new std::string[6];

				// loop through the file and store rows in the string array. 
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

				// retrieve the position where password is found and save it to 'pass'
				pass = *(lines + 3);
				fileFound = true;

				// delete pointer to string array
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

	// return customer password.
	// return "-1" if customers password was not retrieved. 
	return pass;
}

Customer::~Customer()
{
	std::cout << "WARNING: Customer instance removed from program: " << getFirstName() << " " << getLastName() << std::endl;
}

int Customer::getCustomerID() const { return customerID; }
