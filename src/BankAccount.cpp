#include "BankAccount.h"
#include "Transaction.h"
#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <sstream>
#include <stdexcept>
#include <filesystem> // New header for directory creation

int BankAccount::totalBankAccounts = 0;

// if accID is -1,we assume Bank Account is being created at run-time
// if accID is not -1, we assume Bank Account is being retrieved from file system. 
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

    // execute if Bank Account is marked as new.
    if (newAccount) {
        std::string fullPath = "../../../data/bankAccounts.txt";
        std::string dirPath = "../../../data/"; // Parent directory for bankAccounts.txt

        try {
            // *** FIX: Ensure the parent directory exists ***
            std::filesystem::path dir(dirPath);
            if (!std::filesystem::exists(dir)) {
                std::filesystem::create_directories(dir);
            }

            // Open file in APPEND mode. This creates the file if it doesn't exist.
            std::ofstream bankAccountFile(fullPath, std::ios::app);

            if (bankAccountFile.is_open()) {
                // Format: BankAccountID|CustomerID|AccountType(int)|Balance
                bankAccountFile << bankAccountID << "|"
                    << customerID << "|"
                    << static_cast<int>(accountType) << "|"
                    << balance << std::endl;

                std::cout << "Bank account record appended to: " << fullPath << std::endl;
                bankAccountFile.close();

                totalBankAccounts++;
                saveTotalBankAccounts();
            }
            else {
                throw std::runtime_error("Unable to open bankAccounts.txt for appending.");
            }
        }
        catch (const std::exception& e) {
            std::cerr << "ERROR during account creation persistence: " << e.what() << std::endl;
            throw std::runtime_error("Account creation failed: bankAccountID was not able to be assigned");
        }
    }

    std::cout << "Bank account (" << bankAccountID << ") has been loaded/created." << std::endl;
}


// New getNewID() implementation that scans bankAccounts.txt
int BankAccount::getNewID() const
{
    int max_id = 1;
    std::ifstream inputFile("../../../data/bankAccounts.txt");

    if (inputFile.is_open()) {
        std::string line;
        while (std::getline(inputFile, line)) {
            std::stringstream ss(line);
            std::string segment;

            if (std::getline(ss, segment, '|')) {
                try {
                    int current_id = std::stoi(segment);
                    if (current_id >= max_id) {
                        max_id = current_id + 1;
                    }
                }
                catch (const std::exception& e) {
                    std::cerr << "Warning: Error parsing BankAccountID in bankAccounts.txt. " << e.what() << std::endl;
                }
            }
        }
        inputFile.close();
    }

    return max_id;
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
        double oldB = balance;
        balance += amount;
        transactions.emplace_back(customerID, Transaction::AccountTransactionType::DEPOSIT, amount, oldB, balance, "time", -1);
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
        double oldB = balance;
        balance -= amount;
        std::cout << "Withdrawn: $" << amount << std::endl << "New Balance: $" << balance << std::endl;
        transactions.emplace_back(customerID, Transaction::AccountTransactionType::WITHDRAWAL, amount, oldB, balance, "time unspecified", -1);
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
    std::string dirPath = "../../../data/bankInfo/"; // Parent directory for the tracker file

    try {
        // *** FIX: Ensure the 'bankInfo' parent directory exists ***
        std::filesystem::path dir(dirPath);
        if (!std::filesystem::exists(dir)) {
            std::filesystem::create_directories(dir);
        }

        std::ofstream outputFile(dirPath + "totalBankAccounts.txt");
        if (outputFile.is_open()) {
            outputFile << totalBankAccounts;
            outputFile.close();
        }
        else {
            throw std::runtime_error("Unable to save totalBankAccounts.txt: File stream failed to open.");
        }
    }
    catch (const std::exception& e) {
        // This catches errors from filesystem functions too
        std::cerr << "ERROR during saveTotalBankAccounts: " << e.what() << std::endl;
        throw std::runtime_error("Unable to save totalBankAccounts.txt: Directory or file creation failed.");
    }
}

// getters //

int BankAccount::getTotalBankAccounts() { return totalBankAccounts; }

int BankAccount::getBankAccountID() const { return bankAccountID; }

int BankAccount::getCustomerID() const { return customerID; }

BankAccount::BankAccountType BankAccount::getAccountType() const { return accountType; }

double BankAccount::getBalance() const { return balance; }