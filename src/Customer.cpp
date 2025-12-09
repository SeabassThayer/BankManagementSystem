#include "Customer.h"

#include <string>
#include <vector>
#include <iostream>
#include "User.h"
#include "BankAccount.h"

int Customer::totalCustomers = 0;

Customer::Customer(const std::string& user, const std::string& pass, const std::string& fName, const std::string& lName, const int& id, const int& id2)
	: User(user, pass, fName, lName)
{

}

std::string Customer::getUsername() const
{
	return "me";
}

std::string Customer::getPassword() const
{
	return "password";
}

Customer::~Customer()
{
	std::cout << "WARNING: Customer instance removed from program: " << getFirstName() << " " << getLastName() << std::endl;
}

int Customer::getCustomerID() const { return customerID; }