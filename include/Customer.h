#ifndef CUSTOMER_H
#define CUSTOMER_H

#include <string>
#include <vector>
class BankAccount;
#include "User.h"

// Uses derived class, User, to fill in for basic customer information. 
class Customer : public User {
private:
    //unique identifier
    int customerID;

    // current number of customers in the system. 
    static int totalCustomers;

    // stores all the bank accounts that belong to the Customer.
    std::vector<BankAccount> bankAccounts;

public:
    // constructs a Customer with default parameters that assume the Customer is new to the system. 
    Customer(const std::string& user, const std::string& pass, const std::string& fName, const std::string& lName, const int& id = -1, const int& id2 = -1);

    // attempts to save Customer data to the system (Now appends to customers.txt)
    bool createCustomerFile() const;

    ~Customer();

    // FIX 1: Corrected signature to take 6 arguments
    static bool findAndAuthenticate(const std::string& user, const std::string& pass,
        int& foundUserID, int& foundCustomerID,
        std::string& foundFName, std::string& foundLName);

    // returns the bank accounts that belong to the Customer. 
    const std::vector<BankAccount>& getBankAccounts() const;

    // return current amount of Customers based on curent Class variable. 
    static int getTotalCustomers();

    // load amount of Customers from the file system. 
    static void loadTotalCustomers();

    // save amount of Customers to the file system. 
    static void saveTotalCustomers();

    int getCustomerID() const;

    // FIX 2: Removed 'override'
    std::string getUsername() const;

    // FIX 2: Removed 'override'
    std::string getPassword() const;

private:
    // return a unique Customer ID to whoever called the function. 
    int getNewID() const override;
};

#endif  //CUSTOMER_H