#include "BankAccount.h"
#include "Transaction.h"
#include <iostream>
#include <string>
#include <fstream>
#include <vector>

int BankAccount::totalBankAccounts = 0;

// if accID is -1,we assume Bank Account is being created at run-time
// if accID is not -1, we assume Bank Account is being retrieved from file system. 
BankAccount::BankAccount(int cusID, BankAccountType type, double initBalance, int accID)
    :customerID(cusID), accountType(type), balance(initBalance)
{
    // if accID = -1, a new unique ID will be created and assigned. 
    // and account will be marked as new during constructor execution. 
    bool newAccount = false;
    if (accID == -1) {
        std::cout << "Creating new bank account. IDs will be assigned." << std::endl;
        newAccount = true;
        bankAccountID = getNewID();
    }
    else {
        bankAccountID = accID;
    }

    // catch invalid Bank Account ID if it slipped previous check. 
    if (bankAccountID == -1) {
        throw std::runtime_error("bankAccountID was not able to be assigned");
    }

    // execute if Bank Account is marked as new.
    if (newAccount) {
        // use relative path and unique ID to make a file where the data for this 
        // Bank Account will be stored to use between sessions. 
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

// retrieve new unique Bank Account ID using relative paths
int BankAccount::getNewID() const {
    std::ifstream inputFile("../../../data/IDtrackers/bankAccountID.txt");
    int id = -1;

    // retrieve unique ID from file. 
    if (inputFile.is_open()) {
        inputFile >> id;
        inputFile.close();
    }
    else {
        std::cout << "ERROR: unable to open \"bankAccountID.txt\" for READING" << std::endl;
    }

    // if unique ID was retrieved from file, new unique ID is inserted into the file
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

// return all transactions saved by Class instance on it's transactions variable. 
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
    // perform basic validity checks before allowing Customer to withdraw money. 
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
        transactions.emplace_back(customerID, Transaction::AccountTransactionType::WITHDRAWAL, amount, 0, balance, "time unspecified",-1);
    }

}

// print account summary plus all transactions, if any. 
void BankAccount::printAccountSummary() const {
    std::cout << "===============================" << std::endl;
    std::cout << "========Account Summary========" << std::endl;
    std::cout << "===============================" << std::endl;
    std::cout << "Account ID: " << bankAccountID << std::endl;
    std::cout << "Customer ID: " << customerID << std::endl;
    std::cout << "Current Balance: $" << balance << std::endl;
    std::cout << "------ Transaction History ------" << std::endl;

    // checking if bank account has any transactions
    if (transactions.empty()) {
        std::cout << "No transactions found." << std::endl;
    }
    else {
        for (const Transaction& t : transactions) {
            t.printTransaction();
        }
    }
}

// use relative path to get total bank account data from file.
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

// use relative path to save total bank account data to file. 
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

                            // getters //

int BankAccount::getTotalBankAccounts() { return totalBankAccounts; }

int BankAccount::getBankAccountID() const { return bankAccountID; }

int BankAccount::getCustomerID() const { return customerID; }

BankAccount::BankAccountType BankAccount::getAccountType() const { return accountType; }

double BankAccount::getBalance() const { return balance; }