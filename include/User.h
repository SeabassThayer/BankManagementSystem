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
    User(const std::string& user, const std::string& pass, const std::string& fName, const std::string& lName, const int& id = -1);
    virtual ~User();

    int getUserID() const;
    std::string getFirstName() const;
    std::string getLastName() const;

    bool validateUsernamePassword(const std::string& password, const std::string& username) const;
    bool validateName(const std::string& fname, const std::string& lname) const;

    virtual std::string getUsername() const = 0;
    virtual std::string getPassword() const = 0;
private:
    int getNewID() const;
};

#endif // USER_H