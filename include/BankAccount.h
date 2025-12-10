#ifndef BANKACCOUNT_H
#define BANKACCOUNT_H

#include "User.h"
#include "Customer.h"
#include "Transaction.h"

#include <string>
#include <vector>

class BankAccount {
private:
    enum class BankAccountType {
        CHECKINGS,
        SAVINGS
    };

    int bankAccountID;
    int customerID;

    BankAccountType accountType;
    double balance;

    //std::vector<Transaction>& transactions;

    static int totalBankAccounts;

public:
    BankAccount(const std::string& accNumber, const std::string& holderName, double initialBalance);
    ~BankAccount();

    int getBankAccountID() const;
    BankAccountType getAccountType() const;
    double getBalance() const;
    std::string getCustomerID() const;

    void deposit(double amount);
    void withdraw(double amount);
    void printAccountSummary() const;

    static int getTotalBankAccounts();
    static void loadTotalBankAccounts();
    static void saveTotalBankAccounts();
};

#endif // BANKACCOUNT_H