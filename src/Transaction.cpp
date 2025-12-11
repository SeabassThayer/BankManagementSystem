#include "Transaction.h"
#include <string>
#include <iostream>

Transaction::Transaction(int bankAccID, AccountTransactionType type, double amount, double oldB, double newB, std::string time, int transID)
	:bankAccountID(bankAccID), transactionType(type), transactionAmount(amount), oldBalance(oldB), newBalance(newB), timestamp(time)
{
}

Transaction::~Transaction()
{
	std::cout << "WARNING: tranaction record deleted from program memory" << std::endl;
}

int Transaction::getTransactionID() const { return transactionID; }

int Transaction::getBankAccountID() const { return bankAccountID; }

std::string Transaction::getType() const 
{ 
	if (transactionType == AccountTransactionType::DEPOSIT) {
		return "Deposit";
	}
	else {
		return "Withdrawal";
	}
}

double Transaction::getTransactionAmount() const { return transactionAmount; }

double Transaction::getOldBalance() const { return oldBalance; }

double Transaction::getNewBalance() const { return newBalance; }

std::string Transaction::getTimestamp() const { return timestamp; }

void Transaction::printTransaction() const {
	std::cout << "Transaction ID: " << getTransactionID() << std::endl;
	std::cout << "Bank Account ID: " << getBankAccountID() << std::endl;
	std::cout << "Transaction Tyoe: " << getType() << std::endl;
	std::cout << "Transaction Amount: " << getTransactionAmount() << std::endl;
	std::cout << "Previous Balance: " << getOldBalance() << std::endl;
	std::cout << "New Balance: " << getNewBalance() << std::endl;
	std::cout << "Time of Transaction : " << getTimestamp() << std::endl;
}