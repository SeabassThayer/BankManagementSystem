#ifndef BANKMANAGER_H
#define BANKMANAGER_H

#include <string>
#include <vector>
#include "User.h"

class BankManager {
public:
    BankManager(const std::string& username, const std::string& password);
    
    bool login(const std::string& username, const std::string& password);
    void viewAllUsers(const std::vector<User>& users);
    void manageUserAccounts();
    
private:
    std::string username;
    std::string password;
};

#endif // BANKMANAGER_H