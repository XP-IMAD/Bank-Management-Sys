#include <iostream>
#include <string>
#include <vector>
#include <cctype>
#include <fstream>
#include <iomanip>
using namespace std;

const string ClientFileName = "ClientData.txt";
const string UsersFileName = "Users.txt";

struct stClientData
{
	string AccountNumber;
	string PINCode;
	string Name;
	string PhoneNumber;
	double AccountBalance = 0;
	bool MarkForDelete = false;

};

struct stUsers
{
	string UserName;
	string Password;
	short Permission = 0;
	bool MarkForDelete = false;
};

stUsers CurrentUser;

enum enMainMenueOption
{
	enShowClients = 1, enAddNewClient = 2, enDeletClient = 3,
	enUpdateClientInfo = 4, enFindClient = 5, enTransactions = 6,
	enManageUsers = 7, enLogout = 8
};

enum enManageUsersMenueOption
{
	enListUsers = 1, enAddNewUser = 2, enDeleteUser = 3,
	enUpdateUser = 4, enFindUser = 5, enMainM = 6
};

enum enTransactionMenueOption
{
	enDeposit = 1, enWithdraw = 2, enTotalBalances = 3, enMainMenue = 4
};

enum enMainMenuePermissions 
{
	pAll = -1 , pShowClientList = 1 , pAddClient = 2 , pDeleteClient = 4,
	pUpdateClient = 8 , pFindClient = 16 , pTransactions = 32 , 
	pManageUsers = 64
};

void ShowMainMenueScreen();
void ShowTransactionMenueScreen();
void Login();
void ShowManageUsersMenueScreen();

vector<string> SplitString(string Text, string delim)
{
	vector<string> vString;
	short pos = 0;
	string Word;

	while ((pos = Text.find(delim)) != Text.npos)
	{
		Word = Text.substr(0, pos);

		if (Word != "")
		{
			vString.push_back(Word);
		}

		Text.erase(0, pos + delim.length());
	}

	if (Text != "")
	{
		vString.push_back(Text);
	}

	return vString;

}

string ConvertRecordToLine(stClientData ClientData, string Seperator = " #//# ")
{
	string ClientLineRecord;

	ClientLineRecord += ClientData.AccountNumber + Seperator;
	ClientLineRecord += ClientData.PINCode + Seperator;
	ClientLineRecord += ClientData.Name + Seperator;
	ClientLineRecord += ClientData.PhoneNumber + Seperator;
	ClientLineRecord += to_string(ClientData.AccountBalance);

	return ClientLineRecord;

}

stClientData ConvertLineToRecord(string S1, string Seperator = " #//# ")
{
	stClientData ClientData;
	vector<string> vString = SplitString(S1, Seperator);

	ClientData.AccountNumber = vString[0];
	ClientData.PINCode = vString[1];
	ClientData.Name = vString[2];
	ClientData.PhoneNumber = vString[3];
	ClientData.AccountBalance = stod(vString[4]);

	return ClientData;
}

string ConvertUserRecordToLine(stUsers Users, string Seperator = " #//# ")
{
	string UserLineRecord;

	UserLineRecord += Users.UserName + Seperator;
	UserLineRecord += Users.Password + Seperator;
	UserLineRecord += to_string(Users.Permission);

	return UserLineRecord;
}

stUsers ConvertUserLineToRecord(string S1, string Seperator = " #//# ")
{
	stUsers Users;
	vector<string> vString = SplitString(S1, Seperator);

	Users.UserName = vString[0];
	Users.Password = vString[1];
	Users.Permission = stoi(vString[2]);

	return Users;
}

bool ClientExistByAccountNumber(string FileName , string AccountNumber)
{
	fstream MyFile;
	MyFile.open(FileName, ios::in);

	if (MyFile.is_open())
	{
		stClientData ClientData;
		string Line;

		while (getline(MyFile, Line))
		{
			ClientData = ConvertLineToRecord(Line);

			if (ClientData.AccountNumber == AccountNumber)
			{
				MyFile.close();
				return true;
			}
		}

		MyFile.close();
	}

	return false;
}

string ReadUserName()
{
	string UserName;

	cout << "Enter UserName: ";
	cin >> UserName;

	return UserName;
}

stClientData ReadClientData()
{
	stClientData ClientData;

	cout << "Enter Account Numebr: ";
	getline(cin >> ws, ClientData.AccountNumber);

	while (ClientExistByAccountNumber(ClientFileName, ClientData.AccountNumber))
	{
		cout << "\nClient With Account Number [" << ClientData.AccountNumber << "] Already Exists, "
			<< "Enter Another One: ";
		getline(cin >> ws, ClientData.AccountNumber);
	}

	cout << "Enter PIN Code: ";
	getline(cin, ClientData.PINCode);

	cout << "Enter Name: ";
	getline(cin, ClientData.Name);

	cout << "Enter Phone Number: ";
	getline(cin, ClientData.PhoneNumber);

	cout << "Enter Account Balance: ";
	cin >> ClientData.AccountBalance;

	return ClientData;

}

vector<stClientData> LoadClientDataFromFile(string FileName)
{
	stClientData ClientData;
	vector<stClientData> vClients;
	
	fstream MyFile;
	MyFile.open(FileName, ios::in);

	if (MyFile.is_open())
	{
		string Line;

		while (getline(MyFile, Line))
		{
			ClientData = ConvertLineToRecord(Line);

			vClients.push_back(ClientData);
		}

		MyFile.close();
	}

	return vClients;
}

vector<stUsers> LoadUsersFromFile(string FileName)
{
	stUsers Users;
	vector<stUsers> vUsers;

	fstream MyFile;
	MyFile.open(FileName, ios::in);

	if (MyFile.is_open())
	{
		string Line;

		while (getline(MyFile, Line))
		{
			Users = ConvertUserLineToRecord(Line);

			vUsers.push_back(Users);
		}


		MyFile.close();
	}

	return vUsers;
}

vector<stClientData> SaveClientDataToFile(string FileName,const vector<stClientData> &vClients)
{
	fstream MyFile;
	MyFile.open(FileName, ios::out);

	if (MyFile.is_open())
	{
		string DataLine;

		for (const stClientData &C : vClients)
		{
			if (C.MarkForDelete == false)
			{
				DataLine = ConvertRecordToLine(C);

				MyFile << DataLine << endl;
			}
		}

		MyFile.close();

	}

	return vClients;
}

vector<stUsers> SaveUsersToFile(string FileName, const vector<stUsers>& vUsers)
{
	fstream MyFile;
	MyFile.open(FileName, ios::out);

	if (MyFile.is_open())
	{
		string Line;

		for (const stUsers& C : vUsers)
		{
			if (C.MarkForDelete == false)
			{
				Line = ConvertUserRecordToLine(C);

				MyFile << Line << endl;
			}
		}

		MyFile.close();
	}

	return vUsers;
}

bool FindUserByUserNameAndPassword(string UserName, string Password, stUsers &User)
{
	stUsers Users;
	vector<stUsers> vUsers = LoadUsersFromFile(UsersFileName);

	for (const stUsers &C : vUsers)
	{
		if (C.UserName == UserName && C.Password == Password)
		{
			User = C;
			return true;
		}
	}

	return false;
}

bool UserExistByUserName(string UserName, string FileName)
{
	fstream MyFile;
	MyFile.open(FileName, ios::in);

	if (MyFile.is_open())
	{
		string Line;
		stUsers User;

		while (getline(MyFile, Line))
		{
			User = ConvertUserLineToRecord(Line);

			if (User.UserName == UserName)
			{
				MyFile.close();
				return true;
			}
		}

		MyFile.close();
	}

	return false;
}

short ReadPermission()
{
	short Permission = 0;
	char Answer = 'n';

	cout << "\nDo You Want To give Full access? [Y/N]: ";
	cin >> Answer;

	if (toupper(Answer) == 'Y')
	{
		return Permission = -1;
	}
	else
	{
		cout << "\nDo You Want To give access To :\n";

		cout << "\nShow Client List [Y/N]: ";
		cin >> Answer;

		if (toupper(Answer) == 'Y')
			Permission += enMainMenuePermissions::pShowClientList;

		cout << "\nAdd New Client [Y/N]: ";
		cin >> Answer;

		if (toupper(Answer) == 'Y')
			Permission += enMainMenuePermissions::pAddClient;

		cout << "\nDelete Client [Y/N]: ";
		cin >> Answer;

		if (toupper(Answer) == 'Y')
			Permission += enMainMenuePermissions::pDeleteClient;

		cout << "\nUpdate Client [Y/N]: ";
		cin >> Answer;

		if (toupper(Answer) == 'Y')
			Permission += enMainMenuePermissions::pUpdateClient;

		cout << "\nFind Client [Y/N]: ";
		cin >> Answer;

		if (toupper(Answer) == 'Y')
			Permission += enMainMenuePermissions::pFindClient;

		cout << "\nTransactions [Y/N]: ";
		cin >> Answer;

		if (toupper(Answer) == 'Y')
			Permission += enMainMenuePermissions::pTransactions;

		cout << "\nManage Users [Y/N]: ";
		cin >> Answer;

		if (toupper(Answer) == 'Y')
			Permission += enMainMenuePermissions::pManageUsers;

	}

	return Permission;
}

stUsers ReadNewUser()
{
	stUsers User;

	cout << "Enter UserName: ";
	getline(cin >> ws, User.UserName);

	while (UserExistByUserName(User.UserName, UsersFileName))
	{
		cout << "\nUser With [" << User.UserName << "] Already Exist, Enter Another UserName: ";
		getline(cin >> ws, User.UserName);
	}

	cout << "Enter Password: ";
	getline(cin, User.Password);

	User.Permission = ReadPermission();

	return User;

}

void AddDataLineToFile(string FileName , string DataLine)
{
	fstream MyFile;
	MyFile.open(FileName, ios::out | ios::app);

	if (MyFile.is_open())
	{
		MyFile << DataLine << endl;

		MyFile.close();
	}
}

bool FindClientByAccountNumber(string AccountNumber,const vector<stClientData> &vClients, stClientData &ClientData)
{
	for (const stClientData &C : vClients)
	{
		if (C.AccountNumber == AccountNumber)
		{
			ClientData = C;
			return true;
		}
	}

	return false;
}

bool FindUserByUserName(string UserName, const vector<stUsers> &vUsers, stUsers &User)
{
	for (const stUsers& C : vUsers)
	{
		if (C.UserName == UserName)
		{
			User = C;
			return true;
		}
	}

	return false;
}

void PrintClientData(stClientData ClientData)
{
	cout << "| " << setw(15) << left << ClientData.AccountNumber;
	cout << "| " << setw(10) << left << ClientData.PINCode;
	cout << "| " << setw(35) << left << ClientData.Name;
	cout << "| " << setw(12) << left << ClientData.PhoneNumber;
	cout << "| " << setw(12) << left << ClientData.AccountBalance;

}

void PrintClientCard(stClientData ClientData)
{
	cout << "\nThe Following Are The Client Details :";
	cout << "\n----------------------------------------------------------------";
	cout << "\nAccount Number  : " << ClientData.AccountNumber;
	cout << "\nPIN Code        : " << ClientData.PINCode;
	cout << "\nName            : " << ClientData.Name;
	cout << "\nPhone Number    : " << ClientData.PhoneNumber;
	cout << "\nAccount Balance : " << ClientData.AccountBalance;
	cout << "\n----------------------------------------------------------------";

}

void PrintUserCard(stUsers User)
{
	cout << "\nThe Following Are The User Details:";
	cout << "\n----------------------------------------------------";

	cout << "\nUser Name  : " << User.UserName;
	cout << "\nPassword   : " << User.Password;
	cout << "\nPermission : " << User.Permission;

	cout << "\n----------------------------------------------------";

}

void PrintUserData(stUsers User)
{
	cout << "| " << setw(20) << left << User.UserName;
	cout << "| " << setw(15) << left << User.Password;
	cout << "| " << setw(15) << left << User.Permission;

}

bool HaveAccessPermition(enMainMenuePermissions Permission)
{
	if (CurrentUser.Permission == enMainMenuePermissions::pAll)
		return true;

	if ((CurrentUser.Permission & Permission) == Permission)
		return true;
	else
		return false;
}

void ShowAccessDeniedMessage()
{
	cout << "\n-------------------------------------------------";
	cout << "\nAccess Denied,";
	cout << "\nYou Dont Have Permission To Do This";
	cout << "\nPLease Contact Your Admin";
	cout << "\n-------------------------------------------------\n";
}

void PrintClients()
{
	if (!HaveAccessPermition(enMainMenuePermissions::pShowClientList))
	{
		ShowAccessDeniedMessage();

		return;
	}

	vector<stClientData> vClients = LoadClientDataFromFile(ClientFileName);

	cout << "\n\t\t\t\t\tClient List [" << vClients.size() << "] Client(s).";
	cout << "\n_______________________________________________________________________________________"
		<< "______________\n" << endl;

	cout << "| " << setw(15) << left << "Account Number";
	cout << "| " << setw(10) << left << "PIN Code";
	cout << "| " << setw(35) << left << "Client Name";
	cout << "| " << setw(12) << left << "Phone";
	cout << "| " << setw(12) << left << "Balance";

	cout << "\n______________________________________________________________________________________"
		<< "_______________\n" << endl;


	if (vClients.size() == 0)
		cout << "\t\t\tNo Client Available In The System :(\n";
	else
	{
		for (const stClientData& C : vClients)
		{
			PrintClientData(C);

			cout << endl;
		}
	}

	cout << "\n______________________________________________________________________________________"
		<< "______________\n" << endl;

}

void ShowUsersList()
{
	vector<stUsers> vUsers = LoadUsersFromFile(UsersFileName);

	cout << "\n\t\t\t\t\tUsers List [" << vUsers.size() << "] User(s).";
	cout << "\n_________________________________________________________________________"
		<< "______________________\n" << endl;

	cout << "| " << setw(20) << left << "User Name";
	cout << "| " << setw(15) << left << "Password";
	cout << "| " << setw(15) << left << "Permissions";

	cout << "\n__________________________________________________________________________"
		<< "______________________\n" << endl;

	for (const stUsers& C : vUsers)
	{
		PrintUserData(C);

		cout << endl;
	}

	cout << "\n__________________________________________________________________________"
		<< "_____________________\n" << endl;
}

void AddClient()
{
	stClientData ClientData;
	ClientData = ReadClientData();
	AddDataLineToFile(ClientFileName, ConvertRecordToLine(ClientData));
}

void AddUser()
{
	stUsers User;
	User = ReadNewUser();
	AddDataLineToFile(UsersFileName, ConvertUserRecordToLine(User));
}

void AddClients()
{
	char AddMore = 'n';

	do
	{

		cout << "\nAdding New Client :\n\n";
		AddClient();

		cout << "\n\nClient Added Seccesfully, Do You Want To Add More Clients? (Y/N): ";
		cin >> AddMore;

	} while (toupper(AddMore) == 'Y');

}

void AddUsers()
{
	char AddMore = 'n';

	do
	{
		cout << "\nAdding New User:\n\n";
		AddUser();

		cout << "\n\nUser Added Seccessfully, Do You Want To Add More Users? [Y/N]: ";
		cin >> AddMore;

	} while (AddMore == 'y' || AddMore == 'Y');

}

string ReadAccountNumber()
{
	string AccountNumber;

	cout << "PLease Enetr Account Number: ";
	cin >> AccountNumber;

	return AccountNumber;
}

bool MarkClientforDelet(string AccountNumber, vector<stClientData>& vClients)
{
	for (stClientData& C : vClients)
	{
		if (C.AccountNumber == AccountNumber)
		{
			C.MarkForDelete = true;
			return true;
		}
	}

	return false;
}

bool MarkUserForDelete(string UserName, vector<stUsers>& vUsers)
{
	for (stUsers& C : vUsers)
	{
		if (C.UserName == UserName)
		{
			C.MarkForDelete = true;
			return true;
		}
	}

	return false;
}

bool DeleteClientByAccountNumber(string AccountNumber , vector<stClientData> &vClients)
{
	stClientData ClientData;
	char Answer = 'n';
	

	if (FindClientByAccountNumber(AccountNumber, vClients, ClientData))
	{
		PrintClientCard(ClientData);

		cout << "\n\nAre you Sure you Want To Delet This Client? (Y/N): ";
		cin >> Answer;

		if (toupper(Answer) == 'Y')
		{
			MarkClientforDelet(AccountNumber, vClients);

			SaveClientDataToFile(ClientFileName, vClients);

			cout << "\n\nClient Deleted Succesfully :)\n";
			return true;
		}
		
	}
	else
	{
		cout << "\nClient With Account Number [" << AccountNumber << "] Not Found :(\n";
		return false;
	}

	return false;
}

bool DeleteUserByUserName(string UserName, vector<stUsers> &vUsers)
{
	stUsers User;
	char Answer = 'Y';

	if (FindUserByUserName(UserName, vUsers, User))
	{
		if (UserName == "Admin")
		{
			cout << "\n\nYou Cannot Delete This User ;(\n";
			return false;
		}

		PrintUserCard(User);

		cout << "\n\nAre You Sure You Want To Delete This User? [Y/N]: ";
		cin >> Answer;

		if (toupper(Answer) == 'Y')
		{
			MarkUserForDelete(UserName, vUsers);

			SaveUsersToFile(UsersFileName, vUsers);

			cout << "\nUser Deleted Seccessfully :)\n";
			return true;
		}
	}
	else
	{
		cout << "\nUser With UserName [" << UserName << "] Not Found :(\n";
		return false;
	}

	return false;
}

stClientData ChangeClientByAccountNumber(string AccountNumber)
{
	stClientData ClientData;

	ClientData.AccountNumber = AccountNumber;

	cout << "Enter PIN Code: ";
	getline(cin >> ws, ClientData.PINCode);

	cout << "Enter Name: ";
	getline(cin, ClientData.Name);

	cout << "Enter Phone Number: ";
	getline(cin, ClientData.PhoneNumber);

	cout << "Enter Account Balance: ";
	cin  >> ClientData.AccountBalance;

	return ClientData;

}

stUsers ChangeUserByUserName(string UserName)
{
	stUsers User;

	User.UserName = UserName;

	cout << "Enter Password: ";
	getline(cin >> ws, User.Password);

	User.Permission = ReadPermission();

	return User;
}

bool UpdateClientByAccountNumber(string AccountNumber , vector<stClientData> &vClients)
{
	stClientData ClientData;
	char Answer = 'n';

	if (FindClientByAccountNumber(AccountNumber, vClients, ClientData))
	{
		PrintClientCard(ClientData);

		cout << "\n\nAre You Sure You Want To Update This Client? (Y/N): ";
		cin >> Answer;

		if (toupper(Answer) == 'Y')
		{
			for (stClientData &C : vClients)
			{
				if (C.AccountNumber == AccountNumber)
				{
					C = ChangeClientByAccountNumber(AccountNumber);
					break;
				}
			}

			SaveClientDataToFile(ClientFileName, vClients);

			cout << "\n\nClient Updated Succesfully :)\n";
			return true;
		}

	}
	else
	{
		cout << "\nClient With Account Number [" << AccountNumber << "] Not Found :(\n";
		return false;
	}

	return false;

}

bool UpdateUserByUserName(string UserName, vector<stUsers> &vUsers)
{
	stUsers User;
	char Answer = 'y';

	if (FindUserByUserName(UserName, vUsers, User))
	{
		PrintUserCard(User);

		cout << "\n\nAre You Sure You Want To Update This Client? [Y/N]: ";
		cin >> Answer;

		if (toupper(Answer) == 'Y')
		{
			for (stUsers& C : vUsers)
			{
				if (C.UserName == UserName)
				{
					C = ChangeUserByUserName(UserName);
					break;
				}
			}

			SaveUsersToFile(UsersFileName, vUsers);

			cout << "\nUser Updated Seccessfully :)\n";
			return true;

		}
	}
	else
	{
		cout << "\nUser With UserName [" << UserName << "] Not Found!\n";
		return false;
	}

	return false;
}

bool DepositBalanceToClientByAccountNumber(string AccountNumber , vector<stClientData> &vClients , double DepositAmount)
{
	char Transaction = 'n';

	cout << "\nAre You Sure You Want To Perform This Transaction? [Y/N]: ";
	cin >> Transaction;

	if (toupper(Transaction) == 'Y')
	{
		for (stClientData &C : vClients)
		{
			if (C.AccountNumber == AccountNumber)
			{
				C.AccountBalance += DepositAmount;

				SaveClientDataToFile(ClientFileName, vClients);

				cout << "\nTransaction Done Succesfully, New Balance = " << C.AccountBalance << endl;

				return true;
				
			}
		}

	}

	return false;
	
}

void showAddNewClientScreen()
{

	if (!HaveAccessPermition(enMainMenuePermissions::pAddClient))
	{
		ShowAccessDeniedMessage();

		return;
	}

	cout << "\n\t\t\t-----------------------------------------------------------\n";
	cout << "\t\t\t                   Adding New Client Screen";
	cout << "\n\t\t\t-----------------------------------------------------------\n";

	AddClients();
}

void ShowAddNewUserScreen()
{
	cout << "\n\t\t\t-----------------------------------------------------------\n";
	cout << "\t\t\t                   Adding New User Screen";
	cout << "\n\t\t\t-----------------------------------------------------------\n";

	AddUsers();
}

void ShowDeletClientScreen()
{
	if (!HaveAccessPermition(enMainMenuePermissions::pDeleteClient))
	{
		ShowAccessDeniedMessage();

		return;
	}

	cout << "\n\t\t\t-----------------------------------------------------------\n";
	cout << "\t\t\t                   Delete Client Screen";
	cout << "\n\t\t\t-----------------------------------------------------------\n";

	vector<stClientData> vClients = LoadClientDataFromFile(ClientFileName);
	string AccountNumber = ReadAccountNumber();

	DeleteClientByAccountNumber(AccountNumber, vClients);
}

void ShowDeleteUserScreen()
{
	cout << "\n\t\t\t-----------------------------------------------------------\n";
	cout << "\t\t\t                   Delete User Screen";
	cout << "\n\t\t\t-----------------------------------------------------------\n";

	vector<stUsers> vUsers = LoadUsersFromFile(UsersFileName);
	string UserName = ReadUserName();

	DeleteUserByUserName(UserName, vUsers);
}

void ShowUpdateClientScreen()
{
	if (!HaveAccessPermition(enMainMenuePermissions::pUpdateClient))
	{
		ShowAccessDeniedMessage();

		return;
	}
	
	cout << "\n\t\t\t-----------------------------------------------------------\n";
	cout << "\t\t\t                   Update Client Info Screen";
	cout << "\n\t\t\t-----------------------------------------------------------\n";

	vector<stClientData> vClients = LoadClientDataFromFile(ClientFileName);
	string AccountNumber = ReadAccountNumber();

	UpdateClientByAccountNumber(AccountNumber, vClients);
}

void ShowUpdateUserScreen()
{
	cout << "\n\t\t\t-----------------------------------------------------------\n";
	cout << "\t\t\t                   Update User Screen";
	cout << "\n\t\t\t-----------------------------------------------------------\n";

	vector<stUsers> vUsers = LoadUsersFromFile(UsersFileName);
	string UserName = ReadUserName();

	UpdateUserByUserName(UserName, vUsers);
}

void ShowFindClientScreen()
{
	if (!HaveAccessPermition(enMainMenuePermissions::pFindClient))
	{
		ShowAccessDeniedMessage();

		return;
	}

	cout << "\n\t\t\t-----------------------------------------------------------\n";
	cout << "\t\t\t                   Find Client Screen";
	cout << "\n\t\t\t-----------------------------------------------------------\n";

	vector<stClientData> vClients = LoadClientDataFromFile(ClientFileName);
	stClientData ClientData;
	string AccountNumber = ReadAccountNumber();

	if (FindClientByAccountNumber(AccountNumber, vClients, ClientData))
	{
		PrintClientCard(ClientData);
	}
	else
	{
		cout << "\nClient With Account Number [" << AccountNumber << "] Not Found :(\n";
	}
}

void ShowFindUserScreen()
{
	cout << "\n\t\t\t-----------------------------------------------------------\n";
	cout << "\t\t\t                   Find User Screen";
	cout << "\n\t\t\t-----------------------------------------------------------\n";

	vector<stUsers> vUsers = LoadUsersFromFile(UsersFileName);
	stUsers User;
	string UserName = ReadUserName();

	if (FindUserByUserName(UserName, vUsers, User))
	{
		PrintUserCard(User);
	}
	else
	{
		cout << "\nUser With UserName [" << UserName << "] Not Found :(\n";
	}
}

void ShowProgrammeEndsScreen()
{
	
	cout << "\n\t\t\t-----------------------------------------------------------\n";
	cout << "\t\t\t                   Programme Ends :)";
	cout << "\n\t\t\t-----------------------------------------------------------\n";

}

void ShowDepositScreen()
{
	cout << "\n\t\t\t-----------------------------------------------------------\n";
	cout << "\t\t\t                     Deposit Screen";
	cout << "\n\t\t\t-----------------------------------------------------------\n\n";

	vector<stClientData> vClients = LoadClientDataFromFile(ClientFileName);
	string AccountNumber = ReadAccountNumber();
	stClientData ClientData;

	while (!FindClientByAccountNumber(AccountNumber, vClients, ClientData))
	{
		cout << "\nClient With Account [" << AccountNumber << "] Not Exist.\n";
		AccountNumber = ReadAccountNumber();
	}

	PrintClientCard(ClientData);

	double Amount = 0;

	cout << "\nPlease Enter Deposit Amount: ";
	cin >> Amount;

	DepositBalanceToClientByAccountNumber(AccountNumber, vClients, Amount);

}

void ShowWithdrawScreen()
{
	cout << "\n\t\t\t-----------------------------------------------------------\n";
	cout << "\t\t\t                     Withdraw Screen";
	cout << "\n\t\t\t-----------------------------------------------------------\n\n";

	vector<stClientData> vClients = LoadClientDataFromFile(ClientFileName);
	string AccountNumber = ReadAccountNumber();
	stClientData ClientData;

	while (!FindClientByAccountNumber(AccountNumber, vClients, ClientData))
	{
		cout << "\nClient With Account [" << AccountNumber << "] Not Exist.\n";
		AccountNumber = ReadAccountNumber();
	}

	PrintClientCard(ClientData);

	double WithdrawAmount = 0;

	cout << "\nPlease Enter Withdraw Amount: ";
	cin >> WithdrawAmount;

	while (WithdrawAmount > ClientData.AccountBalance)
	{
		cout << "\nAmount Exceeds The Balance, You Can't Withdraw Up To : " << ClientData.AccountBalance;
		cout << "\nPlease Enter Another Amount: ";
		cin >> WithdrawAmount;
	}

	DepositBalanceToClientByAccountNumber(AccountNumber, vClients, WithdrawAmount * -1);

}

void ShowClientBalance(stClientData ClientData)
{
	cout << "| " << setw(20) << left << ClientData.AccountNumber;
	cout << "| " << setw(35) << left << ClientData.Name;
	cout << "| " << setw(15) << left << ClientData.AccountBalance;

}

void ShowBalancesList()
{
	vector<stClientData> vClients = LoadClientDataFromFile(ClientFileName);
	double TotalBalances = 0;

	cout << "\n\t\t\t\t\tBalances List [" << vClients.size() << "] Client(s).";
	cout << "\n_______________________________________________________________________________________"
		<< "______________\n" << endl;

	cout << "| " << setw(20) << left << "Account Number";
	cout << "| " << setw(35) << left << "Client Name";
	cout << "| " << setw(15) << left << "Balance";

	cout << "\n______________________________________________________________________________________"
		<< "_______________\n" << endl;

	if (vClients.size() == 0)
		cout << "\t\t\tNo Client Available In The System :(\n";
	else
	{
		for (const stClientData& C : vClients)
		{
			ShowClientBalance(C);
			TotalBalances += C.AccountBalance;

			cout << endl;
		}
	}

	cout << "\n______________________________________________________________________________________"
		<< "______________\n" << endl;

	cout << "\t\t\t\t Total Balances = " << TotalBalances << endl;
}

short ReadTransactionOption()
{
	short Option = 0;

	do
	{
		cout << "choose What Do You Want To Do? [1 TO 4]: ";
		cin >> Option;

	} while (Option < 1 || Option > 4);

	return Option;
}

void GoBackToTransactionMenue()
{
	cout << "\nPress Any Key To Go Back To Transaction Menue...";
	system("pause>0");

	ShowTransactionMenueScreen();
}

void PerformTransactionMenue(enTransactionMenueOption TransactionOption)
{
	switch (TransactionOption)
	{
	case enDeposit:
		system("cls");
		ShowDepositScreen();
		GoBackToTransactionMenue();
		break;

	case enWithdraw:
		system("cls");
		ShowWithdrawScreen();
		GoBackToTransactionMenue();
		break;

	case enTotalBalances:
		system("cls");
		ShowBalancesList();
		GoBackToTransactionMenue();
		break;

	case enMainMenue:
		system("cls");
		ShowMainMenueScreen();
		break;

	default:
		break;
	}

}

short ReadManageUserOption()
{
	short Option = 0;

	do
	{
		cout << "Choose What Do You Want To Do? [1 TO 6]: ";
		cin >> Option;

	} while (Option < 1 || Option > 6);

	return Option;
}

short ReadMainMenueOption()
{
	short Option = 0;

	do
	{
		cout << "Choose What Do You Want To Do? [1 TO 8]: ";
		cin >> Option;

	} while (Option < 1 || Option > 8);

	return Option;
}

void ShowTransactionMenueScreen()
{
	if (!HaveAccessPermition(enMainMenuePermissions::pTransactions))
	{
		ShowAccessDeniedMessage();

		return;
	}

	system("cls");
	cout << "\n===========================================================";
	cout << "\n                  Transaction Menue Screen:";
	cout << "\n===========================================================\n\n";

	cout << "\t [1] DEPOSIT\n";
	cout << "\t [2] WITHDRAW\n";
	cout << "\t [3] TOTAL BALANCES\n";
	cout << "\t [4] MAIN MENUE\n";

	cout << "\n===========================================================\n\n";

	enTransactionMenueOption TheTransactionOption = (enTransactionMenueOption)ReadTransactionOption();

	PerformTransactionMenue(TheTransactionOption);

}

void GoBackToMainMenue()
{
	cout << "\n\nPress Any Key To Go Back To Main Menue...";
	system("pause>0");

	ShowMainMenueScreen();
}

void PerformMainMenueOption(enMainMenueOption MainMenueOption)
{
	switch (MainMenueOption)
	{
	case enShowClients:
		system("cls");
		PrintClients();
		GoBackToMainMenue();
		break;

	case enAddNewClient:
		system("cls");
		showAddNewClientScreen();
		GoBackToMainMenue();
		break;

	case enDeletClient:
		system("cls");
		ShowDeletClientScreen();
		GoBackToMainMenue();
		break;

	case enUpdateClientInfo:
		system("cls");
		ShowUpdateClientScreen();
		GoBackToMainMenue();
		break;

	case enFindClient:
		system("cls");
		ShowFindClientScreen();
		GoBackToMainMenue();
		break;

	case enTransactions:
		system("cls");
		ShowTransactionMenueScreen();
		GoBackToMainMenue();
		break;

	case enManageUsers:
		system("cls");
		ShowManageUsersMenueScreen();
		GoBackToMainMenue();
		break;

	case enLogout:
		Login();
		break;

	default:
			break;
	}
}

void GoBackToManageUsersMenueOP()
{
	cout << "\n\nPress Any Key To Go Back To Manage Menue...";
	system("pause>0");

	ShowManageUsersMenueScreen();
}

void PerformManageUserMenue(enManageUsersMenueOption ManageUserOP)
{
	switch (ManageUserOP)
	{
	case enManageUsersMenueOption::enListUsers:
		system("cls");
		ShowUsersList();
		GoBackToManageUsersMenueOP();
		break;

	case enManageUsersMenueOption::enAddNewUser:
		system("cls");
		ShowAddNewUserScreen();
		GoBackToManageUsersMenueOP();
		break;

	case enManageUsersMenueOption::enDeleteUser:
		system("cls");
		ShowDeleteUserScreen();
		GoBackToManageUsersMenueOP();
		break;

	case enManageUsersMenueOption::enUpdateUser:
		system("cls");
		ShowUpdateUserScreen();
		GoBackToManageUsersMenueOP();
		break;

	case enManageUsersMenueOption::enFindUser:
		system("cls");
		ShowFindUserScreen();
		GoBackToManageUsersMenueOP();
		break;

	case enManageUsersMenueOption::enMainM:
		system("cls");
		ShowMainMenueScreen();
		break;

	default:
		break;

	}
}

void ShowManageUsersMenueScreen()
{
	if (!HaveAccessPermition(enMainMenuePermissions::pManageUsers))
	{
		ShowAccessDeniedMessage();

		return;
	}

	system("cls");
	cout << "\n=======================================================";
	cout << "\n\t\t  Manage Users Menue Screen :";
	cout << "\n=======================================================\n\n";

	cout << "\t [1] List Users\n";
	cout << "\t [2] Add New User\n";
	cout << "\t [3] Delete User\n";
	cout << "\t [4] Update User\n";
	cout << "\t [5] Find User\n";
	cout << "\t [6] Main Menue\n";
	cout << "\n=======================================================\n\n";

	enManageUsersMenueOption ManageUserOption = (enManageUsersMenueOption)ReadManageUserOption();

	PerformManageUserMenue(ManageUserOption);

}

void ShowMainMenueScreen()
{
	system("cls");
	cout << "\n=======================================================";
	cout << "\n\t\t  Main Menue Screen :";
	cout << "\n=======================================================\n\n";

	cout << "\t [1] Show Client List\n";
	cout << "\t [2] ADD New Client\n";
	cout << "\t [3] Delete Client\n";
	cout << "\t [4] Update Client Info\n";
	cout << "\t [5] Find Client\n";
	cout << "\t [6] Transactions\n";
	cout << "\t [7] Manage Users\n";
	cout << "\t [8] Logout\n";
	cout << "\n=======================================================\n\n";

	enMainMenueOption ReadtheOption = (enMainMenueOption)ReadMainMenueOption();

	PerformMainMenueOption(ReadtheOption);

}

bool LoadUserInfo(string UserName, string Password)
{
	if (FindUserByUserNameAndPassword(UserName, Password, CurrentUser))
		return true;
	else
		return false;
}

void ShowLoginScreen()
{
	system("cls");
	cout << "\n-------------------------------------------------\n";
	cout << "                   Login Screen";
	cout << "\n-------------------------------------------------\n";
}

void Login()
{
	bool LoginFail = false;

	string UserName, Password;

	do
	{
		ShowLoginScreen();

		if (LoginFail)
		{
			cout << "Invalid UserName/Password!\n";
		}

		cout << "Enter UserName: ";
		cin >> UserName;

		cout << "Enter Password: ";
		cin >> Password;

		LoginFail = !LoadUserInfo(UserName, Password);

	} while (LoginFail);

	ShowMainMenueScreen();
}

int main()
{

	Login();

	system("pause>0");
	return 0;
}