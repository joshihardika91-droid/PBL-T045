#include <iostream>
#include <fstream>
#include <string>
#include "User.h"
#include"student.h"
#include "db.h"
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

    vector<vector<string> > rows;
    string sql = "SELECT user_id, name, password, role, status FROM users ORDER BY id";

    if (!dbQuery(sql, rows))
    {
        return;
    }

    int count = rows.size();
    if (count == 0)
    {
        return;
    }

    users = new User[count];

    for (int i = 0; i < count; i++)
    {
        users[i].UserId = rows[i][0];
        users[i].Name = rows[i][1];
        users[i].Password = rows[i][2];
        users[i].Role = rows[i][3];
        users[i].Status = rows[i][4];
        userCount++;
    }
}

void User::saveUsers()
{
    dbBegin();

    for (int i = 0; i < userCount; i++)
    {
        string sql = "INSERT INTO users (user_id, name, password, role, status) VALUES ('"
                     + dbEscape(users[i].UserId) + "', '"
                     + dbEscape(users[i].Name) + "', '"
                     + dbEscape(users[i].Password) + "', '"
                     + dbEscape(users[i].Role) + "', '"
                     + dbEscape(users[i].Status) + "') "
                     "ON CONFLICT(user_id) DO UPDATE SET name = excluded.name, password = excluded.password, "
                     "role = excluded.role, status = excluded.status";

        if (!dbExecute(sql))
        {
            dbRollback();
            return;
        }
    }

    dbCommit();
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
                    string course;

                cout << "Enter student's course: ";
                cin.ignore();
                getline(cin, course);

                Student student;
                student.SetStudentDetails(stoi(id), users[i].Name);
                student.SetCourse(course);

                if (student.SaveStudentDetails())
                {
                    users[i].Status = "Approved";
                    saveUsers();
                    cout << "Student approved successfully.\n";
                }
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