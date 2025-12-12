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
        default: std::cout << "Invalid option. Try again.\n"; break;
        }
    }
}

void LoginManager::showMainMenu()
{
    std::cout << "\nWelcome to Awesome Bank\n";
    std::cout << "1. User Login\n";
    std::cout << "2. Create Account\n";
    std::cout << "3. Manager Login\n";
    std::cout << "4. Exit\n";
    std::cout << "Choose an option: ";
}

// *** FIX 1: New consolidated login logic ***
void LoginManager::handleUserLogin()
{
    std::string username, password;
    std::cout << "\n--- User Login ---\n";
    std::cout << "Enter Username: ";
    std::cin >> username;
    std::cout << "Enter Password: ";
    std::cin >> password;

    int foundUserID = -1;
    int foundCustomerID = -1;
    std::unique_ptr<Customer> activeCustomer = nullptr;

    // NOTE: This assumes you have implemented Customer::findAndAuthenticate in Customer.cpp
    if (Customer::findAndAuthenticate(username, password, foundUserID, foundCustomerID)) {
        try {
            // Authentication successful. Now load the Customer object using the IDs found in customers.txt.
            // The Customer constructor must contain logic to load the rest of the user data (names, etc.) 
            // from customers.txt when both IDs are passed (id != -1 and id2 != -1).
            activeCustomer = std::make_unique<Customer>(
                username,
                password, // Use the verified password
                "LoadingFName", // Placeholder: Constructor should load the real name
                "LoadingLName", // Placeholder: Constructor should load the real name
                foundUserID,
                foundCustomerID
            );

            std::cout << "\n*** Login Successful! Welcome, " << activeCustomer->getFirstName() << " ***\n";
            customerMenu(*activeCustomer); // Start the customer session

        }
        catch (const std::runtime_error& e) {
            std::cerr << "\nERROR: Failed to load customer object after authentication: " << e.what() << std::endl;
        }
    }
    else {
        std::cout << "\n!!! Login Failed. Invalid username or password. !!!\n";
    }
}


void LoginManager::handleAccountCreation()
{
    std::string username, password, fName, lName;
    double initialDeposit;
    int accTypeChoice;

    std::cout << "\n--- New Account Creation ---\n";
    std::cout << "Enter First Name: ";
    std::cin >> fName;
    std::cout << "Enter Last Name: ";
    std::cin >> lName;
    std::cout << "Enter desired Username: ";
    std::cin >> username;
    std::cout << "Enter desired Password (4-12 chars, min 1 l/c, 1 u/c, 1 num): ";
    std::cin >> password;
    std::cout << "Select Account Type (1: CHECKINGS, 2: SAVINGS): ";
    std::cin >> accTypeChoice;
    std::cout << "Enter Initial Deposit Amount (> 0): $";
    std::cin >> initialDeposit;

    // Input validation for account type and deposit
    if (std::cin.fail() || (accTypeChoice != 1 && accTypeChoice != 2) || initialDeposit <= 0) {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cout << "\n!!! Invalid input for account type or deposit amount. Creation cancelled. !!!\n";
        return;
    }

    BankAccount::BankAccountType accType = (accTypeChoice == 1) ? BankAccount::BankAccountType::CHECKINGS : BankAccount::BankAccountType::SAVINGS;

    try {
        // 1. Create Customer: This constructor validates, assigns a unique ID (-1 means new ID), and saves the user/pass/info to file.
        // Note: The base User class will validate the password and throw an error if invalid.
        Customer newCustomer(username, password, fName, lName, -1, -1);

        // 2. Create Bank Account: This constructor links the account to the CustomerID, assigns a unique ID, and saves the account data to file.
        BankAccount newAccount(newCustomer.getCustomerID(), accType, initialDeposit, -1);

        std::cout << "\n*** Customer Account (" << newCustomer.getCustomerID()
            << ") and Bank Account (" << newAccount.getBankAccountID()
            << ") Created Successfully! ***\n";

    }
    catch (const std::runtime_error& e) {
        // Catch runtime errors from Customer/User/BankAccount creation (e.g., file error, validation error)
        std::cerr << "\n!!! Account creation failed: " << e.what() << " !!!\n";
    }
}


void LoginManager::handleManagerLogin()
{
    // NOTE: This function is still using the old, individual file system logic (fs::directory_iterator). 
    // It should be updated to use a consolidated BankManager::findAndAuthenticate function 
    // that scans a single file (e.g., ../../../data/managers.txt) next.

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
    // The current logic uses a hardcoded dummy account for operations. 
    // Next Step: We need to implement BankAccount::loadForCustomer(int customerID) 
    // to find the real account ID in bankAccounts.txt and load it.

    // Simulate loading the account (requires a proper BankAccount::load() function which is not present)
    // Using a fixed BankAccount ID (1) since we know the account creation worked for ID 1.
    // NOTE: This relies on the BankAccount constructor loading the data from bankAccounts.txt when accID != -1
    BankAccount account(customer.getCustomerID(), BankAccount::BankAccountType::CHECKINGS, 1000.00, 1); // Using ID 1

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

        switch (choice) {
        case 1: handleManagerOperations(manager); break;
        case 2:
            std::cout << "\n--- Access All User Data ---\n";
            std::cout << "As a manager, you have the capability to iterate through the 'data/customers/' and 'data/bankAccounts/' directories to view all stored user information and transaction history.\n";
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