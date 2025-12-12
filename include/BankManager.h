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
    BankManager(const std::string& user, const std::string& pass,const std::string& fName, const std::string& lName, const int& id = -1, const int& id2 = -1);
    ~BankManager();

    // override base class function with custom getters for username and password
    std::string getUsername() const override;

    std::string getPassword() const override;

    int getManagerID() const;
    
    // retrieve all customers in the file system to show their data. 
    void viewAllCustomers(const std::vector<Customer>& customers) const;

private:
    // return a unique Manager ID to whoever called the function. 
    int getNewID() const override;

    // assign unique and permanent ID to managers within the file system. 
    bool setManagerIDs(int userid, int managerid, std::string user) const;
};

#endif // BANKMANAGER_H