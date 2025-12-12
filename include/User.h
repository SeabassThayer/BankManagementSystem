#ifndef USER_H
#define USER_H

#include <string>

// Abstract class to use in Customer & BankManager classes
class User {
private:
    // unique identifier
    int userID;

    // unique username
    std::string username;
    std::string password;
    std::string firstName;
    std::string lastName;

public:
    // Constructs a User. If 'id' is -1, a new unique ID will be assigned.
    User(const std::string& user, const std::string& pass, const std::string& fName, const std::string& lName, const int& id = -1);
    virtual ~User();

    int getUserID() const;
    std::string getFirstName() const;
    std::string getLastName() const;

    // check that Username and Password input into the class constructor are appropiate
    // MIGHT BECOME PRIVATE IF WE DO NOT USE IT OUTSIDE OF THE CLASS DEFINITON
    bool validateUsernamePassword(const std::string& password, const std::string& username) const;

    // check that First name and Last name input into the class constructor meeet basic rules.
    // MIGHT BECOME PRIVATE IF WE DO NOT USE IT OUTSIDE OF THE CLASS DEFINITON
    bool validateName(const std::string& fname, const std::string& lname) const;

    // pure virtual functions to be implemented in BankManager & Customer class. 
    std::string getUsername() const;
    std::string getPassword() const;

protected:
    // Return a unique ID from the data folder. 
    virtual int getNewID() const = 0;
};

#endif // USER_H