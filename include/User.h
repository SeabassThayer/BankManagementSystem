#ifndef USER_H
#define USER_H

#include <string>

class User {
private:
    std::string username;
    std::string password;
    // Additional attributes related to user accounts can be added here

public:
    User(const std::string& username, const std::string& password);
    
    std::string getUsername() const;
    bool validatePassword(const std::string& password) const;

    // Additional methods for managing user accounts can be declared here
};

#endif // USER_H