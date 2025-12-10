#ifndef TRANSACTION_H
#define TRANSACTION_H

#include <string>

class Transaction {
private:
    enum class AccountTransactionType {
        DEPOSIT,
        WITHDRAWAL
    };

    int transactionID;
    int bankAccountID;

    AccountTransactionType transactionType;

    double transactionAmount;
    double oldBalance;
    double newBalance;

    std::string timestamp; // To store the time of the transaction

public:
    Transaction(const std::string& accountNumber, AccountTransactionType type, double amount);
    ~Transaction();

    int getTransactionID() const;
    int getBankAccountID() const;

    AccountTransactionType getType() const;

    double getTransactionAmount() const;
    double getOldBalance() const;
    double getNewBalance() const;

    std::string getTimestamp() const;

    void printTransaction() const;
};

#endif // TRANSACTION_H