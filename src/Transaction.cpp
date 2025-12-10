#include "Transaction.h"
#include <string>

Transaction::Transaction(const std::string& accountNumber, TransactionType type, double amount) : accountNumber(accountNumber), type(type), amount(amount)
{
	

}

std::string Transaction::getAccountNumber() const
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
}

