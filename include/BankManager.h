#ifndef BANKMANAGER_H
#define BANKMANAGER_H

#include "User.h"
#include "Customer.h"
#include "BankAccount.h"

#include <string>
#include <vector>

// Uses derived class, User, to fill in for basic manager information. 
class BankManager : public User {
private:
    // unique identifier
    int managerID;

public:
    // constructs a Manager with default parameters that assume the Manager is new to the system & logging in for the first time. 
    BankManager(const std::string& user, const std::string& pass, const std::string& fName, const std::string& lName, const int& id = -1, const int& id2 = -1);
    ~BankManager();

    // *** DELETED: getUsername() and getPassword() overrides. ***
    // They are now inherited from the base User class.

    int getManagerID() const;

    // TO DO: retrieve all customers in the file system to show their data. 
    void viewAllCustomers(const std::vector<User>& users) const;

    // TO DO: get the total amount of bank accounts and the total amount of customers. 
    void viewBankSystemInfo() const;

    //Customer findCustomerAccount(std::string& customerID) const; // TO-DO if time allows
    //BankAccount findBankAccount(std::string& bankAccountID) const; // TO-DO if time allows

private:
    // return a unique Manager ID to whoever called the function. 
    int getNewID() const override;

    // assign unique and permanent ID to managers within the file system. 
    bool setManagerIDs(int userid, int managerid, std::string user) const;
};

#endif // BANKMANAGER_H