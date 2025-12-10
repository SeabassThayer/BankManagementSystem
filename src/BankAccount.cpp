#include "BankAccount.h"
#include "Transaction.h"
#include <iostream>
#include <string>
#include <fstream>

int BankAccount::totalBankAccounts = 0;

BankAccount::BankAccount(const std::string& accNumber, const std::string& holderName, double initialBalance) 
{
    


}
BankAccount::~BankAccount() {}

void BankAccount::deposit(double amount)
{
    // Checks if amount is less than or equal to 0, resulting in an error
    if (amount <= 0) {
        std::cout << "AMOUNT IS ZERO OR NEGATIVE" << std::endl;
    }
    else
    {
        balance += amount;
        std::cout << "Deposited: " << amount << std::endl << "New Balance: " << balance << std::endl;
        std::ofstream outputFile("userTransactions/" + "accountNumber" + ".txt", std::ofstream::app); //Opens the User's transaction file via account #
        outputFile << "Deposit of: $" << amount << "   Balance of: $" << balance << std::endl; //Stored Line for Transaction History
        outputFile.close();
    }
    
}

void BankAccount::withdraw(double amount) {
    if (balance < amount) { //If Balance is lower than amount, throw error
       std::cout << "INSUFFICIENT BALANCE" << std::endl; 
    }
    else 
    {
        balance -= amount;
        std::cout << "Withdrawn: " << amount << std::endl << "New Balance: " << balance << std::endl;
        std::ofstream outputFile("userTransactions/" + "accountNumber" + ".txt", std::ofstream::app); //Opens the User's transaction file via account #
        outputFile << "Withdraw of: $" << amount  << "  Balance of: $" << balance << std::endl; //Stored Line for Transaction History
        outputFile.close();
    }

}

void BankAccount::printAccountSummary() const {
    std::cout << "===============================" << std::endl;
    std::cout << "========Account Summary========" << std::endl;
    std::cout << "===============================" << std::endl;

    std::cout << "Account Number: " << "accountNumber" << std::endl;
    std::cout << "Current Balance: " << balance << std::endl;

    std::ifstream inputFile("userTransactions/" + "accountNumber" + ".txt");

    if (!inputFile.is_open()) {
        std::cout << "Error: Could not open transaction history." << std::endl;
        return;
    }

    std::cout << "------ Transaction History ------" << std::endl;

    std::string line;
    bool found = false;

    while (getline(inputFile, line)) { //Read every line in Transaction file and print
        std::cout << line << std::endl;
        found = true;
    }

    if (!found) {
        std::cout << "No transactions found for this user." << std::endl;
    }

    inputFile.close();
}
