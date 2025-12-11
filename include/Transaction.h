#ifndef TRANSACTION_H
#define TRANSACTION_H

#include <string>

class Transaction {
public:
    enum class AccountTransactionType {
        DEPOSIT,
        WITHDRAWAL
    };

private:
    int transactionID;
    int bankAccountID;

    AccountTransactionType transactionType;

    double transactionAmount;
    double oldBalance;
    double newBalance;

    std::string timestamp; // To store the time of the transaction

public:
    Transaction(int bankAccID, AccountTransactionType type, double amount, double oldB, double newB, std::string time, int transID = -1);
    ~Transaction();

    int getTransactionID() const;
    int getBankAccountID() const;

    std::string getType() const;
    double getTransactionAmount() const;
    double getOldBalance() const;
    double getNewBalance() const;
    std::string getTimestamp() const;

    void printTransaction() const;

private:
    int getNewID() const;
};

#endif // TRANSACTION_H