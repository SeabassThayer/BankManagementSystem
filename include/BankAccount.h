#ifndef BANKACCOUNT_H
#define BANKACCOUNT_H

#include "User.h"
#include "Customer.h"
#include "Transaction.h"

#include <string>
#include <vector>

// simulate and store a basic bank account
class BankAccount {
public:
    // declare the possible account types with enum
    // rather than using strings
    enum class BankAccountType {
        CHECKINGS,
        SAVINGS
    };

private:
    // unique identifier
    int bankAccountID;

    // customer ID of account owner. 
    int customerID;

    // type of account based on enum class.
    BankAccountType accountType;

    // current balance in account.
    double balance;

    std::vector<Transaction> transactions;

    //update bank information in the system.
    void saveToFile() const;

    //load transaction information from the system
    void loadTransactions();

    // current number of bank accounts in the system. 
    // saved in file system to keep track even if program terminates. 
    static int totalBankAccounts;

public:
    // constructs a Bank Account with the default assumption that it is being created rather than loaded from file System
    BankAccount(int cusID, BankAccountType type, double initBalance, int accID = -1);

    ~BankAccount();

    // setters and gettes //
    int getBankAccountID() const;

    int getCustomerID() const;

    BankAccountType getAccountType() const;

    double getBalance() const;
   
    // modify the balance of the account // 
    void deposit(double amount);

    void withdraw(double amount);

    // output an overview of the account excluding data irrelevant to the Customer.
    // also excludes passwords for safety.
    void printAccountSummary() const;
    
    // return all the Transactions that have been recorded for this bank account
    const std::vector<Transaction>& getTransactions() const;

    // return current amount of bank accounts based on Class variable.
    static int getTotalBankAccounts();

    // load current amount of bank accounts from the file system.
    static void loadTotalBankAccounts();

    // save current amount of bank accounts to the file system. 
    static void saveTotalBankAccounts();

    // return bank account instance given a customerID (assumes 1 customer has 1 account each)
    static BankAccount loadAccount(int customerID);

private:
    // return a unique bank account ID to whoever called the function.
    int getNewID() const;
};

#endif // BANKACCOUNT_H