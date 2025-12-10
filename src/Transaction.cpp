#include "Transaction.h"
#include <string>
#include <iostream>

Transaction::Transaction(int bankAccID, AccountTransactionType type, double amount, double oldB, double newB, std::string time, int transID)
	:bankAccountID(bankAccID), transactionType(type), transactionAmount(amount), oldBalance(oldB), newBalance(newB), timestamp(time)
{
	
}

Transaction::~Transaction()
{
	std::cout << "BYE!" << std::endl;
}

/*std::string Transaction::getAccountNumber() const
{
	return accountNumber;
}
Transaction::TransactionType Transaction::getType() const
{
	return type;
}
double Transaction::getAmount() const
{
	return amount;
}
std::string Transaction :: getTimestamp() const
{
	return timestamp;
}*/

void Transaction::printTransaction() const {
	std::cout << "HELLO!" << std::endl;
}