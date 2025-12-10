#ifndef BANKACCOUNT_H
#define BANKACCOUNT_H

#include "User.h"
#include "Customer.h"
#include "Transaction.h"

#include <string>
#include <vector>

class BankAccount {
public:
    enum class BankAccountType {
        CHECKINGS,
        SAVINGS
    };

private:
    int bankAccountID;
    int customerID;
    BankAccountType accountType;
    double balance;

    std::vector<Transaction> transactions;

    static int totalBankAccounts;

public:
    BankAccount(int cusID, BankAccountType type, double initBalance,int accID = -1);
    ~BankAccount();

    int getBankAccountID() const;
    int getCustomerID() const;
    BankAccountType getAccountType() const;
    double getBalance() const;
    
    void deposit(double amount);
    void withdraw(double amount);
    void printAccountSummary() const;
    
    const std::vector<Transaction>& getTransactions() const;

    static int getTotalBankAccounts();
    static void loadTotalBankAccounts();
    static void saveTotalBankAccounts();

private:
    int getNewID() const;
};

#endif // BANKACCOUNT_H