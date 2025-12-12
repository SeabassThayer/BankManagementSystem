#include "LoginManager.h"
#include "Customer.h"
#include "BankManager.h"
#include "BankAccount.h"
#include <iostream>
#include <limits>
#include <string>
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <filesystem>
#include <memory> // Required for smart pointers

namespace fs = std::filesystem;

// --- Private Helper Functions for Session Management ---
// These functions are defined inside LoginManager.cpp to handle the post-login flow.
void customerMenu(Customer& customer);
void managerMenu(BankManager& manager);
void handleBankOperations(Customer& customer);
void handleManagerOperations(BankManager& manager);


//LoginManager class implementation
LoginManager::LoginManager() {}

LoginManager::~LoginManager() {}

void LoginManager::run() 
{
    try {
        // Loads totalCustomers from totalCustomers.txt
        Customer::loadTotalCustomers();

        // Loads totalBankAccounts from totalBankAccounts.txt
        BankAccount::loadTotalBankAccounts();
    }
    catch (const std::runtime_error& e) {
        // This catch block handles the error if the files are missing 
        // (e.g., if it's the very first time running the program)
        std::cerr << "Warning: Could not load bank system metrics: " << e.what() << ". Starting counters at 0.\n";
    }

    bool running = true;
    while (running) {
        showMainMenu();
        int choice;
        std::cin >> choice;

        // handles user input that is not a number between 1 and 4
        if (std::cin.fail()) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "Invalid input. Please enter a number between 1 and 4.\n";
            continue;
        }

        switch (choice) {
            case 1: handleUserLogin(); break;
            case 2: handleAccountCreation(); break;
            case 3: handleManagerLogin(); break;
            case 4: running = false; break;
            default: std::cout << "ERROR: Invalid option. Try again.\n"; break;
        }
    }
}

void LoginManager::showMainMenu() 
{
    std::cout << "\n\n\nWelcome to Awesome Bank\n";
    std::cout << "1. User Login\n";
    std::cout << "2. Create Account\n";
    std::cout << "3. Manager Login\n";
    std::cout << "4. Exit\n";
    std::cout << "Choose an option: ";
}

void LoginManager::handleUserLogin() 
{
    std::string username, password;
    std::cout << "\n--- User Login ---\n";
    std::cout << "Enter Username: ";
    std::cin >> username;
    std::cout << "Enter Password: ";
    std::cin >> password;

    std::unique_ptr<Customer> activeCustomer = nullptr;
    std::string folderPath = "../../../data/customers";

    try {
        // Iterate through all customer files to check credentials (using file name as ID)
        for (const auto& entry : fs::directory_iterator(folderPath)) {
            if (entry.is_regular_file() && entry.path().extension() == ".txt") {
                std::ifstream file(entry.path());

                // Read the lines to get UserID, CustomerID, Username, Password
                std::string line, fileUserID, fileCustomerID, fileUsername, filePassword, fileFName, fileLName;

                // Read exactly 6 lines for the file structure defined in Customer.cpp
                if (std::getline(file, fileUserID) &&
                    std::getline(file, fileCustomerID) &&
                    std::getline(file, fileUsername) &&
                    std::getline(file, filePassword) &&
                    std::getline(file, fileFName) &&
                    std::getline(file, fileLName))
                {
                    // Check if credentials match
                    if (fileUsername == username && filePassword == password) {
                        int userID = std::stoi(fileUserID);
                        int customerID = std::stoi(fileCustomerID);

                        // Create a Customer object from the loaded file data.
                        // The 'id' (userID) and 'id2' (customerID) are passed to load the existing user.
                        activeCustomer = std::make_unique<Customer>(
                            fileUsername,
                            "Aa11", // Placeholder, password validity checked on creation, but base class expects a valid one
                            fileFName,
                            fileLName,
                            userID,
                            customerID
                        );
                        break;
                    }
                }
            }
        }
    }
    catch (const fs::filesystem_error& e) {
        std::cerr << "Filesystem Error during user login: " << e.what() << std::endl;
    }
    catch (const std::exception& e) {
        std::cerr << "Error during customer file processing: " << e.what() << std::endl;
    }

    if (activeCustomer) {
        std::cout << "\n*** Login Successful! Welcome, " << activeCustomer->getFirstName() << " ***\n";
        customerMenu(*activeCustomer); // Start the customer session
    }
    else {
        std::cout << "\n!!! Login Failed. Invalid username or password. !!!\n";
    }
}


void LoginManager::handleAccountCreation() 
{
    try {
        std::string username, password, firstName, lastName;
        int accountTypeChoice;
        double initialBalance;

        std::cout << "\n\n\nCreate New Account\n";

        // Ask for User data
        std::cout << "Enter username: ";
        std::cin >> username;
        std::cout << "Enter password: ";
        std::cin >> password;
        std::cout << "Enter first name: ";
        std::cin >> firstName;
        std::cout << "Enter last name: ";
        std::cin >> lastName;

        // Create new Customer (id = -1, id2 = -1 signals new user)
        Customer newCustomer(username, password, firstName, lastName, -1, -1);

        // Ask for account type & ensure user input is either 1 or 2
        std::cout << "Choose account type (1.Checkings, 2.Savings): ";
        std::cin >> accountTypeChoice;
        if (std::cin.fail() || (accountTypeChoice != 1 && accountTypeChoice != 2)) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            throw std::runtime_error("Invalid account type selected.");
        }

        // Ask for initial bank account balance
        // validate to ensure user inputs number bigger than 0
        std::cout << "Enter initial deposit amount: ";
        std::cin >> initialBalance;
        if (std::cin.fail() || initialBalance < 0) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            throw std::runtime_error("ERROR: Invalid initial balance entered.");
        }

        // use enum to select the user-selected bank account type. 
        BankAccount::BankAccountType type = (accountTypeChoice == 1) ? BankAccount::BankAccountType::CHECKINGS : BankAccount::BankAccountType::SAVINGS;

        // Create new Bank Account
        BankAccount newAccount(newCustomer.getCustomerID(), type, initialBalance);

        std::cout << "\nAccount successfully created for " << firstName << " " << lastName << "!\n";
        std::cout << "Customer ID: " << newCustomer.getCustomerID() << "\n";
        std::cout << "Bank Account ID: " << newAccount.getBankAccountID() << "\n";

    }
    catch (const std::runtime_error& e) {
        std::cerr << "ERROR: " << e.what() << std::endl;
    }
}


void LoginManager::handleManagerLogin()
{
    std::string username, password;
    std::cout << "\n--- Manager Login ---\n";
    std::cout << "Enter Manager Username: ";
    std::cin >> username;
    std::cout << "Enter Manager Password: ";
    std::cin >> password;

    std::unique_ptr<BankManager> activeManager = nullptr;
    std::string folderPath = "../../../data/managers";

    try {
        // Iterate through all manager files to check credentials (manager files are named by username)
        for (const auto& entry : fs::directory_iterator(folderPath)) {
            if (entry.is_regular_file() && entry.path().extension() == ".txt") {
                std::ifstream file(entry.path());

                // Read the lines to get UserID, ManagerID, Username, Password, FName, LName
                std::string line, fileUserID, fileManagerID, fileUsername, filePassword, fileFName, fileLName;

                // Read exactly 6 lines for the file structure defined in BankManager.cpp
                if (std::getline(file, fileUserID) &&
                    std::getline(file, fileManagerID) &&
                    std::getline(file, fileUsername) &&
                    std::getline(file, filePassword) &&
                    std::getline(file, fileFName) &&
                    std::getline(file, fileLName))
                {
                    // Check if credentials match
                    if (fileUsername == username && filePassword == password) {
                        int userID = std::stoi(fileUserID);
                        int managerID = std::stoi(fileManagerID);

                        // Create a BankManager object from the loaded file data.
                        activeManager = std::make_unique<BankManager>(
                            fileUsername,
                            "Aa11",
                            fileFName,
                            fileLName,
                            userID,
                            managerID
                        );
                        break;
                    }
                }
            }
        }
    }
    catch (const fs::filesystem_error& e) {
        std::cerr << "Filesystem Error during manager login: " << e.what() << std::endl;
    }
    catch (const std::exception& e) {
        std::cerr << "Error during manager file processing: " << e.what() << std::endl;
    }

    if (activeManager) {
        std::cout << "\n*** Login Successful! Welcome, Manager " << activeManager->getFirstName() << " ***\n";
        managerMenu(*activeManager); // Start the manager session
    }
    else {
        std::cout << "\n!!! Login Failed. Invalid manager username or password. !!!\n";
    }
}

void customerMenu(Customer& customer) {
    bool loggedIn = true;
    while (loggedIn) {
        std::cout << "\n--- Customer Menu: " << customer.getFirstName() << " ---\n";
        std::cout << "1. Deposit\n";
        std::cout << "2. Withdraw\n";
        std::cout << "3. Print Account Summary\n";
        std::cout << "4. Logout\n";
        std::cout << "Choose an option: ";

        int choice;
        std::cin >> choice;

        if (std::cin.fail()) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "Invalid input. Please enter a number.\n";
            continue;
        }

        switch (choice) {
        case 1:
        case 2:
        case 3:
            handleBankOperations(customer);
            break;
        case 4:
            loggedIn = false;
            // unique_ptr will be automatically destroyed when it goes out of scope in handleUserLogin()
            std::cout << "\nLogged out successfully.\n";
            break;
        default: std::cout << "Invalid option. Try again.\n"; break;
        }
    }
}

void handleBankOperations(Customer& customer) {
    // A complete implementation would load the BankAccount object(s) associated with customer.getCustomerID() 
    // from the ../../../data/bankAccounts/ folder. For this implementation, we will simulate the process.

    // 1. Find and load the customer's bank account (assuming only one for simplicity).
    std::string bankAccountFilePath = "../../../data/bankAccounts/" + std::to_string(customer.getCustomerID()) + ".txt";
    // NOTE: This assumes the customerID and BankAccountID are the same, which is a simplification. 
    // In a real system, the customer file would list account IDs. We will use a dummy ID (1001) for the demo.

    BankAccount account = BankAccount::loadAccount(customer.getCustomerID());
    int choice;
    double amount;
    std::cout << "\n--- Bank Operations for Account " << account.getBankAccountID() << " ---\n";
    std::cout << "1. Deposit | 2. Withdraw | 3. Summary: ";
    std::cin >> choice;

    if (std::cin.fail()) {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cout << "\n!!! Invalid input. Returning to menu. !!!\n";
        return;
    }

    try {
        switch (choice) {
        case 1:
            std::cout << "Enter amount to Deposit: $";
            std::cin >> amount;
            account.deposit(amount); // Calls the function in BankAccount.cpp
            break;
        case 2:
            std::cout << "Enter amount to Withdraw: $";
            std::cin >> amount;
            account.withdraw(amount); // Calls the function in BankAccount.cpp (checks for insufficient funds)
            break;
        case 3:
            account.printAccountSummary(); // Calls the function in BankAccount.cpp (prints balance and transactions)
            break;
        default:
            std::cout << "Invalid operation choice.\n";
        }
    }
    catch (const std::exception& e) {
        std::cerr << "Operation error: " << e.what() << std::endl;
    }
}

void managerMenu(BankManager& manager) {
    bool loggedIn = true;
    while (loggedIn) {
        std::cout << "\n--- Manager Menu: " << manager.getFirstName() << " ---\n";
        std::cout << "1. View Bank System Metrics\n";
        std::cout << "2. Access All User Data (Simulated)\n";
        std::cout << "3. Logout\n";
        std::cout << "Choose an option: ";

        int choice;
        std::cin >> choice;

        if (std::cin.fail()) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "Invalid input. Please enter a number.\n";
            continue;
        }

        std::vector<Customer> allCustomers = Customer::loadAllCustomers();
        switch (choice) {
        case 1: handleManagerOperations(manager); break;
        case 2:
            manager.viewAllCustomers(allCustomers);
            break;
        case 3:
            loggedIn = false;
            std::cout << "\nLogged out successfully.\n";
            break;
        default: std::cout << "Invalid option. Try again.\n"; break;
        }
    }
}

void handleManagerOperations(BankManager& manager) {
    std::cout << "\n--- Bank System Metrics ---\n";

    // Attempt to load and display static data members (total accounts/customers)
    try {
        BankAccount::loadTotalBankAccounts(); // Loads the static counter
        Customer::loadTotalCustomers(); // Loads the static counter
        std::cout << "Total Active Bank Accounts: " << BankAccount::getTotalBankAccounts() << std::endl;
        std::cout << "Total Customers: " << Customer::getTotalCustomers() << std::endl;
    }
    catch (const std::runtime_error& e) {
        std::cerr << "Could not retrieve total bank info: " << e.what() << std::endl;
        std::cerr << "Make sure the directory structure (e.g., ../../../data/bankInfo/) and files (totalBankAccounts.txt, totalCustomers.txt) exist.\n";
    }
}