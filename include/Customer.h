#ifndef CUSTOMER_H
#define CUSTOMER_H

#include <string>
#include <vector>
#include "BankAccount.h"
#include "User.h"

// Uses derived class, User, to fill in for basic customer information. 
class Customer : public User {
private:
    //unique identifier
    int customerID;
    static int totalCustomers;
    //std::vector<BankAccount>& bankAccounts;

public:
    Customer(const std::string& user, const std::string& pass,const std::string& fName, const std::string& lName, const int& id = -1, const int& id2 = -1);
    bool createCustomerFile(std::string user, std::string pass) const;
    ~Customer();

    std::vector<BankAccount>& getBankAccounts();

    static int getTotalCustomers();
    static void loadTotalCustomers();
    static void saveTotalCustomers();
    
    int getCustomerID() const;
    std::string getUsername() const override;
    std::string getPassword() const override;

private:
    int getNewID() const override;
};

#endif  //CUSTOMER_H