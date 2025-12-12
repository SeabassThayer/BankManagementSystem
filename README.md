# Bank Management System

## Overview
The Bank Management System is a C++ application designed to manage user bank accounts and transactions efficiently. It provides functionalities for users to create and manage their accounts, perform transactions, and for bank managers to oversee user data and manage login processes.

## Project Structure
The project is organized into several directories and files:

- **src/**: Contains the source code files for the application.
  - `main.cpp`: Entry point of the application.
  - `User.cpp`: Implementation of the User class.
  - `Transaction.cpp`: Implementation of the Transaction class.
  - `BankAccount.cpp`: Implementation of the BankAccount class.
  - `BankManager.cpp`: Implementation of the BankManager class.
  - `LoginManager.cpp`: Implementation of the LoginManager class.
  - `Customer.cpp`: Implementation of the Customer class. 

- **include/**: Contains header files for the classes.
  - `User.h`: Declaration of the User class.
  - `Transaction.h`: Declaration of the Transaction class.
  - `BankAccount.h`: Declaration of the BankAccount class.
  - `BankManager.h`: Declaration of the BankManager class.
  - `LoginManager.h`: Declaration of the LoginManager class.
  - `Customer.cpp`: Implementation of the Customer class. 

- **data/**: Contains TXT files for user and manager login information.
  - **bankAccounts/**: Contains text files that each contain a bank account.
  - **bankInfo/**: Contains text files that each contain bank logistics.
  - **customers/**: Contains text files that each contain a customer.
  - **IDtrackers/**: Contains text files that each contain a unique ID to assign to a new entity.
  - **managers/**: Contains text files that each contain a manager.
  - **transactions/**: Contains text files that each contain a transaction.

- **CMakeLists.txt**: Configuration file for CMake to build the project.

- **Makefile**: Instructions for building the project using make.

- **.gitignore**: Specifies files and directories to be ignored by Git.

## Setup Instructions
1. Clone the repository to your local machine.
2. Navigate to the project directory.
3. Use CMake to configure the project:
   ```
   mkdir build
   cd build
   cmake ..
   ```
4. Build the project:
   ```
   make
   ```
5. Run the application:
   ```
   ./BankManagementSystem
   ```

## Features
- User account creation and management.
- Transaction handling including deposits and withdrawals.
- Manager functionalities for overseeing user data.
- Secure login processes for both users and managers.

## Future Enhancements
- Implement additional features such as loan management and interest calculations.
- Improve user interface for better user experience.
- Add more comprehensive error handling and input validation.
