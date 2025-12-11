#ifndef LOGINMANAGER_H
#define LOGINMANAGER_H

#include "Customer.h"
#include "BankManager.h"
#include "BankAccount.h"

class LoginManager {
public:
    LoginManager();
    ~LoginManager();

    void run(); // main loop

private:
    void showMainMenu();
    void handleUserLogin();
    void handleAccountCreation();
    void handleManagerLogin();
};

#endif // LOGINMANAGER_H