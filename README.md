# Bank-System
Simple bank system in C++
A simple console-based Bank System developed in C++ to manage clients and users with different permissions.

## Features

### Clients
- Add, delete, search, and update client information.
- Deposit and withdraw funds for any account.
- Check account balances.
- All client data is stored in Clients.txt.

### Users
- Manage user accounts (add, delete, update) based on permissions.
- Each user has different access rights controlling what operations they can perform.
- User data is stored in Users.txt.

### Permissions
- Only users with proper permissions can perform sensitive operations like modifying users or deleting clients.
- Permissions are enforced at runtime.

## Usage
1. Log in as a user.
2. Use the menu to manage clients or users according to your permissions.
3. All data changes are saved automatically in the respective text files.
