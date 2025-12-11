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

void LoginManager::handleUserLogin() 
{

}

void LoginManager::handleAccountCreation() 
{

}

void LoginManager::handleManagerLogin()
{

}