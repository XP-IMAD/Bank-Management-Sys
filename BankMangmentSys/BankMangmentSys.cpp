#include <iostream>
#include <string>
#include <vector>
#include <cctype>
#include <fstream>
#include <iomanip>
using namespace std;

const string ClientFileName = "ClientData.txt";

void ShowMainMenueScreen();
void ShowTransactionMenueScreen();

struct stClientData
{
	string AccountNumber;
	string PINCode;
	string Name;
	string PhoneNumber;
	double AccountBalance = 0;
	bool MarkForDelete = false;

};

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

void PrintClients()
{
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

void AddClient()
{
	stClientData ClientData;
	ClientData = ReadClientData();
	AddDataLineToFile(ClientFileName, ConvertRecordToLine(ClientData));
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

bool DeleteClientByAccountNumbwer(string AccountNumber , vector<stClientData> &vClients)
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

	cout << "\n\t\t\t-----------------------------------------------------------\n";
	cout << "\t\t\t                   Adding New Client Screen";
	cout << "\n\t\t\t-----------------------------------------------------------\n";

	AddClients();
}

void ShowDeletClientScreen()
{
	
	cout << "\n\t\t\t-----------------------------------------------------------\n";
	cout << "\t\t\t                   Delete Client Screen";
	cout << "\n\t\t\t-----------------------------------------------------------\n";

	vector<stClientData> vClients = LoadClientDataFromFile(ClientFileName);
	string AccountNumber = ReadAccountNumber();

	DeleteClientByAccountNumbwer(AccountNumber, vClients);
}

void ShowUpdateClientScreen()
{
	
	cout << "\n\t\t\t-----------------------------------------------------------\n";
	cout << "\t\t\t                   Update Client Info Screen";
	cout << "\n\t\t\t-----------------------------------------------------------\n";

	vector<stClientData> vClients = LoadClientDataFromFile(ClientFileName);
	string AccountNumber = ReadAccountNumber();

	UpdateClientByAccountNumber(AccountNumber, vClients);
}

void ShowFindClientScreen()
{
	
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

enum enTransactionMenueOption
{
	enDeposit = 1 , enWithdraw = 2 , enTotalBalances = 3 , enMainMenue = 4
};

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

enum enMainMenueOption
{
	enShowClients = 1, enAddNewClient = 2, enDeletClient = 3,
	enUpdateClientInfo = 4, enFindClient = 5, enTransactions = 6,
	enEXIT = 7
};

short ReadMainMenueOption()
{
	short Option = 0;

	do
	{
		cout << "Choose What Do You Want To Do? [1 TO 7]: ";
		cin >> Option;

	} while (Option < 1 || Option > 7);

	return Option;
}

void ShowTransactionMenueScreen()
{
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
		break;

	case enEXIT:
		system("cls");
		ShowProgrammeEndsScreen();
		break;

	default:
			break;
	}
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
	cout << "\t [7] EXIT\n";
	cout << "\n=======================================================\n\n";

	enMainMenueOption ReadtheOption = (enMainMenueOption)ReadMainMenueOption();

	PerformMainMenueOption(ReadtheOption);

}

int main()
{
	ShowMainMenueScreen();

	system("pause>0");

	return 0;
}