#ifndef USER_H
#define USER_H

#include <string>

class User {
public:
    User(const std::string& username, const std::string& password);
    
    std::string getUsername() const;
    bool validatePassword(const std::string& password) const;

    // Additional methods for managing user accounts can be declared here

private:
    std::string username;
    std::string password;
    // Additional attributes related to user accounts can be added here
};

#endif // USER_H