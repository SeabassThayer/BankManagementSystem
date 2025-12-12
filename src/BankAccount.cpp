#include "BankAccount.h"
#include "Transaction.h"
#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <filesystem>

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
    else {
        loadTransactions();
    }
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

BankAccount::~BankAccount() { }

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
        double old = balance;
        balance += amount;
        transactions.emplace_back(bankAccountID, Transaction::AccountTransactionType::DEPOSIT, amount, old, balance, "N/A", -1);
        saveToFile();
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
        double old = balance;
        balance -= amount;
        std::cout << "Withdrawn: $" << amount << std::endl << "New Balance: $" << balance << std::endl;
        transactions.emplace_back(bankAccountID, Transaction::AccountTransactionType::WITHDRAWAL, amount, old, balance, "N/A",-1);
        saveToFile();
    }
}

void BankAccount::saveToFile() const {
    std::ofstream accountFile("../../../data/bankAccounts/" + std::to_string(bankAccountID) + ".txt");

    if (!accountFile.is_open()) {
        throw std::runtime_error("ERROR: Could not save bank account file.");
    }

    accountFile << bankAccountID << std::endl;
    accountFile << customerID << std::endl;
    accountFile << (accountType == BankAccountType::CHECKINGS ? "CHECKINGS" : "SAVINGS") << std::endl;
    accountFile << balance;

    accountFile.close();
}

void BankAccount::loadTransactions() {
    transactions.clear();
    std::string folderPath = "../../../data/transactions/";

    // loop through all the transactions in system
    for (const auto& file : std::filesystem::directory_iterator(folderPath)) {

        if (!file.is_regular_file()) continue;

        std::ifstream transFile(file.path());
        if (!transFile.is_open()) {
            std::cout << "ERROR: Unable to open transaction file: " << file.path().string() << std::endl;
            continue;
        }

        int tID;
        int accID;
        std::string accType;
        double amount, oldB, newB;
        std::string timestamp;

        transFile >> tID;
        transFile >> accID;

        // This transaction belongs to another account
        if (accID != bankAccountID) {
            transFile.close();
            continue;
        }

        transFile >> accType;
        transFile >> amount;
        transFile >> oldB;
        transFile >> newB;

        transFile.ignore();
        std::getline(transFile, timestamp);

        transFile.close();

        // Convert string to enum
        Transaction::AccountTransactionType type =(accType == "Deposit" ? Transaction::AccountTransactionType::DEPOSIT : Transaction::AccountTransactionType::WITHDRAWAL);

        // Load transaction
        transactions.emplace_back(bankAccountID, type, amount, oldB, newB, timestamp, tID);
    }
}

BankAccount BankAccount::loadAccount(int customerID)
{
    std::string folderPath = "../../../data/bankAccounts/";

    // loop through the bank accounts on file.
    for (const auto& file : std::filesystem::directory_iterator(folderPath))
    {
        if (!file.is_regular_file()) continue;

        std::ifstream accountFile(file.path());
        if (!accountFile.is_open()) continue;

        int accID;
        int fileCustomerID;
        std::string accType;
        double balance;

        // Read data in the same order as saved
        accountFile >> accID;
        accountFile >> fileCustomerID;

        if (fileCustomerID != customerID) {
            accountFile.close();
            continue;
        }

        accountFile >> accType;
        accountFile >> balance;
        accountFile.close();

        BankAccountType type = (accType == "CHECKINGS" ? BankAccountType::CHECKINGS : BankAccountType::SAVINGS);

        // Construct account 
        BankAccount acc(fileCustomerID, type, balance, accID);

        // Load existing transactions for this account
        acc.loadTransactions();

        return acc;
    }

    throw std::runtime_error("No bank account found for customerID: " + std::to_string(customerID));
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
            std::cout << "\n";
            t.printTransaction();
            std::cout << "\n";
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