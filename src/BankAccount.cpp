#include "BankAccount.h"
#include "Transaction.h"
#include <iostream>
#include <string>
#include <fstream>
#include <vector>

int BankAccount::totalBankAccounts = 0;

BankAccount::BankAccount(int cusID, BankAccountType type, double initBalance, int accID)
    :customerID(cusID), accountType(type), balance(initBalance)
{
    bool newAccount = false;

    if (accID == -1) {
        std::cout << "Creating new bank account. IDs will be assigned." << std::endl;
        newAccount = true;
        bankAccountID = getNewID();
    }
    else {
        bankAccountID = accID;
    }

    if (bankAccountID == -1) {
        throw std::runtime_error("bankAccountID was not able to be assigned");
    }

    if (newAccount) {
        std::ofstream accountFile("../../../data/bankAccounts/" + std::to_string(bankAccountID) + ".txt");
        if (accountFile.is_open()) {
            accountFile << bankAccountID << std::endl;
            accountFile << customerID << std::endl;
            accountFile << (accountType == BankAccountType::CHECKINGS ? "CHECKINGS" : "SAVINGS") << std::endl;
            accountFile << balance;
            accountFile.close();
            totalBankAccounts++;
            saveTotalBankAccounts();
        }
        else {
            throw std::runtime_error("Bank account file could not be created");
        }
    }

    std::cout << "Bank account (" << bankAccountID << ") has been loaded/created." << std::endl;
}

int BankAccount::getNewID() const {
    std::ifstream inputFile("../../../data/IDtrackers/bankAccountID.txt");
    int id = -1;

    if (inputFile.is_open()) {
        inputFile >> id;
        inputFile.close();
    }
    else {
        std::cout << "ERROR: unable to open \"bankAccountID.txt\" for READING" << std::endl;
    }

    if (id != -1) {
        std::ofstream outputFile("../../../data/IDtrackers/bankAccountID.txt");
        if (outputFile.is_open()) {
            outputFile << id + 1;
            outputFile.close();
        }
        else {
            std::cout << "ERROR: unable to open \"bankAccountID.txt\" for WRITING" << std::endl;
        }
    }

    return id;
}

BankAccount::~BankAccount() 
{
    std::cout << "WARNING: Bank account is being removed from program memory" << std::endl;
}

const std::vector<Transaction>& BankAccount::getTransactions() const {
    return transactions;
}

void BankAccount::deposit(double amount)
{
    // Checks if amount is less than or equal to 0, resulting in an error
    if (amount <= 0) {
        std::cout << "AMOUNT IS ZERO OR NEGATIVE" << std::endl;
    }
    else
    {
        balance += amount;
        transactions.emplace_back(customerID, Transaction::AccountTransactionType::DEPOSIT, amount, 0, balance, "time", -1);
        std::cout << "Deposited: $" << amount << std::endl << "New Balance: $" << balance << std::endl;
    }
    
}

void BankAccount::withdraw(double amount) {
    if (amount <= 0) {
        std::cout << "ERROR: Withdraw amount must be positive." << std::endl;
        return;
    }
    if (balance < amount) { //If Balance is lower than amount, throw error
       std::cout << "INSUFFICIENT BALANCE" << std::endl; 
    }
    else 
    {
        balance -= amount;
        std::cout << "Withdrawn: $" << amount << std::endl << "New Balance: $" << balance << std::endl;
        transactions.emplace_back(customerID, Transaction::AccountTransactionType::WITHDRAWAL, amount, 0, balance, "time",-1);
    }

}

void BankAccount::printAccountSummary() const {
    std::cout << "===============================" << std::endl;
    std::cout << "========Account Summary========" << std::endl;
    std::cout << "===============================" << std::endl;
    std::cout << "Account ID: " << bankAccountID << std::endl;
    std::cout << "Customer ID: " << customerID << std::endl;
    std::cout << "Current Balance: $" << balance << std::endl;
    std::cout << "------ Transaction History ------" << std::endl;

    if (transactions.empty()) {
        std::cout << "No transactions found." << std::endl;
    }
    else {
        for (const Transaction& t : transactions) {
            t.printTransaction();
        }
    }
}

void BankAccount::loadTotalBankAccounts()
{
    std::ifstream inputFile("../../../data/bankInfo/totalBankAccounts.txt");
    if (inputFile.is_open()) {
        inputFile >> totalBankAccounts;
        inputFile.close();
    }
    else {
        throw std::runtime_error("unable to load totalBankAccounts.txt");
    }
}

void BankAccount::saveTotalBankAccounts() {
    std::ofstream outputFile("../../../data/bankInfo/totalBankAccounts.txt");
    if (outputFile.is_open()) {
        outputFile << totalBankAccounts;
        outputFile.close();
    }
    else {
        throw std::runtime_error("Unable to save totalBankAccounts.txt");
    }
}

int BankAccount::getTotalBankAccounts() { return totalBankAccounts; }

int BankAccount::getBankAccountID() const { return bankAccountID; }

int BankAccount::getCustomerID() const { return customerID; }

BankAccount::BankAccountType BankAccount::getAccountType() const { return accountType; }

double BankAccount::getBalance() const { return balance; }