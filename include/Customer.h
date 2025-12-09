#ifndef CUSTOMER_H
#define CUSTOMER_H

#include <string>
#include <vector>
#include "BankAccount.h"

class Customer : public User {
private:
    static int totalCustomers;
    std::vector<BankAccount>& bankAccounts;
    int customerID;

public:
    Customer(const std::string& user, const std::string& pass,const std::string& fName, const std::string& lName, const int& id = -1, const int& id2 = -1);
    ~Customer();

    int getCustomerID() const;
    std::vector<BankAccount>& getBankAccounts();

    std::string getUsername() const override;
    std::string getPassword() const override;

private:
    int getNewID() const;
};

#endif  //CUSTOMER_H