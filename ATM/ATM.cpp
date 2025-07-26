#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <iomanip>

using namespace std;
const string ClientsFileName = "Clients.txt";
const string UsersFileName = "Users.txt";

void ShowMainMenue();
void ShowTransactionsMenue();
enum enTransactionsMenueOptions { eDeposit = 1, eWithdraw = 2, eShowTotalBalance = 3, eShowMainMenue = 4 };

enum enMainMenueOptions { eListClients = 1, eAddNewClient = 2, eDeleteClient = 3, eUpdateClient = 4, eFindClient = 5, eShowTransactionsMenue = 6,eManageUsers = 7, eLogOut = 8 };

enum enManageUsersOptions { eListusers = 1, eAddNewUser = 2, eDeleteuser = 3, eUpdateUser = 4, efindUser = 5, eMainMenue = 6 };

struct sClient
{
    string AccountNumber;
    string PinCode;
    string Name;
    string Phone;
    double AccountBalance;
    bool MarkForDelete = false;
};

struct sUser
{
    string UserName;
    string UserPassword;
    int UserPremssion;
    bool MarkForDelete = false;
};

sUser CurrentUser;

enum enMainMenuePermissions {
    eAll = -1, pListClients = 1, pAddNewClient = 2, pDeleteClient = 4,
    pUpdateClients = 8, pFindClient = 16, pTranactions = 32, pManageUsers = 64
};

void GoBackToMainMenue();

vector<string> SplitString(string S1, string Delim)
{

    vector<string> vString;

    short pos = 0;
    string sWord;  

     
    while ((pos = S1.find(Delim)) != std::string::npos)
    {
        sWord = S1.substr(0, pos); // store the word   
        if (sWord != "")
        {
            vString.push_back(sWord);
        }

        S1.erase(0, pos + Delim.length());  
    }

    if (S1 != "")
    {
        vString.push_back(S1);
    }

    return vString;

}

sClient ConvertLinetoRecord(string Line, string Seperator = "#//#")
{

    sClient Client;
    vector<string> vClientData;

    vClientData = SplitString(Line, Seperator);

    Client.AccountNumber = vClientData[0];
    Client.PinCode = vClientData[1];
    Client.Name = vClientData[2];
    Client.Phone = vClientData[3];
    Client.AccountBalance = stod(vClientData[4]);


    return Client;

}

string ConvertRecordToLine(sClient Client, string Seperator = "#//#")
{

    string stClientRecord = "";

    stClientRecord += Client.AccountNumber + Seperator;
    stClientRecord += Client.PinCode + Seperator;
    stClientRecord += Client.Name + Seperator;
    stClientRecord += Client.Phone + Seperator;
    stClientRecord += to_string(Client.AccountBalance);

    return stClientRecord;

}

bool ClientExistsByAccountNumber(string AccountNumber, string FileName)
{

    vector <sClient> vClients;

    fstream MyFile;
    MyFile.open(FileName, ios::in);

    if (MyFile.is_open())
    {

        string Line;
        sClient Client;

        while (getline(MyFile, Line))
        {

            Client = ConvertLinetoRecord(Line);
            if (Client.AccountNumber == AccountNumber)
            {
                MyFile.close();
                return true;
            }


            vClients.push_back(Client);
        }

        MyFile.close();

    }

    return false;


}

sUser ConvertlineToRecord_Users(string Line, string Seperator = "#//#")
{
    sUser User;
    vector <string> vUsersData ;
    vUsersData = SplitString(Line, Seperator);
    User.UserName = vUsersData[0];
    User.UserPassword = vUsersData[1];
    User.UserPremssion = stoi(vUsersData[2]);
    return User;
}

sClient ReadNewClient()
{
    sClient Client;

    cout << "Enter Account Number? ";

    
    getline(cin >> ws, Client.AccountNumber);

    while (ClientExistsByAccountNumber(Client.AccountNumber, ClientsFileName))
    {
        cout << "\nClient with [" << Client.AccountNumber << "] already exists, Enter another Account Number? ";
        getline(cin >> ws, Client.AccountNumber);
    }


    cout << "Enter PinCode? ";
    getline(cin, Client.PinCode);

    cout << "Enter Name? ";
    getline(cin, Client.Name);

    cout << "Enter Phone? ";
    getline(cin, Client.Phone);

    cout << "Enter AccountBalance? ";
    cin >> Client.AccountBalance;

    return Client;

}

vector <sClient> LoadCleintsDataFromFile(string FileName)
{

    vector <sClient> vClients;

    fstream MyFile;
    MyFile.open(FileName, ios::in);//read Mode

    if (MyFile.is_open())
    {

        string Line;
        sClient Client;

        while (getline(MyFile, Line))
        {

            Client = ConvertLinetoRecord(Line);

            vClients.push_back(Client);
        }

        MyFile.close();

    }

    return vClients;

}

void PrintClientRecordLine(sClient Client)
{

    cout << "| " << setw(15) << left << Client.AccountNumber;
    cout << "| " << setw(10) << left << Client.PinCode;
    cout << "| " << setw(40) << left << Client.Name;
    cout << "| " << setw(12) << left << Client.Phone;
    cout << "| " << setw(12) << left << Client.AccountBalance;

}

void PrintClientRecordBalanceLine(sClient Client)
{

    cout << "| " << setw(15) << left << Client.AccountNumber;
    cout << "| " << setw(40) << left << Client.Name;
    cout << "| " << setw(12) << left << Client.AccountBalance;

}

bool CheckAccessPermission(enMainMenuePermissions Permission);

void ShowAllClientsScreen()
{
    if (CheckAccessPermission(enMainMenuePermissions::pListClients))
    {

    }
    else {
        cout << "\n You don't have premission !!!";
        GoBackToMainMenue();
    }

    vector <sClient> vClients = LoadCleintsDataFromFile(ClientsFileName);

    cout << "\n\t\t\t\t\tClient List (" << vClients.size() << ") Client(s).";
    cout << "\n_______________________________________________________";
    cout << "_________________________________________\n" << endl;

    cout << "| " << left << setw(15) << "Accout Number";
    cout << "| " << left << setw(10) << "Pin Code";
    cout << "| " << left << setw(40) << "Client Name";
    cout << "| " << left << setw(12) << "Phone";
    cout << "| " << left << setw(12) << "Balance";
    cout << "\n_______________________________________________________";
    cout << "_________________________________________\n" << endl;

    if (vClients.size() == 0)
        cout << "\t\t\t\tNo Clients Available In the System!";
    else

        for (sClient Client : vClients)
        {

            PrintClientRecordLine(Client);
            cout << endl;
        }

    cout << "\n_______________________________________________________";
    cout << "_________________________________________\n" << endl;

}

bool CheckAccessPermission(enMainMenuePermissions Permission)
{
    if (CurrentUser.UserPremssion == enMainMenuePermissions::eAll)
    {
        return true;
    }
    if ((Permission & CurrentUser.UserPremssion) == Permission)
        return true;
    else return false;
}

void ShowTotalBalances()
{

    vector <sClient> vClients = LoadCleintsDataFromFile(ClientsFileName);

    cout << "\n\t\t\t\t\tBalances List (" << vClients.size() << ") Client(s).";
    cout << "\n_______________________________________________________";
    cout << "_________________________________________\n" << endl;

    cout << "| " << left << setw(15) << "Accout Number";
    cout << "| " << left << setw(40) << "Client Name";
    cout << "| " << left << setw(12) << "Balance";
    cout << "\n_______________________________________________________";
    cout << "_________________________________________\n" << endl;

    double TotalBalances = 0;

    if (vClients.size() == 0)
        cout << "\t\t\t\tNo Clients Available In the System!";
    else

        for (sClient Client : vClients)
        {

            PrintClientRecordBalanceLine(Client);
            TotalBalances += Client.AccountBalance;

            cout << endl;
        }

    cout << "\n_______________________________________________________";
    cout << "_________________________________________\n" << endl;
    cout << "\t\t\t\t\t   Total Balances = " << TotalBalances;

}

void PrintClientCard(sClient Client)
{
    cout << "\nThe following are the client details:\n";
    cout << "-----------------------------------";
    cout << "\nAccout Number: " << Client.AccountNumber;
    cout << "\nPin Code     : " << Client.PinCode;
    cout << "\nName         : " << Client.Name;
    cout << "\nPhone        : " << Client.Phone;
    cout << "\nAccount Balance: " << Client.AccountBalance;
    cout << "\n-----------------------------------\n";

}

bool FindClientByAccountNumber(string AccountNumber, vector <sClient> vClients, sClient& Client)
{

    for (sClient C : vClients)
    {

        if (C.AccountNumber == AccountNumber)
        {
            Client = C;
            return true;
        }

    }
    return false;

}

sClient ChangeClientRecord(string AccountNumber)
{
    sClient Client;

    Client.AccountNumber = AccountNumber;

    cout << "\n\nEnter PinCode? ";
    getline(cin >> ws, Client.PinCode);

    cout << "Enter Name? ";
    getline(cin, Client.Name);

    cout << "Enter Phone? ";
    getline(cin, Client.Phone);

    cout << "Enter AccountBalance? ";
    cin >> Client.AccountBalance;

    return Client;

}

bool MarkClientForDeleteByAccountNumber(string AccountNumber, vector <sClient>& vClients)
{

    for (sClient& C : vClients)
    {

        if (C.AccountNumber == AccountNumber)
        {
            C.MarkForDelete = true;
            return true;
        }

    }

    return false;

}

vector <sClient> SaveCleintsDataToFile(string FileName, vector <sClient> vClients)
{

    fstream MyFile;
    MyFile.open(FileName, ios::out);//overwrite

    string DataLine;

    if (MyFile.is_open())
    {

        for (sClient C : vClients)
        {

            if (C.MarkForDelete == false)
            {
                //we only write records that are not marked for delete.  
                DataLine = ConvertRecordToLine(C);
                MyFile << DataLine << endl;

            }

        }

        MyFile.close();

    }

    return vClients;

}

void AddDataLineToFile(string FileName, string  stDataLine)
{
    fstream MyFile;
    MyFile.open(FileName, ios::out | ios::app);

    if (MyFile.is_open())
    {

        MyFile << stDataLine << endl;

        MyFile.close();
    }

}

void AddNewClient()
{
    sClient Client;
    Client = ReadNewClient();
    AddDataLineToFile(ClientsFileName, ConvertRecordToLine(Client));

}

void AddNewClients()
{
    char AddMore = 'Y';
    do
    {
        //system("cls");
        cout << "Adding New Client:\n\n";

        AddNewClient();
        cout << "\nClient Added Successfully, do you want to add more clients? Y/N? ";


        cin >> AddMore;

    } while (toupper(AddMore) == 'Y');

}

bool DeleteClientByAccountNumber(string AccountNumber, vector <sClient>& vClients)
{

    sClient Client;
    char Answer = 'n';

    if (FindClientByAccountNumber(AccountNumber, vClients, Client))
    {

        PrintClientCard(Client);

        cout << "\n\nAre you sure you want delete this client? y/n ? ";
        cin >> Answer;
        if (Answer == 'y' || Answer == 'Y')
        {
            MarkClientForDeleteByAccountNumber(AccountNumber, vClients);
            SaveCleintsDataToFile(ClientsFileName, vClients);

            //Refresh Clients 
            vClients = LoadCleintsDataFromFile(ClientsFileName);

            cout << "\n\nClient Deleted Successfully.";
            return true;
        }

    }
    else
    {
        cout << "\nClient with Account Number (" << AccountNumber << ") is Not Found!";
        return false;
    }

}

bool UpdateClientByAccountNumber(string AccountNumber, vector <sClient>& vClients)
{

    sClient Client;
    char Answer = 'n';

    if (FindClientByAccountNumber(AccountNumber, vClients, Client))
    {

        PrintClientCard(Client);
        cout << "\n\nAre you sure you want update this client? y/n ? ";
        cin >> Answer;
        if (Answer == 'y' || Answer == 'Y')
        {

            for (sClient& C : vClients)
            {
                if (C.AccountNumber == AccountNumber)
                {
                    C = ChangeClientRecord(AccountNumber);
                    break;
                }

            }

            SaveCleintsDataToFile(ClientsFileName, vClients);

            cout << "\n\nClient Updated Successfully.";
            return true;
        }

    }
    else
    {
        cout << "\nClient with Account Number (" << AccountNumber << ") is Not Found!";
        return false;
    }

}

bool DepositBalanceToClientByAccountNumber(string AccountNumber, double Amount, vector <sClient>& vClients)
{


    char Answer = 'n';


    cout << "\n\nAre you sure you want perfrom this transaction? y/n ? ";
    cin >> Answer;
    if (Answer == 'y' || Answer == 'Y')
    {

        for (sClient& C : vClients)
        {
            if (C.AccountNumber == AccountNumber)
            {
                C.AccountBalance += Amount;
                SaveCleintsDataToFile(ClientsFileName, vClients);
                cout << "\n\nDone Successfully. New balance is: " << C.AccountBalance;

                return true;
            }

        }


        return false;
    }

}

string ReadClientAccountNumber()
{
    string AccountNumber = "";

    cout << "\nPlease enter AccountNumber? ";
    cin >> AccountNumber;
    return AccountNumber;

}

void ShowDeleteClientScreen()
{
    if (CheckAccessPermission(enMainMenuePermissions::pDeleteClient))
    {

    }
    else {
        cout << "\n You don't have premission !!!";
        GoBackToMainMenue();
    }
    cout << "\n-----------------------------------\n";
    cout << "\tDelete Client Screen";
    cout << "\n-----------------------------------\n";

    vector <sClient> vClients = LoadCleintsDataFromFile(ClientsFileName);
    string AccountNumber = ReadClientAccountNumber();
    DeleteClientByAccountNumber(AccountNumber, vClients);

}

void ShowUpdateClientScreen()
{
    if (CheckAccessPermission(enMainMenuePermissions::pUpdateClients))
    {

    }
    else {
        cout << "\n You don't have premission !!!";
        GoBackToMainMenue();
    }
    cout << "\n-----------------------------------\n";
    cout << "\tUpdate Client Info Screen";
    cout << "\n-----------------------------------\n";

    vector <sClient> vClients = LoadCleintsDataFromFile(ClientsFileName);
    string AccountNumber = ReadClientAccountNumber();
    UpdateClientByAccountNumber(AccountNumber, vClients);

}

void ShowAddNewClientsScreen()
{
    if (CheckAccessPermission(enMainMenuePermissions::pAddNewClient))
    {

    }
    else {
        cout << "\n You don't have premission !!!";
        GoBackToMainMenue();
    }
    cout << "\n-----------------------------------\n";
    cout << "\tAdd New Clients Screen";
    cout << "\n-----------------------------------\n";

    AddNewClients();

}

void ShowFindClientScreen()
{
    if (CheckAccessPermission(enMainMenuePermissions::pFindClient))
    {

    }
    else {
        cout << "\n You don't have premission !!!";
        GoBackToMainMenue();
    }
    cout << "\n-----------------------------------\n";
    cout << "\tFind Client Screen";
    cout << "\n-----------------------------------\n";

    vector <sClient> vClients = LoadCleintsDataFromFile(ClientsFileName);
    sClient Client;
    string AccountNumber = ReadClientAccountNumber();
    if (FindClientByAccountNumber(AccountNumber, vClients, Client))
        PrintClientCard(Client);
    else
        cout << "\nClient with Account Number[" << AccountNumber << "] is not found!";

}

void ShowEndScreen()
{
    cout << "\n-----------------------------------\n";
    cout << "\tProgram Ends :-)";
    cout << "\n-----------------------------------\n";

}

void ShowDepositScreen()
{
    cout << "\n-----------------------------------\n";
    cout << "\tDeposit Screen";
    cout << "\n-----------------------------------\n";


    sClient Client;

    vector <sClient> vClients = LoadCleintsDataFromFile(ClientsFileName);
    string AccountNumber = ReadClientAccountNumber();


    while (!FindClientByAccountNumber(AccountNumber, vClients, Client))
    {
        cout << "\nClient with [" << AccountNumber << "] does not exist.\n";
        AccountNumber = ReadClientAccountNumber();
    }


    PrintClientCard(Client);

    double Amount = 0;
    cout << "\nPlease enter deposit amount? ";
    cin >> Amount;

    DepositBalanceToClientByAccountNumber(AccountNumber, Amount, vClients);

}

void ShowWithDrawScreen()
{
    cout << "\n-----------------------------------\n";
    cout << "\tWithdraw Screen";
    cout << "\n-----------------------------------\n";

    sClient Client;

    vector <sClient> vClients = LoadCleintsDataFromFile(ClientsFileName);
    string AccountNumber = ReadClientAccountNumber();


    while (!FindClientByAccountNumber(AccountNumber, vClients, Client))
    {
        cout << "\nClient with [" << AccountNumber << "] does not exist.\n";
        AccountNumber = ReadClientAccountNumber();
    }

    PrintClientCard(Client);

    double Amount = 0;
    cout << "\nPlease enter withdraw amount? ";
    cin >> Amount;

    //Validate that the amount does not exceeds the balance
    while (Amount > Client.AccountBalance)
    {
        cout << "\nAmount Exceeds the balance, you can withdraw up to : " << Client.AccountBalance << endl;
        cout << "Please enter another amount? ";
        cin >> Amount;
    }

    DepositBalanceToClientByAccountNumber(AccountNumber, Amount * -1, vClients);

}

void ShowTotalBalancesScreen()
{

    ShowTotalBalances();

}

string Convert_UserRecordToLine(sUser User,string dilim ="#//#")
{
    string stUserRecord = "";
    stUserRecord += User.UserName + dilim;
    stUserRecord += User.UserPassword + dilim;
    stUserRecord += to_string(User.UserPremssion) ;
    return stUserRecord;
}

void GoBackToMainMenue()
{
    cout << "\n\nPress any key to go back to Main Menue...";
    system("pause>0");
    ShowMainMenue();

}

void GoBackToTransactionsMenue()
{
    cout << "\n\nPress any key to go back to Transactions Menue...";
    system("pause>0");
    ShowTransactionsMenue();

}

short ReadTransactionsMenueOption()
{
    cout << "Choose what do you want to do? [1 to 4]? ";
    short Choice = 0;
    cin >> Choice;

    return Choice;
}

void PerfromTranactionsMenueOption(enTransactionsMenueOptions TransactionMenueOption)
{
    switch (TransactionMenueOption)
    {
    case enTransactionsMenueOptions::eDeposit:
    {
        system("cls");
        ShowDepositScreen();
        GoBackToTransactionsMenue();
        break;
    }

    case enTransactionsMenueOptions::eWithdraw:
    {
        system("cls");
        ShowWithDrawScreen();
        GoBackToTransactionsMenue();
        break;
    }


    case enTransactionsMenueOptions::eShowTotalBalance:
    {
        system("cls");
        ShowTotalBalancesScreen();
        GoBackToTransactionsMenue();
        break;
    }


    case enTransactionsMenueOptions::eShowMainMenue:
    {

        ShowMainMenue();

    }
    }

}

void ShowTransactionsMenue()
{
    if (CheckAccessPermission(enMainMenuePermissions::pTranactions))
    {

    }
    else {
        cout << "\n You don't have premission !!!";
        GoBackToMainMenue();
    }
    system("cls");
    cout << "===========================================\n";
    cout << "\t\tTransactions Menue Screen\n";
    cout << "===========================================\n";
    cout << "\t[1] Deposit.\n";
    cout << "\t[2] Withdraw.\n";
    cout << "\t[3] Total Balances.\n";
    cout << "\t[4] Main Menue.\n";
    cout << "===========================================\n";
    PerfromTranactionsMenueOption((enTransactionsMenueOptions)ReadTransactionsMenueOption());
}

short ReadMainMenueOption()
{
    cout << "Choose what do you want to do? [1 to 8]? ";
    short Choice = 0;
    cin >> Choice;

    return Choice;
}

short ReadManageUsersOption()
{
    cout << "Choose what do you want to do? [1 to 6]? ";
    short Choice = 0;
    cin >> Choice;
    return Choice;
}

void ManageUsersScreen();

void GoBackToManageuserScreen()
{
    cout << "\n\nPress any key to go back to Manage Menue...";
    system("pause>0");
    ManageUsersScreen();
}

vector<sUser> LoadUsersDateFromfileToVector()
{
    vector<sUser> vUsers;
    fstream UserFile;
    UserFile.open(UsersFileName, ios::in);
    if (UserFile.is_open())
    {
        string Line;
        sUser User;
        while (getline(UserFile, Line))
        {
            User = ConvertlineToRecord_Users(Line);
            vUsers.push_back(User);
        }
        UserFile.close();
    }
    return vUsers;
}

void PrintUsersRecordLine(sUser User)
{
    cout << "| " << left << setw(15) << User.UserName;
    cout << "| " << left << setw(10) << User.UserPassword;
    cout << "| " << left << setw(40) << User.UserPremssion;
}

void ShowUsersList()
{
    vector<sUser> vUsers = LoadUsersDateFromfileToVector();
    cout << "\n\t\t\t\t\tUsers List (" << vUsers.size() << ") User(s).";
    cout << "\n_______________________________________________________";
    cout << "_________________________________________\n" << endl;

    cout << "| " << left << setw(15) << "Username ";
    cout << "| " << left << setw(10) << "Password ";
    cout << "| " << left << setw(40) << "permissions ";

    cout << "\n_______________________________________________________";
    cout << "_________________________________________\n" << endl;
    if (vUsers.size() == 0)
    {
        cout << "\t\t\t\tNo users available in the system !\n";
    }
    else 
    {
        for (sUser User : vUsers)
        {
            PrintUsersRecordLine(User);
            cout << endl;
        }
    }
    cout << "\n_______________________________________________________";
    cout << "_________________________________________\n" << endl;
}

int ReadUserPremissions()
{
    int Permissions = 0;
    string Answer;

    cout << "\nDo you want to give full access? y/n? ";
    getline(cin >> ws, Answer);
    if (Answer == "y" || Answer == "Y")
        return -1;


    cout << "\nShow Clients list ? y/n : ";
    getline(cin >> ws, Answer);
    if (Answer == "y" || Answer == "Y") {
        Permissions += enMainMenuePermissions::pListClients;
    }
    
    cout<<"\nAdd New Client? y/n? "; 
    getline(cin >> ws, Answer);

    if (Answer == "y" || Answer == "Y") {
        Permissions += enMainMenuePermissions::pAddNewClient;
    }

    cout << "\nDelete Client? y/n? ";
    getline(cin >> ws, Answer);
    if (Answer == "y" || Answer == "Y") {
        Permissions += enMainMenuePermissions::pDeleteClient;
    }


    cout << "\nUpdate Client? y/n? ";
    getline(cin >> ws, Answer);
    if (Answer == "y" || Answer == "Y") {
        Permissions += enMainMenuePermissions::pUpdateClients;
    }



    cout << "\nFind Client? y/n? ";
    getline(cin >> ws, Answer);
    if (Answer == "y" || Answer == "Y") {
        Permissions += enMainMenuePermissions::pFindClient;
    }


    cout << "\nTransactions? y/n? ";
    getline(cin >> ws, Answer);
    if (Answer == "y" || Answer == "Y") {
        Permissions += enMainMenuePermissions::pTranactions;
    }

    cout << "\nManage Users? y/n? ";
    getline(cin >> ws, Answer);
    if (Answer == "y" || Answer == "Y") {
        Permissions += enMainMenuePermissions::pManageUsers;
    }


    return Permissions;


}

bool IsUserAlredyExsist(string Username)
{
    
    bool IsAlredyExcsist = false;
    vector<sUser> Users;
    fstream Myfile;
    Myfile.open(UsersFileName, ios::in);
    if (Myfile.is_open())
    {
        sUser User;
        string line = "";
        while (getline(Myfile, line))
        {
            User = ConvertlineToRecord_Users(line);
            if (User.UserName == Username)
            {
                IsAlredyExcsist= true;
                break;
            }
        }
        Myfile.close();
    }
        if (IsAlredyExcsist)
            cout << "\nThis username is already taken Please Enter another username : \n";
       
        return IsAlredyExcsist;
}

sUser GetNewUserInformation()
{
    sUser NewUser;
    bool IsAlredyExcsist = "false";

    do
    {
        cout << "\n\tEnter User Name       : ";
        getline(cin >> ws, NewUser.UserName);
        IsAlredyExcsist = IsUserAlredyExsist(NewUser.UserName);

    } while (IsAlredyExcsist);


    cout << "\n\tEnter Ueser Password  :";
    getline(cin >> ws, NewUser.UserPassword);

    NewUser.UserPremssion = ReadUserPremissions();

    
    return NewUser;
}

void AddUserDataLineToFile(string FileName, string DataLine)
{
    fstream MyFile;
    MyFile.open(FileName, ios::out | ios::app);
    if (MyFile.is_open())
    {
        MyFile << DataLine << endl;
        MyFile.close();
    }
}

void AddNewUsers()
{
        string AddMore = "y";
    do {

        system("cls");

        sUser NewUser;

        cout << "\n-----------------------------------------\n";
        cout << "\t\tAdding New User Screen\n";
        cout << "\n-----------------------------------------\n";

        NewUser = GetNewUserInformation();

        AddUserDataLineToFile(UsersFileName, Convert_UserRecordToLine(NewUser));


        cout << "\n\nDo you want to add more users y/n : ";

        getline(cin >> ws, AddMore);

    } while (AddMore == "y" || AddMore == "Y" || AddMore == "yes" || AddMore == "Yes");
}

bool FindUserByUserName(string UserName, vector<sUser> Users, sUser &User)
{
    for (sUser &u : Users)
    {
        if (u.UserName == UserName)
        {
            User = u;
            return true;
        }
    }
    return false;
}

void PrintUserCard(sUser User)
{
    cout << "\nThe following are the client details:\n";
    cout << "-----------------------------------";
    cout << "\nUserName   : " << User.UserName;
    cout << "\nPassword   : " << User.UserPassword;
    cout << "\nPremission : " << User.UserPremssion;
    cout << "\n-----------------------------------\n";
}

void MarkUserForDelete(string UserName,vector<sUser> &Users)
{
    for (sUser &u : Users)
    {

        if (u.UserName == UserName)
        {
            u.MarkForDelete = true;
        }

    }

}

void SaveUsersDataToFile(vector<sUser> &Users)
{
    fstream MyFile;
    MyFile.open(UsersFileName, ios::out);//overwrite

    string DataLine;

    if (MyFile.is_open())
    {

        for (sUser u :Users)
        {

            if (u.MarkForDelete == false)
            {
                //we only write records that are not marked for delete.  
                DataLine = Convert_UserRecordToLine(u);
                MyFile << DataLine << endl;

            }

        }

        MyFile.close();

    }
}

bool DeleteUserByUserName(string UserName, vector<sUser> Users)
{
    sUser User;
    char Answer = 'n';
    if (FindUserByUserName(UserName, Users, User))
    {

        PrintUserCard(User);

        cout << "\n\nAre you sure you want delete this User? y/n ? ";
        cin >> Answer;
        if (Answer == 'y' || Answer == 'Y')
        {
            MarkUserForDelete(UserName,Users);
            SaveUsersDataToFile(Users);
            cout << "\n\nUser Deleted Successfully.";
            return true;
        }
    }
    else
    {
        cout << "\nUser with User Name (" << UserName << ") is Not Found!";
        return false;
    }


}

void ShowDeleteUserScreen()
{
    cout << "\n-----------------------------------\n";
    cout << "\tDelete User Screen";
    cout << "\n-----------------------------------\n";

    vector <sUser> Users = LoadUsersDateFromfileToVector();
    string UserName;
    cout << "\nEnter Account Number ";
    getline(cin >> ws, UserName);
    DeleteUserByUserName(UserName, Users);

}

sUser UserNewRecord(string UserName)
{
    sUser User;
    User.UserName = UserName;

    cout << "\nEnter Youre New Pasword";
    getline(cin >> ws, User.UserPassword);
    User.UserPremssion = ReadUserPremissions();
    return User;
}

void Updateuserinformationbyusername(string UserName,vector<sUser> Users)
{
    sUser User;
    char Answer = 'n';
    if (FindUserByUserName(UserName, Users, User)) 
    {
        PrintUserCard(User);
        cout << "\n\nAre you sure you want to update this user ?y/n ?  : ";
        cin >> Answer;
        if (Answer == 'y' || Answer == 'Y')
        {
            for (sUser &u : Users)
            {
                if (u.UserName == UserName)
                {
                    u = UserNewRecord(UserName);
                    break;
                }
            }
            SaveUsersDataToFile(Users);
            cout << "\n\nUser Updated Successfully. ";
            
        }


    }
    else
    {
        cout << "\n\nUser With User Name (" << UserName << ") Is Not Found !! ";
    }

}

void UpdateUserInformation()
{
    cout << "\n-----------------------------------\n";
    cout << "\tUpdate User Info Screen";
    cout << "\n-----------------------------------\n";


    vector<sUser> Users = LoadUsersDateFromfileToVector();
    string UserName;
    cout << "\nEntere the User Name for the user you want to update his info ";
    getline(cin >> ws, UserName);
    Updateuserinformationbyusername(UserName, Users);

}

void FindUser()
{
    vector<sUser> Users = LoadUsersDateFromfileToVector();
    string UserName;
    sUser User;
    cout << "\nEntere the UserName for the user you want to see his info  :  ";
    getline(cin >> ws, UserName);

    if (FindUserByUserName(UserName, Users, User))
    {
        cout << endl;
        PrintUserCard(User);
        cout << endl;

    }

}

void PreformManageUsersMenue(enManageUsersOptions ManageUserOption)
{
    switch (ManageUserOption)
    {
    case eListusers:
    {
        system("cls");
        ShowUsersList();
        GoBackToManageuserScreen();
        break;
    }//--------------------------------------------------------------------------------------11111
    case eAddNewUser:
    {
        system("cls");
        AddNewUsers();//error Here
        GoBackToManageuserScreen();
        break;
    }//--------------------------------------------------------------------------------------22222
    case eDeleteuser:
    {
        system("cls");
        ShowDeleteUserScreen();
        GoBackToManageuserScreen();
        break;
    }//--------------------------------------------------------------------------------------33333
    case eUpdateUser:
    {
        system("cls");
        UpdateUserInformation();
        GoBackToManageuserScreen();
        break;
    }//--------------------------------------------------------------------------------------44444
    case efindUser:
    {
        system("cls");
        FindUser();
        GoBackToManageuserScreen();
        break;
    }//--------------------------------------------------------------------------------------55555
    case eMainMenue:
    {
        system("cls");
        ShowMainMenue();
        break;
    }//--------------------------------------------------------------------------------------66666
    }
}

void ManageUsersScreen()
{
    if (CheckAccessPermission(enMainMenuePermissions::pManageUsers))
    {

    }
    else {
        cout << "\n You don't have premission !!!";
        GoBackToMainMenue();
    }
    system("cls");
    cout << "===========================================\n";
    cout << "\t\tManage Users Screen \n";
    cout << "===========================================\n";
    cout << "\t[1] list Users.\n";
    cout << "\t[2] Add New User .\n";
    cout << "\t[3] Delete User.\n";
    cout << "\t[4] Update User.\n";
    cout << "\t[5] Find User.\n";
    cout << "\t[6] Main Menue.\n";
    cout << "===========================================\n";
    PreformManageUsersMenue((enManageUsersOptions)ReadManageUsersOption());
}

void LogInScreen();

void PerfromMainMenueOption(enMainMenueOptions MainMenueOption)
{

    switch (MainMenueOption)
    {
    case enMainMenueOptions::eListClients:
    {
        system("cls");
        ShowAllClientsScreen();
        GoBackToMainMenue();
        break;
    }
    case enMainMenueOptions::eAddNewClient:
    {
        system("cls");
        ShowAddNewClientsScreen();
        GoBackToMainMenue();
        break;
    }
    case enMainMenueOptions::eDeleteClient:
    {
        system("cls");
        ShowDeleteClientScreen();
        GoBackToMainMenue();
        break;
    }
    case enMainMenueOptions::eUpdateClient:
    {
        system("cls");
        ShowUpdateClientScreen();
        GoBackToMainMenue();
        break;
    }
    case enMainMenueOptions::eFindClient:
    {
        system("cls");
        ShowFindClientScreen();
        GoBackToMainMenue();
        break;
    }
    case enMainMenueOptions::eShowTransactionsMenue:
    {
        system("cls");
        ShowTransactionsMenue();
        break;
    }
    case enMainMenueOptions::eManageUsers:
    {
        system("cls");
        ManageUsersScreen();
        break;
    }
    case enMainMenueOptions::eLogOut:
    {

        system("cls");

        LogInScreen();
        break;
    }

    }

}

void ShowMainMenue()
{
    system("cls");
    cout << "===========================================\n";
    cout << "\t\tMain Menue Screen\n";
    cout << "===========================================\n";
    cout << "\t[1] Show Client List.\n";
    cout << "\t[2] Add New Client.\n";
    cout << "\t[3] Delete Client.\n";
    cout << "\t[4] Update Client Info.\n";
    cout << "\t[5] Find Client.\n";
    cout << "\t[6] Transactions.\n";
    cout << "\t[7] Manage Users.\n";
    cout << "\t[8] Log Out.\n";
    cout << "===========================================\n";
    PerfromMainMenueOption((enMainMenueOptions)ReadMainMenueOption());
}

bool FindUserBy_Username_Password(string UserName,string Password)
{
    sUser User;
    fstream UserFile;
    UserFile.open(UsersFileName, ios::in);//read Mode

    if (UserFile.is_open())
    {

        string line = "";
        while (getline(UserFile, line))
        {
            User= ConvertlineToRecord_Users(line);
            if ((User.UserName == UserName)&&(User.UserPassword==Password))
            {
                CurrentUser = User;
                return true;           
            }
        }
    }
    return false;
}

bool LoudUserInfo(string UserName, string password)
{
    if (FindUserBy_Username_Password(UserName, password))
    {
        return true;
    }
    else return false;
}

void LogInScreen()
{
    bool LoginFiled = false;
    string UserName, password;
    do {
        system("cls");
        cout << "===========================================\n";
        cout << "\t\ Login  Screen\n";
        cout << "===========================================\n";
        if (LoginFiled)
        {
            cout << "invalid Username/password ! \n";
        }
        cout << "Enter Username     : \n";
        getline(cin >> ws, UserName);
        cout << "Enter The Password : \n";
        getline(cin >> ws, password);
        LoginFiled = !(LoudUserInfo(UserName, password));
    } while (LoginFiled);
    ShowMainMenue();
}

int main()
{
    LogInScreen();
    system("pause>0");
    return 0;
}