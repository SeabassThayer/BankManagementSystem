#include "Transaction.h"
#include <string>
#include <iostream>
#include <fstream>

Transaction::Transaction(int bankAccID, AccountTransactionType type, double amount, double oldB, double newB, std::string time, int transID)
	:bankAccountID(bankAccID), transactionType(type), transactionAmount(amount), oldBalance(oldB), newBalance(newB), timestamp(time)
{
	transactionID = transID;
	bool newTransaction = false;
	if (transactionID == -1) {
		std::cout << "Transaction is being created and saved to the system" << std::endl;
		newTransaction = true;
		transactionID = getNewID();
	}

	if (transactionID == -1) {
		throw std::runtime_error("transactionID was not able to be assigned");
	}

	if (newTransaction) {
		std::ofstream transactionFile("../../../data/" + std::to_string(transactionID) + ".txt");
		if (transactionFile.is_open()) {
			transactionFile << transactionID << std::endl;
			transactionFile << bankAccountID << std::endl;
			transactionFile << getType() << std::endl;
			transactionFile << transactionAmount << std::endl;
			transactionFile << oldBalance << std::endl;
			transactionFile << newBalance << std::endl;
			transactionFile << getTimestamp() << std::endl;
			transactionFile.close();
		}
	}
	else {
		throw std::runtime_error("Transaction file was not created");
	}

	std::cout << "Transaction File was generated!" << std::endl;
}

int Transaction::getNewID() const
{
	std::ifstream inputFile("../../../data/IDtrackers/transactionID.txt");
	int id = -1;

	if (inputFile.is_open()) {
		inputFile >> id;
		inputFile.close();
	}
	else {
		std::cout << "ERROR: unable to open \"transactionID\" for READING" << std::endl;
	}

	if (id != -1) {
		std::ofstream outputFile("../../../data/IDtrackers/transactionID.txt");
		if (outputFile.is_open()) {
			outputFile << id + 1;
			outputFile.close();
		}
		else {
			std::cout << "ERROR: unable to open \"transactionID\" for WRITING" << std::endl;
		}
	}

	return id;
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