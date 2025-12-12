#include "Customer.h"

#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>      // Required for file parsing
#include <stdexcept>
#include <filesystem>
#include "User.h"
#include "BankAccount.h"

int Customer::totalCustomers = 0;

// The loadCustomerData helper is removed as we now retrieve all necessary data (including names) 
// during the static findAndAuthenticate call in LoginManager.

// assume that the user is being created during run-time if "id2" is -1
// if "id2" is not -1, then the assumption is that you are loading a Customer from the file system
// (which means the User part of the object is being constructed with the real data retrieved during login).
Customer::Customer(const std::string& user, const std::string& pass, const std::string& fName, const std::string& lName, const int& id, const int& id2)
// The User constructor is called with the actual names, username, password, and IDs.
    : User(user, pass, fName, lName, id)
{
    customerID = id2;
    bool newCustomer = false;

    if (customerID == -1) {
        std::cout << "Welcome to Awesome Bank! IDs will be assigned" << std::endl;
        newCustomer = true;
        customerID = getNewID();
    }
    // No 'else' case needed. For existing customers, the correct data is already passed and stored
    // in the User base class by the LoginManager::handleUserLogin function.

    // catch error if customerID was not given a unique ID for some reason. 
    if (customerID == -1) {
        throw std::runtime_error("customerID was not able to be assigned");
    }

    // execute if Customer is new. 
    if (newCustomer) {
        // Call the updated persistence function to append to customers.txt
        if (!createCustomerFile()) {
            throw std::runtime_error("Customer file was not able to be created");
        }
        totalCustomers++;
        saveTotalCustomers();
        std::cout << "new user has been created!" << std::endl;
    }

    std::cout << "customer (" << getFirstName() << ") has been loaded/created." << std::endl;
}

// *** FIXED: Implementation of the static authentication function with name retrieval ***
bool Customer::findAndAuthenticate(const std::string& user, const std::string& pass,
    int& foundUserID, int& foundCustomerID,
    std::string& foundFName, std::string& foundLName)
{
    std::ifstream inputFile("../../../data/customers.txt");
    if (!inputFile.is_open()) {
        std::cerr << "ERROR: Cannot open customers.txt for authentication." << std::endl;
        return false;
    }

    std::string line;
    while (std::getline(inputFile, line)) {
        // Format: UserID|CustomerID|Username|Password|FirstName|LastName
        std::stringstream ss(line);
        std::string segment;
        std::vector<std::string> fields;

        while (std::getline(ss, segment, '|')) {
            fields.push_back(segment);
        }

        if (fields.size() >= 6) {
            std::string fileUsername = fields[2];
            std::string filePassword = fields[3];

            if (fileUsername == user && filePassword == pass) {
                try {
                    foundUserID = std::stoi(fields[0]);
                    foundCustomerID = std::stoi(fields[1]);
                    // Extract the names for use in the Customer object constructor
                    foundFName = fields[4];
                    foundLName = fields[5];

                    inputFile.close();
                    return true;
                }
                catch (const std::exception& e) {
                    std::cerr << "Warning: Error parsing IDs/data in customers.txt during login: " << e.what() << std::endl;
                }
            }
        }
    }

    inputFile.close();
    return false;
}

// *** FIXED: Update createCustomerFile to append to customers.txt (consolidated) ***
bool Customer::createCustomerFile() const
{
    bool success = true;
    std::string fullPath = "../../../data/customers.txt";

    try {
        // Open file in APPEND mode.
        std::ofstream newCustomerFile(fullPath, std::ios::app);
        if (newCustomerFile.is_open()) {
            // Format: UserID|CustomerID|Username|Password|FirstName|LastName
            newCustomerFile << getUserID() << "|"
                << getCustomerID() << "|"
                << getUsername() << "|"
                << getPassword() << "|"
                << getFirstName() << "|"
                << getLastName() << std::endl;

            std::cout << "Customer record appended to: " << fullPath << std::endl;
            newCustomerFile.close();
        }
        else {
            throw std::runtime_error("Unable to open customers.txt for appending: " + fullPath);
        }
    }
    catch (const std::runtime_error& e) {
        success = false;
        std::cerr << "ERROR: " << e.what() << std::endl;
    }

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
    // Ensure the parent directory exists (optional, but good practice)
    if (!std::filesystem::exists("../../../data/bankInfo/")) {
        std::filesystem::create_directories("../../../data/bankInfo/");
    }
    std::ofstream outputFile("../../../data/bankInfo/totalCustomers.txt");
    if (outputFile.is_open()) {
        outputFile << totalCustomers;
        outputFile.close();
    }
    else {
        throw std::runtime_error("Unable to save totalCustomers.txt");
    }
}

int Customer::getTotalCustomers() { return totalCustomers; }

// *** FIXED: New getNewID() implementation that scans customers.txt ***
int Customer::getNewID() const
{
    int max_id = 1;
    std::ifstream inputFile("../../../data/customers.txt");

    if (inputFile.is_open()) {
        std::string line;
        while (std::getline(inputFile, line)) {
            // Format: UserID|CustomerID|Username|...
            std::stringstream ss(line);
            std::string segment;
            std::string customerIDSegment;

            // Extract the first segment (UserID) then the second segment (CustomerID) separated by '|'
            if (std::getline(ss, segment, '|') && std::getline(ss, customerIDSegment, '|')) {
                try {
                    int current_id = std::stoi(customerIDSegment);
                    if (current_id >= max_id) {
                        max_id = current_id + 1;
                    }
                }
                catch (const std::exception& e) {
                    std::cerr << "Warning: Error parsing CustomerID in customers.txt during new ID generation. " << e.what() << std::endl;
                }
            }
        }
        inputFile.close();
    }
    return max_id;
}


// *** FIXED: Simplified Getters rely on the User base class ***
std::string Customer::getUsername() const
{
    return User::getUsername();
}


std::string Customer::getPassword() const
{
    return User::getPassword();
}

Customer::~Customer()
{
    std::cout << "WARNING: Customer instance removed from program: " << getFirstName() << " " << getLastName() << std::endl;
}

int Customer::getCustomerID() const { return customerID; }