#ifndef TRANSACTION_H
#define TRANSACTION_H

#include <string>

// Stores information about any transaction done within the system
class Transaction {
public:
    // declare the possible account transaction types with enum
    // rather than using strings
    enum class AccountTransactionType {
        DEPOSIT,
        WITHDRAWAL
    };

private:
    // unique identifier
    int transactionID;

    // account id of the person who performed the transaction
    int bankAccountID;

    AccountTransactionType transactionType;

    // amount withdraw/deposited from account
    double transactionAmount;

    // account balance before the transaction
    double oldBalance;

    // account balance after the transaction
    double newBalance;

    std::string timestamp; // To store the time of the transaction

public:
    // Constructs a transaction assuming that all the transaction data is already kown
    // if transID is -1, it is a new transaction.
    // if trandID is not -1, the transaction is being loaded from file. 
    Transaction(int bankAccID, AccountTransactionType type, double amount, double oldB, double newB, std::string time, int transID = -1);
    ~Transaction();

    // getter and setters //

    int getTransactionID() const;

    int getBankAccountID() const;

    std::string getType() const;

    double getTransactionAmount() const;

    double getOldBalance() const;

    double getNewBalance() const;

    std::string getTimestamp() const;

    // outputs all the information about the transaction
    void printTransaction() const;

private:
    // return a unique transaction ID to whoever called the function. 
    int getNewID() const;
};

#endif // TRANSACTION_H