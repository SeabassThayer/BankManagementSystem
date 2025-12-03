#ifndef TRANSACTION_H
#define TRANSACTION_H

#include <string>

class Transaction {
public:
    enum class TransactionType {
        DEPOSIT,
        WITHDRAWAL
    };

    Transaction(const std::string& accountNumber, TransactionType type, double amount);
    
    std::string getAccountNumber() const;
    TransactionType getType() const;
    double getAmount() const;
    std::string getTimestamp() const;

private:
    std::string accountNumber;
    TransactionType type;
    double amount;
    std::string timestamp; // To store the time of the transaction
};

#endif // TRANSACTION_H