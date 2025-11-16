#include<iostream>
#include<fstream>
#include<string>
#include<iomanip>
#include<vector>;
using namespace std;
enum enMainMenuOptions { eListClient = 1, eAddNewClients = 2, eDeleteClient = 3, eUpdateClient = 4, eFindClient = 5, eShowTransactionMenu = 6, eManageUsers = 7, eExit = 8 };
enum enTransactionMenuOptions { eDeposit = 1, eWithdrow = 2, eTotalBalance = 3, eShowMainMenu = 4 };
enum enManageUsersOption { eListUsers = 1, eAddNewUser = 2, eDeleteUser = 3, eUpdateUser = 4, eFindUser = 5, eShowMainMenu2 = 6 };
enum enPermissions
{
	eFullAccess = -1, eShowClientListPermission = 1, eAddNewClientPermission = 2, eDeleteClientPermission = 4, eUpdateClientInfoPermission = 8, eFindClientPermission = 16,
	eShowTransactionMenuPermission = 32, eManageUsersPermission = 64
};
struct sClient {
	string accountNumber;
	string pinCode;
	string name;
	string phoneNumber;
	double accountBalance;
	bool markForDelete = false;
};
struct sUser {
	string userName;
	string password;
	short permissions;
	bool markForDeleteUser = false;

};

sUser currentUser;

const string fileName = "Clients.txt";
const string userFile = "Users.txt";

void showMainMenuScreen();
void showTransactionMenu();
void showManageUsersScreen();
void showDeniedScreen();
void Login();

bool checkUserPermission(sUser user, enPermissions permission);

vector<string> splitString(string s1, string delimiter) {
	vector<string> vString;
	short pos = 0;
	string sWord;
	while ((pos = s1.find(delimiter)) != std::string::npos) {
		sWord = s1.substr(0, pos);
		if (sWord != " ")
		{
			vString.push_back(sWord);
		}
		s1.erase(0, pos + delimiter.length());
	}
	if (s1 != "")
	{
		vString.push_back(s1);
	}
	return vString;
}
sClient convertLineDataToRecord(string line, string separator = "#//#") {

	sClient client;
	vector<string> vClientData;

	vClientData = splitString(line, separator);

	client.accountNumber = vClientData[0];
	client.pinCode = vClientData[1];
	client.name = vClientData[2];
	client.phoneNumber = vClientData[3];
	client.accountBalance = stod(vClientData[4]);//cast string to double

	return client;
}
vector<sClient> loadDataFromFile(string fileName) {
	vector<sClient> vOfClients;
	fstream file1;
	sClient client;
	file1.open(fileName, ios::in);
	if (file1.is_open())
	{
		string line;
		while (getline(file1, line))
		{
			client = convertLineDataToRecord(line);

			vOfClients.push_back(client);
		}
	}
	file1.close();
	return vOfClients;
}
void printClientCard(sClient client) {
	cout << "\nThe following are the client details: ";
	cout << "\n======================================";
	cout << "\nAccount number : " << client.accountNumber;
	cout << "\nPIN code       : " << client.pinCode;
	cout << "\nName           : " << client.name;
	cout << "\nPhone number   : " << client.phoneNumber;
	cout << "\nAccount balance: " << client.accountBalance;
	cout << "\n======================================\n";
}
string readAccountNumber() {
	string accountNumber = "";
	cout << "Please enter Account Number: ";
	cin >> accountNumber;
	return accountNumber;
}
string convertRecordToLine(sClient client, string separator = "#//#") {
	string stClientRecord = "";

	stClientRecord += client.accountNumber + separator;
	stClientRecord += client.pinCode + separator;
	stClientRecord += client.name + separator;
	stClientRecord += client.phoneNumber + separator;
	stClientRecord += to_string(client.accountBalance);

	return stClientRecord;
}
bool clientExistsByAccountNumber(string accountNumber, string fileName) {
	vector<sClient> vClients;
	fstream myFile;
	myFile.open(fileName, ios::in);
	if (myFile.is_open()) {
		string line;
		sClient client;

		while (getline(myFile, line))
		{
			client = convertLineDataToRecord(line);

			if (client.accountNumber == accountNumber)
			{
				myFile.close();
				return true;
			}
			vClients.push_back(client);
		}
		myFile.close();
		return false;
	}
}
sClient readNewClient() {
	sClient C;
	cout << "Enter account number? ";
	getline(cin >> ws, C.accountNumber);

	while (clientExistsByAccountNumber(C.accountNumber, fileName))
	{
		cout << "\nClient with [" << C.accountNumber << "] already exists, Enter another account number? ";
		getline(cin >> ws, C.accountNumber);
	}

	cout << "\nEnter PIN Code: ";
	getline(cin >> ws, C.pinCode);
	cout << "Enter Name : ";
	getline(cin, C.name);
	cout << "Enter Phone Number: ";
	getline(cin, C.phoneNumber);
	cout << "Enter Account Balance: ";
	cin >> C.accountBalance;

	return C;

}

void addDataLineToFile(string fileName, string sLine) {
	fstream My1File;

	My1File.open(fileName, ios::out | ios::app);

	if (My1File.is_open())
	{

		My1File << sLine << endl;

		My1File.close();
	}
}
bool findClientByAccountNumber(vector<sClient> vClients, string accountNumber, sClient& client) {

	for (sClient& C : vClients)
	{

		if (C.accountNumber == accountNumber)
		{
			client = C;
			return true;
		}
	}

	return false;
}
bool markClientForDeleteByAccountNumber(vector<sClient>& vClients, string accountNumber) {
	for (sClient& c : vClients) {
		if (c.accountNumber == accountNumber)
		{
			c.markForDelete = true;
			return true;
		}
	}
	return false;
}

void saveDataToFile(vector<sClient> vClients, string accountNumber) {
	fstream file1;
	file1.open(fileName, ios::out);
	if (file1.is_open())
	{
		for (sClient& c : vClients)
		{
			string dataLine;
			if (c.markForDelete == false)
			{
				dataLine = convertRecordToLine(c);
				file1 << dataLine << endl;
			}
		}
	}
	file1.close();
}
void showFindClientScreen() {
	if (!checkUserPermission(currentUser, enPermissions::eFindClientPermission))
	{
		showDeniedScreen();
		return;
	}
	cout << "\n=======================================";
	cout << "\n\tFind Client Screen Info";
	cout << "\n=======================================\n";
	vector<sClient> vClients = loadDataFromFile(fileName);
	string accountNumber = readAccountNumber();
	sClient client;
	if (findClientByAccountNumber(vClients, accountNumber, client))
	{
		printClientCard(client);
	}
	else
		cout << "Client with [" << accountNumber << "] is not found!";
}
sClient changeClientRecord(string accountNumber)
{
	sClient C;
	C.accountNumber = accountNumber;
	cout << "\nEnter PIN Code: ";
	getline(cin >> ws, C.pinCode);
	cout << "Enter Name : ";
	getline(cin, C.name);
	cout << "Enter Phone Number: ";
	getline(cin, C.phoneNumber);
	cout << "Enter Account Balance: ";
	cin >> C.accountBalance;
	return C;
}
bool updateClientByAccountNumber(vector<sClient>& vClients, string accountNumber) {
	sClient client;
	char updateClient = 'y';
	if (findClientByAccountNumber(vClients, accountNumber, client))
	{

		printClientCard(client);

		cout << "\n\nAre you sure you want update this client ? y\\n? ";
		cin >> updateClient;

		if (tolower(updateClient) == 'y')
		{
			for (sClient& c : vClients)
			{
				if (c.accountNumber == accountNumber)
				{
					c = changeClientRecord(accountNumber);
					break;
				}
			}
			saveDataToFile(vClients, accountNumber);

			cout << "\nClient updated successfully. \n";
			return true;

		}
	}
	else
	{
		cout << "Client with account number (" << accountNumber << ") is not found!";
		return false;
	}
}
void showUpdateClientScreen() {
	if (!checkUserPermission(currentUser, enPermissions::eUpdateClientInfoPermission))
	{
		showDeniedScreen();
		return;
	}
	cout << "\n========================================";
	cout << "\n\tUpdate Client Screen Info";
	cout << "\n========================================\n";
	vector<sClient> vClients = loadDataFromFile(fileName);
	string accountNumber = readAccountNumber();
	updateClientByAccountNumber(vClients, accountNumber);
}
bool deleteClientByAccountNumber(vector<sClient>& vClients, string accountNumber) {
	sClient client;

	char deleteClient = 'y';

	if (findClientByAccountNumber(vClients, accountNumber, client))
	{

		printClientCard(client);

		cout << "\nAre you sure you want to delete this client? y/n? ";
		cin >> deleteClient;

		if (tolower(deleteClient) == 'y')
		{
			markClientForDeleteByAccountNumber(vClients, accountNumber);
			saveDataToFile(vClients, accountNumber);

			vClients = loadDataFromFile(fileName);

			cout << "\nClient deleted successfully. " << endl;
			return true;
		}
	}
	else
	{
		cout << "\nClient with Account Number (" << accountNumber << ") is NOT found!\n";
		return false;
	}
}
void showDeleteClientScreen() {
	if (!checkUserPermission(currentUser, enPermissions::eDeleteClientPermission))
	{
		showDeniedScreen();
		return;
	}
	cout << "\n=======================================";
	cout << "\n\tDelete Client Screen Info";
	cout << "\n=======================================\n";
	vector<sClient> vClients = loadDataFromFile(fileName);
	string accountNumber = readAccountNumber();
	deleteClientByAccountNumber(vClients, accountNumber);
}
void addNewClient() {
	sClient client;
	client = readNewClient();
	addDataLineToFile(fileName, convertRecordToLine(client));
}

void addNewClients() {

	if (!checkUserPermission(currentUser, enPermissions::eAddNewClientPermission))
	{
		showDeniedScreen();
		return;
	}
	char addMoreClient = 'Y';

	do
	{

		system("cls");

		cout << "\n=======================================";
		cout << "\n\Add new client screen";
		cout << "\n=======================================\n";

		cout << "Adding new client: \n";

		addNewClient();

		cout << "Client added successfully, do you want to add more client? ";
		cin >> addMoreClient;

	} while (toupper(addMoreClient) == 'Y');
}
void printClientRecordLine(sClient client) {

	cout << "| " << left << setw(15) << client.accountNumber;
	cout << "| " << left << setw(10) << client.pinCode;
	cout << "| " << left << setw(30) << client.name;
	cout << "| " << left << setw(12) << client.phoneNumber;
	cout << "| " << left << setw(12) << client.accountBalance;

}
void showDeniedScreen() {
	system("cls");
	cout << "====================================\n";
	cout << "Access Denied!, \nYou don't have permission to do this,\nPlease contact you admin.\n";
	cout << "====================================\n";


}
bool checkUserPermission(sUser user, enPermissions permission) {

	if (user.permissions == enPermissions::eFullAccess)
		return true;

	if ((permission & user.permissions) == permission)
		return true;

	return false;
}
void ShowAllClientsScreen() {
	if (!checkUserPermission(currentUser, enPermissions::eShowClientListPermission))
	{
		showDeniedScreen();
		return;
	}
	vector<sClient> vOfClients = loadDataFromFile(fileName);
	cout << "\n\t\t\t\tClient List(" << vOfClients.size() << ") Client(s).\n";

	cout << "\n__________________________________________________________________";
	cout << "______________________________________________\n";

	cout << "| " << left << setw(15) << "Account Number";
	cout << "| " << left << setw(10) << "PIN Code";
	cout << "| " << left << setw(30) << "Name";
	cout << "| " << left << setw(12) << "Phone Number";
	cout << "| " << left << setw(12) << "Account Balance";

	cout << "\n__________________________________________________________________";
	cout << "______________________________________________\n";

	if (vOfClients.size() == 0)
	{
		cout << "\n\t\t\t\tNo clients available in this system! \n\n";
	}

	for (sClient& client : vOfClients) {
		printClientRecordLine(client);
		cout << endl;
	}
}
bool depositBalanceToClientByAccountNumber(string accountNumber, double amount, vector<sClient> vClients) {

	char answer = 'n';
	cout << "\n\nAre you sure you want perform this transactions? y/n? ";
	cin >> answer;

	if (answer == 'y' || answer == 'Y')
	{
		for (sClient& client : vClients)
		{
			if (client.accountNumber == accountNumber)
			{
				client.accountBalance += amount;
				saveDataToFile(vClients, accountNumber);
				cout << "\n\nDone successfully.New Balance is: " << client.accountBalance;
				return true;
			}
		}
		return false;
	}
}
void showDepositScreen() {
	cout << "============================================";
	cout << "\n             Deposit Screen             ";
	cout << "\n============================================\n";
	sClient client;
	vector<sClient> vClients = loadDataFromFile(fileName);
	string accountNumber = readAccountNumber();

	while (!findClientByAccountNumber(vClients, accountNumber, client))
	{
		cout << "\nClient with [" << accountNumber << "] does not exist. \n";
		accountNumber = readAccountNumber();
	}

	printClientCard(client);

	double amount = 0;
	cout << "\nPlease enter deposit amount? ";
	cin >> amount;

	depositBalanceToClientByAccountNumber(accountNumber, amount, vClients);
}
void showWithDrawScreen() {
	cout << "============================================";
	cout << "\n             Withdraw Screen             ";
	cout << "\n============================================\n";
	sClient client;
	vector <sClient> vClients = loadDataFromFile(fileName);
	string accountNumber = readAccountNumber();

	while (!findClientByAccountNumber(vClients, accountNumber, client))
	{
		cout << "\nClient with [" << accountNumber << "] does not exist. \n";
		accountNumber = readAccountNumber();
	}

	printClientCard(client);

	double amount = 0;
	cout << "\nPlease enter withdraw amount? ";
	cin >> amount;
	while (amount > client.accountBalance) {
		cout << "Amount Exceeds The Balance, you can withdraw uo to: " << client.accountBalance;
		cout << "Please enter another amount? ";
		cin >> amount;
	}
	depositBalanceToClientByAccountNumber(accountNumber, amount * -1, vClients);
}
void printClientRecordBalanceLine(sClient client) {

	cout << "| " << left << setw(15) << client.accountNumber;
	cout << "| " << left << setw(40) << client.name;
	cout << "| " << left << setw(12) << client.accountBalance;

}
void calTotalBalancesScreen() {

	double totalBalances = 0;
	vector <sClient> vClients = loadDataFromFile(fileName);

	cout << "\n\t\t\t\t\tBalances List(" << vClients.size() << ") Client(s). ";
	cout << "\n__________________________________________________________________";
	cout << "______________________________________________\n";

	cout << "| " << left << setw(15) << "Account Number";
	cout << "| " << left << setw(40) << "Client Name";
	cout << "| " << left << setw(12) << "Balance";

	cout << "\n__________________________________________________________________";
	cout << "______________________________________________\n";

	if (vClients.size() == 0) {
		cout << "\t\t\t\tNo Clients Available In This System!";
	}
	else
		for (sClient client : vClients) {
			printClientRecordBalanceLine(client);
			cout << "\n";
			totalBalances += client.accountBalance;
		}

	cout << "\n__________________________________________________________________";
	cout << "______________________________________________\n" << endl;
	cout << "\t\t\t\t\t    Total Balances  = " << totalBalances;
}
void goBackToTransactionMenu() {
	cout << "\nPress any key to go back to transaction menu..";
	system("pause>0");
	showTransactionMenu();
}
void performTransactionMenuOption(enTransactionMenuOptions transactionMenuOption) {
	switch (transactionMenuOption)
	{
	case enTransactionMenuOptions::eDeposit:
	{
		system("cls");
		showDepositScreen();
		goBackToTransactionMenu();
		break;
	}
	case enTransactionMenuOptions::eWithdrow:
	{
		system("cls");
		showWithDrawScreen();
		goBackToTransactionMenu();
		break;
	}
	case enTransactionMenuOptions::eTotalBalance:
	{
		system("cls");
		calTotalBalancesScreen();
		goBackToTransactionMenu();
		break;
	}
	case enTransactionMenuOptions::eShowMainMenu:
	{
		system("cls");
		showMainMenuScreen();
		break;
	}
	}
}
short readMainMenuOption() {
	cout << "\n\n Choose what you want to do? [1 To 6]?  ";
	short choice = 0;
	cin >> choice;
	return choice;
}
short readTransactionMenuOption() {
	cout << "\n\n Choose what you want to do? [1 To 4]?  ";
	short choice = 0;
	cin >> choice;
	return choice;
}
void showTransactionMenu() {
	if (!checkUserPermission(currentUser, enPermissions::eShowTransactionMenuPermission))
	{
		showDeniedScreen();
		return;
	}
	system("cls");
	cout << "============================================";
	cout << "\n             Transaction menu Screen             ";
	cout << "\n============================================";
	cout << "\n[1] Deposit.";
	cout << "\n[2] Withdraw.";
	cout << "\n[3] Total Balances.";
	cout << "\n[4] Main Menu";
	cout << "\n============================================";
	performTransactionMenuOption((enTransactionMenuOptions)readTransactionMenuOption());

}
void goBackToManageUsersMenu() {
	cout << "\nPress any key to go back to manage users menu...\n";
	system("pause>0");

	showManageUsersScreen();
}
void showEndScreen() {
	cout << "\n=====================================";
	cout << "\n\tProgram ends :-) \n";
	cout << "\n=====================================\n";
}
void goBackToMainMenu() {
	cout << "\nPress any key to go back t main menu...\n";
	system("pause>0");

	showMainMenuScreen();
}
sUser convertLineDateToUserRecord(string line) {
	sUser user;
	vector<string> vUsers = splitString(line, "#//#");

	user.userName = vUsers[0];
	user.password = vUsers[1];
	user.permissions = stoi(vUsers[2]);

	return user;
}
string convertUserRecordToLine(sUser user) {
	return user.userName + "#//#" + user.password + "#//#" + to_string(user.permissions);
}
vector<sUser> loadDataFromUserFile(string fileName) {
	vector<sUser> vOfUsers;
	fstream file;
	sUser user;

	file.open(fileName, ios::in);
	if (file.is_open())
	{
		string line;
		while (getline(file, line))
		{
			user = convertLineDateToUserRecord(line);

			vOfUsers.push_back(user);
		}
	}
	file.close();
	return vOfUsers;
}
sUser readUserInformation() {
	sUser user;

	cout << "\nEnter Username: ";
	cin >> user.userName;
	cout << "Enter Password: ";
	cin >> user.password;

	return user;
}
bool findUserByUsernameAndPassword(string UserName, string Password, sUser& user) {
	vector<sUser> vUsers = loadDataFromUserFile(userFile);

	for (sUser& u : vUsers) {
		if (u.userName == UserName && u.password == Password)
		{
			user = u;
			return true;
		}
	}
	return false;
}

bool loadUserInfo(string userName, string password) {
	if (findUserByUsernameAndPassword(userName, password, currentUser))
	{
		return true;
	}
	return false;
}
short readManageMenuOption() {
	cout << "\n\n Choose what you want to do? [1 To 6]?  ";
	short choice = 0;
	cin >> choice;
	return choice;
}
void printUserRecordLine(sUser user) {
	cout << "| " << left << setw(15) << user.userName;
	cout << "| " << left << setw(15) << user.password;
	cout << "| " << left << setw(15) << user.permissions;
}
void showAllUsersScreen() {
	vector<sUser> vUsers = loadDataFromUserFile(userFile);
	cout << "\n\t\t\t\tUsers List(" << vUsers.size() << ") Users(s).\n";

	cout << "__________________________________________________________________";
	cout << "__________________________\n";
	cout << "| " << left << setw(15) << "User Name";
	cout << "| " << left << setw(15) << "Password";
	cout << "| " << left << setw(15) << "permissions";
	cout << "\n__________________________________________________________________";
	cout << "__________________________\n";
	for (sUser& u : vUsers) {
		printUserRecordLine(u);
		cout << endl;
	}
}


bool findUserByUserName(vector<sUser>& vUsers, string UserName, sUser& user) {
	vUsers = loadDataFromUserFile(userFile);
	for (sUser& u : vUsers) {
		if (u.userName == UserName)
		{
			user = u;
			return true;
		}
	}
	return false;
}

short readPermissions() {
	sUser user;
	short permission = 0;

	char access = 'y';

	cout << "\nDo you want to give full access? y/n?";
	cin >> access;


	if (toupper(access) == 'Y')
	{
		permission = enPermissions::eFullAccess;
		return enPermissions::eFullAccess;
	}
	else
	{
		cout << "\nDo you want to give access to: ";

		cout << "\nShow client list? y/n? ";
		cin >> access;
		if (toupper(access) == 'Y')
		{
			permission = enPermissions::eShowClientListPermission;
		}
		cout << "\nAdd New client ? y/n? ";
		cin >> access;
		if (toupper(access) == 'Y')
		{
			permission += enPermissions::eAddNewClientPermission;
		}
		cout << "\nDelete client ? y/n? ";
		cin >> access;
		if (toupper(access) == 'Y')
		{
			permission += enPermissions::eDeleteClientPermission;
		}
		cout << "\nUpdate client Info ? y/n? ";
		cin >> access;
		if (toupper(access) == 'Y')
		{
			permission += enPermissions::eUpdateClientInfoPermission;
		}
		cout << "\nFind client ? y/n? ";
		cin >> access;
		if (toupper(access) == 'Y')
			permission += enPermissions::eFindClientPermission;
		cout << "\nShow Transaction Menu ? y/n? ";
		cin >> access;
		if (toupper(access) == 'Y')
			permission += enPermissions::eShowTransactionMenuPermission;
		cout << "\nManage Users? y/n? ";
		cin >> access;
		if (toupper(access) == 'Y')
			permission += enPermissions::eManageUsersPermission;
	}
	return permission;
}
sUser readNewUser()
{
	vector<sUser> users = loadDataFromUserFile(userFile);
	sUser user;

	cout << "Enter Username? ";
	getline(cin >> ws, user.userName);

	while (findUserByUserName(users, user.userName, user))
	{
		cout << "User with [" << user.userName << "] already exists, Enter another Username? ";
		getline(cin >> ws, user.userName);
	}

	cout << "Enter Password? ";
	getline(cin >> ws, user.password);

	user.permissions = readPermissions();

	return user;
}

void addNewUser() {
	sUser user;
	user = readNewUser();
	addDataLineToFile(userFile, convertUserRecordToLine(user));
}
void showAddUsersScreen() {
	char addMoreUsers = 'y';
	do
	{
		system("cls");

		cout << "\n=====================================";
		cout << "\n          Add New User Screen         \n";
		cout << "\n=====================================\n";
		cout << "Adding New User: \n";

		addNewUser();

		cout << "\nDo you want to add more users? y/n? ";
		cin >> addMoreUsers;

	} while (toupper(addMoreUsers) == 'Y');

}

bool markUserToDelete(vector<sUser>& vUsers, string userName) {
	for (sUser& users : vUsers)
	{
		if (users.userName == userName)
		{
			users.markForDeleteUser = true;
			return true;
		}
	}
	return false;
}
void printUserCard(sUser user) {
	cout << "\nThe following are the user details: ";
	cout << "\n======================================";
	cout << "\nUsername    : " << user.userName;
	cout << "\nPassword    : " << user.password;
	cout << "\npermissions : " << user.permissions;
	cout << "\n======================================\n";
}
void saveDataToUserFile(string fileName, vector<sUser> vUsers, string userName) {
	fstream file1;
	file1.open(fileName, ios::out);
	if (file1.is_open())
	{
		for (sUser& u : vUsers)
		{
			string dataLine;
			if (u.markForDeleteUser == false)
			{
				dataLine = convertUserRecordToLine(u);
				file1 << dataLine << endl;
			}
		}
	}
	file1.close();
}
bool deleteUserByUserName(vector<sUser>& vUser, string userName) {
	sUser user;
	char deleteUser = 'y';

	if (findUserByUserName(vUser, userName, user))
	{
		if (userName == "Admin")
		{
			cout << "\nYou cannot delete this user.\n";
		}
		else
		{
			printUserCard(user);

			cout << "\nAre you sure you want to delete this client? y/n? ";
			cin >> deleteUser;

			if (tolower(deleteUser) == 'y')
			{
				markUserToDelete(vUser, userName);
				saveDataToUserFile(userFile, vUser, userName);

				vUser = loadDataFromUserFile(userFile);

				cout << "\nUser deleted successfully. " << endl;
				return true;
			}
		}
	}
	else
	{
		cout << "\nUser with Username (" << userName << ") is NOT found!\n";
		return false;
	}
}
string readUserName() {
	string username;
	cout << "Please Enter Username? ";
	cin >> username;
	return username;
}
void showDeleteUserScreen()
{
	cout << "\n=======================================";
	cout << "\n\tDelete User Screen     ";
	cout << "\n=======================================\n";
	vector<sUser> vUsers = loadDataFromUserFile(userFile);

	string userName = readUserName();

	deleteUserByUserName(vUsers, userName);
}
sUser readNewInfo(sUser u, string userName) {
	u.userName = userName;

	cout << "Enter Password? ";
	getline(cin >> ws, u.password);

	u.permissions = readPermissions();

	return u;
}
bool updateUserInfo(vector<sUser>& vUsers, string userName) {
	sUser user;
	char updateInfo = 'y';
	if (findUserByUserName(vUsers, userName, user)) {
		printUserCard(user);
		cout << "Are you sure you want update this user? y/n? ";
		cin >> updateInfo;
		if (tolower(updateInfo) == 'y')
		{
			for (sUser& u : vUsers)
			{
				if (u.userName == userName)
					u = readNewInfo(u, userName);
			}
			saveDataToUserFile(userFile, vUsers, userName);
			vUsers = loadDataFromUserFile(userFile);

			cout << "\nUser updated successfully. " << endl;
			return true;
		}

	}
	else
	{
		cout << "\nUser with Username (" << userName << ") is NOT found!\n";
		return false;
	}
}
void showUpdateUserScreen()
{
	cout << "\n=======================================";
	cout << "\n\tUpdate User Screen     ";
	cout << "\n=======================================\n";
	vector<sUser> vUsers = loadDataFromUserFile(userFile);
	string userName = readUserName();
	updateUserInfo(vUsers, userName);
}
void showFindUserScreen() {
	cout << "\n=======================================";
	cout << "\n\tFind User Screen     ";
	cout << "\n=======================================\n";
	vector<sUser> vUsers = loadDataFromUserFile(userFile);
	string userName = readUserName();

	if (findUserByUserName(vUsers, userName, currentUser))
	{
		printUserCard(currentUser);
	}
	else
	{
		cout << "\nUser with [" << userName << "] is not found!";
	}
}
void performManageUsersOption(enManageUsersOption manageUsersOption) {
	switch (manageUsersOption)
	{
	case enManageUsersOption::eListUsers:
	{
		system("cls");
		showAllUsersScreen();
		goBackToManageUsersMenu();
		break;
	}
	case enManageUsersOption::eAddNewUser:
	{
		system("cls");
		showAddUsersScreen();
		goBackToManageUsersMenu();
		break;
	}
	case enManageUsersOption::eDeleteUser:
	{
		system("cls");
		showDeleteUserScreen();
		goBackToManageUsersMenu();
		break;
	}
	case enManageUsersOption::eUpdateUser:
	{
		system("cls");
		showUpdateUserScreen();
		goBackToManageUsersMenu();
		break;
	}
	case enManageUsersOption::eFindUser:
	{
		system("cls");
		showFindUserScreen();
		goBackToManageUsersMenu();
		break;
	}
	case enManageUsersOption::eShowMainMenu2:
	{
		system("cls");
		showMainMenuScreen();
		break;
	}
	}
}
void showManageUsersScreen() {
	if (!checkUserPermission(currentUser, enPermissions::eManageUsersPermission))
	{
		showDeniedScreen();
		return;
	}
	system("cls");
	cout << "============================================";
	cout << "\n            Manage Users Menu Screen             ";
	cout << "\n============================================";
	cout << "\n[1] List Users.";
	cout << "\n[2] Add New User.";
	cout << "\n[3] Delete User.";
	cout << "\n[4] Update User.";
	cout << "\n[5] Find User.";
	cout << "\n[6] Main Menu";
	cout << "\n============================================";
	performManageUsersOption((enManageUsersOption)readManageMenuOption());

}
void performMainMenuOption(enMainMenuOptions mainMenuOption) {

	switch (mainMenuOption)
	{
	case enMainMenuOptions::eListClient:
	{
		system("cls");
		ShowAllClientsScreen();
		goBackToMainMenu();
		break;
	}
	case enMainMenuOptions::eAddNewClients:
	{
		system("cls");
		addNewClients();
		goBackToMainMenu();
		break;
	}
	case enMainMenuOptions::eDeleteClient:
	{
		system("cls");
		showDeleteClientScreen();
		goBackToMainMenu();
		break;
	}
	case enMainMenuOptions::eUpdateClient:
	{
		system("cls");
		showUpdateClientScreen();
		goBackToMainMenu();
		break;
	}
	case enMainMenuOptions::eFindClient:
	{
		system("cls");
		showFindClientScreen();
		goBackToMainMenu();
		break;
	}
	case enMainMenuOptions::eShowTransactionMenu:
	{
		system("cls");
		showTransactionMenu();
		goBackToMainMenu();
		break;
	}
	case enMainMenuOptions::eManageUsers:
	{
		system("cls");
		showManageUsersScreen();
		goBackToMainMenu();
		break;
	}
	case enMainMenuOptions::eExit:
	{
		system("cls");
		Login();
		break;
	}
	}
}
void showMainMenuScreen()
{
	system("cls");

	cout << "============================================";
	cout << "\n             Main Menu Screen             ";
	cout << "\n============================================";
	cout << "\n\n[1] Show All Clients.";
	cout << "\n[2] Add New Client.";
	cout << "\n[3] Delete Client.";
	cout << "\n[4] Update Client Info.";
	cout << "\n[5] Find Client.";
	cout << "\n[6] Transaction.";
	cout << "\n[7] Manage Users.";
	cout << "\n[8] Logout.";
	cout << "\n============================================";
	performMainMenuOption((enMainMenuOptions)readMainMenuOption());
}

void Login()
{
	bool login = true;
	do
	{

		system("cls");
		cout << "============================================";
		cout << "\n             Login Screen             ";
		cout << "\n============================================";
		if (!login)
		{
			cout << "\nInvalid Username/Password!";
		}
		currentUser = readUserInformation();

		login = loadUserInfo(currentUser.userName, currentUser.password);

	} while (!login);

	showMainMenuScreen();
}
int main() {
	Login();
	system("pause>0");
	return 0;
}