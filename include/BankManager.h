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
    BankManager(const std::string& user, const std::string& pass,const std::string& fName, const std::string& lName, const int& id = -1, const int& id2 = -1);
    ~BankManager();

    std::string getUsername() const override;
    std::string getPassword() const override;

    int getManagerID() const;
    
    void viewAllCustomers(const std::vector<User>& users) const;
    Customer findCustomerAccount(std::string& customerID) const;
    BankAccount findBankAccount(std::string& bankAccountID) const;

private:
    int getNewID() const override;
    bool setManagerIDs(int userid, int managerid, std::string user) const;
};

#endif // BANKMANAGER_H