#include "BankManager.h"

#include <string>
#include <vector>
#include <iostream>
#include "User.h"
#include "Customer.h"
#include "BankAccount.h"

BankManager::BankManager(const std::string& user, const std::string& pass, const std::string& fName, const std::string& lName, const int& id, const int& id2)
	: User(user, pass, fName, lName)
{

}

std::string BankManager::getUsername() const 
{
	return "me";
}

std::string BankManager::getPassword() const 
{
	return "password";
}

BankManager::~BankManager()
{
	std::cout << "WARNING: Manager instance removed from program: " << getFirstName() << " " << getLastName() << std::endl;
}

int BankManager::getManagerID() const { return managerID; }