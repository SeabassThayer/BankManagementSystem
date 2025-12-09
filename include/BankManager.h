#ifndef BANKMANAGER_H
#define BANKMANAGER_H

#include <string>
#include <vector>
#include "User.h"
#include "Customer.h"
#include "BankAccount.h"

// Uses derived class, User, to fill in for basic manager information. 
class BankManager : public User {
private:
    // unique identifier
    int managerID;

public:
    //TO-DO:id cannot be -1, because we are not allowed to create new managers
    BankManager(const std::string& user, const std::string& pass,const std::string& fName, const std::string& lName, const int& id = -1, const int& id2 = -1);
    ~BankManager();

    std::string getUsername() const override;
    std::string getPassword() const override;

    int getManagerID() const;
    
    void viewAllCustomers(const std::vector<User>& users) const;
    Customer findCustomerAccount() const;
    BankAccount findBankAccount() const;
    //bool login(const std::string& username, const std::string& password);
    //void manageUserAccounts();

    void resetIDs() const; //temporary - delete before submission
    void eraseAllUsers() const; // temporary - delete before submission
private:
    int getNewID() const;
};

#endif // BANKMANAGER_H