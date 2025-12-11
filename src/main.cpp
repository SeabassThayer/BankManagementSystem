#include "BankAccount.h"
#include "BankManager.h"
#include "LoginManager.h"
#include "Transaction.h"
#include "User.h"
#include "Customer.h"

#include <string>
#include <iostream>
#include <stdexcept>

int main() 
{
	try {
		Customer::loadTotalCustomers();
		BankAccount::loadTotalBankAccounts();
		Customer me("SunnyLord", "SUNNY20lord", "sunny", "lord", 114, 2);
		std::cout << "user id: " << me.getUserID() << std::endl;
		std::cout << "customer id: " << me.getCustomerID() << std::endl;
		std::cout << "username: " << me.getUsername() << std::endl;
		std::cout << "password: " << me.getPassword() << std::endl;
		std::cout << "first name: " << me.getFirstName() << std::endl;
		std::cout << "last name: " << me.getLastName() << std::endl;
		std::cout << "total customers: " << me.getTotalCustomers() << std::endl;

		BankAccount sav1(2, BankAccount::BankAccountType::CHECKINGS, 256, 2);

		Transaction tran1(0, Transaction::AccountTransactionType::DEPOSIT, 50, 5, 55, "big ol time", -1);
	}
	catch (const std::runtime_error& e) {
		std::cerr << "ERROR: could not create user: " << e.what() << std::endl;
	}

	return 0;
}