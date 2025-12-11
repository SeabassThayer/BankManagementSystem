#include "LoginManager.h"
#include <iostream>

LoginManager::LoginManager() {}

LoginManager::~LoginManager() {}

void LoginManager::run() 
{
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
            throw std::runtime_error("ERRORInvalid initial balance entered.");
        }

        // use enum to select the user-selected bank account type. 
        BankAccount::BankAccountType type = (accountTypeChoice == 1) ? BankAccount::BankAccountType::CHECKINGS: BankAccount::BankAccountType::SAVINGS;

        // Create new Bank Account
        BankAccount newAccount(newCustomer.getCustomerID(), type, initialBalance);

        std::cout << "Account successfully created for " << firstName << " " << lastName << "!\n";
        std::cout << "Customer ID: " << newCustomer.getCustomerID() << "\n";
        std::cout << "Bank Account ID: " << newAccount.getBankAccountID() << "\n";

    }
    catch (const std::runtime_error& e) {
        std::cerr << "ERROR: " << e.what() << std::endl;
    }
}

void LoginManager::handleManagerLogin()
{

}