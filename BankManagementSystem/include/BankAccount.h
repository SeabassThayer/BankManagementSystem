#ifndef BANKACCOUNT_H
#define BANKACCOUNT_H

#include <string>

class BankAccount {
private:
    std::string accountNumber;
    std::string accountHolderName;
    double balance;
    static int activeAccounts;

public:
    BankAccount(const std::string& accNumber, const std::string& holderName, double initialBalance);
    ~BankAccount();

    void deposit(double amount);
    bool withdraw(double amount);
    void printAccountSummary() const;

    static int getActiveAccounts();
};

#endif // BANKACCOUNT_H