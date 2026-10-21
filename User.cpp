#include <iostream>
#include <fstream>
#include <string>
#include "User.h"
using namespace std;

User *User::users = NULL;
int User::userCount = 0;

void User::loadUsers()
{
    if (users != NULL)
    {
        delete[] users;
        users = NULL;
    }
    userCount = 0;

    ifstream fin("users.txt");
    if (!fin)
    {
        return;
    }
    string line;
    int count = 0;
    while (getline(fin, line))
    {
        if (line != "")
        {
            count++;
        }
    }
    fin.close();

    if (count == 0)
    {
        return;
    }
    users = new User[count];

    ifstream fin2("users.txt");
    while (getline(fin2, line) && userCount < count)
    {
        if (line == "")
        {
            continue;
        }
        string parts[5];
        int p = 0;
        for (int i = 0; i < line.length(); i++)
        {
            if (line[i] == '|')
            {
                p++;
                if (p > 4)
                {
                    break;
                }
            }
            else
            {
                parts[p] += line[i];
            }
        }
        users[userCount].UserId = parts[0];
        users[userCount].Name = parts[1];
        users[userCount].Password = parts[2];
        users[userCount].Role = parts[3];
        users[userCount].Status = parts[4];
        userCount++;
    }
    fin2.close();
}

void User::saveUsers()
{
    ofstream fout("users.txt");
    for (int i = 0; i < userCount; i++)
    {
        fout << users[i].UserId << "|" << users[i].Name << "|" << users[i].Password << "|"
             << users[i].Role << "|" << users[i].Status << endl;
    }
    fout.close();
}

void User::Register()
{
    loadUsers();

    string id, name, pass;
    cout << "Enter Student ID: ";
    cin >> id;

    for (int i = 0; i < userCount; i++)
    {
        if (users[i].UserId == id)
        {
            cout << "User already exists!" << endl;
            return;
        }
    }

    cout << "Enter Name: ";
    cin.ignore();
    getline(cin, name);
    cout << "Enter Password: ";
    cin >> pass;

    User *temp = new User[userCount + 1];
    for (int i = 0; i < userCount; i++)
    {
        temp[i] = users[i];
    }
    temp[userCount].UserId = id;
    temp[userCount].Name = name;
    temp[userCount].Password = pass;
    temp[userCount].Role = "student";
    temp[userCount].Status = "Pending";

    delete[] users;
    users = temp;
    userCount++;

    saveUsers();
    cout << "Registered successfully. Wait for admin approval." << endl;
}

string User::Login()
{
    loadUsers();

    string id, pass;
    cout << "Enter User ID: ";
    cin >> id;
    cout << "Enter Password: ";
    cin >> pass;

    for (int i = 0; i < userCount; i++)
    {
        if (users[i].UserId == id && users[i].Password == pass)
        {if (users[i].Role == "student" && users[i].Status != "Approved")
        {
            cout << "Your account is not approved yet." << endl;
            return "";}
            UserId = users[i].UserId;
            Name = users[i].Name;
            Role = users[i].Role;
            Status = users[i].Status;
            return users[i].Role;
        }
    }
    cout << "Invalid ID or password!" << endl;
    return "";
}

void User::ChangePassword()
{
    loadUsers();

    string id, oldPass, newPass;
    cout << "Enter User ID: ";
    cin >> id;

    for (int i = 0; i < userCount; i++)
    {
        if (users[i].UserId == id)
        {
            cout << "Enter old password: ";
            cin >> oldPass;
            if (users[i].Password == oldPass)
            {
                cout << "Enter new password: ";
                cin >> newPass;
                users[i].Password = newPass;
                saveUsers();
                cout << "Password changed successfully." << endl;
            }
            else
            {
                cout << "Wrong old password!" << endl;
            }
            return;
        }
    }
    cout << "User not found!" << endl;
}

void User::ViewPendingUsers()
{
    loadUsers();

    bool found = false;
    for (int i = 0; i < userCount; i++)
    {
        if (users[i].Status == "Pending")
        {
            cout << users[i].UserId << "  " << users[i].Name << endl;
            found = true;
        }
    }
    if (!found)
    {
        cout << "No pending users." << endl;
    }
}
void User::UpdateStatus()
{
    loadUsers();

    bool found = false;

    for (int i = 0; i < userCount; i++)
    {
        if (users[i].Role == "student" &&
            users[i].Status == "Pending")
        {
            found = true;
            break;
        }
    }

    if (!found)
    {
        cout << "No pending students to approve or reject.\n";
        return;
    }

    ViewPendingUsers();

    string id;
    int choice;

    cout << "\nEnter Student ID to process: ";
    cin >> id;

    for (int i = 0; i < userCount; i++)
    {
        if (users[i].UserId == id &&
            users[i].Role == "student" &&
            users[i].Status == "Pending")
        {
            cout << "1. Approve Student\n";
            cout << "2. Reject Student\n";
            cout << "Enter choice: ";
            cin >> choice;

            if (choice == 1)
            {
                users[i].Status = "Approved";
                saveUsers();
                cout << "Student approved successfully.\n";
            }
            else if (choice == 2)
            {
                users[i].Status = "Rejected";
                saveUsers();
                cout << "Student rejected.\n";
            }
            else
            {
                cout << "Invalid choice. No changes made.\n";
            }

            return;
        }
    }

    cout << "Student not found in pending registrations.\n";
}