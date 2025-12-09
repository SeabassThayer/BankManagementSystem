#ifndef USER_H
#define USER_H

#include <string>

class User {
private:
    int userID;
    std::string username;
    std::string password;
    std::string firstName;
    std::string lastName;

public:
    User(const std::string& user, const std::string& pass, const std::string& fName, const std::string& lName);
    ~User();
    int getUserID() const;
    std::string getUsername() const;
    std::string getPassword() const;
    std::string getFirstName() const;
    std::string getLastName() const;
    
    bool validatePassword(const std::string& password) const;
    bool validateUsername(const std::string& username) const;
    bool validateName(const std::string& name) const;

private:
    int getNewID() const;
};

#endif // USER_H