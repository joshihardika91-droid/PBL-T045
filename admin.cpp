#include<iostream>
#include<string>
using namespace std;


class Admin
{
private:
    string username;
    string password;

public:
    Admin()
    {
        username = "ADMIN";
        password = "hostel@123";
    }

    int login()
    {
        string user, pass;

        cout << "Enter Username: ";
        cin >> user;

        cout << "Enter Password: ";
        cin >> pass;

        if(user == username && pass == password)
        return(1);
        else
        return(0);
    }
};

class Attendance
{
private:
    string StudentId;
    string Name;
    string Date;
    string Status;
void Attendancemark()
{
    cout << "Enter student id: " << endl;
    cin >> StudentId;
    cout << "Enter student name: " << endl;
    cin.ignore();
    getline(cin,Name);
    cout << "Enter present date: " << endl;
    cin >> Date;
    cout << "Enter status (Present/Absent): " << endl;
    cin >> Status;
    cout << "Attendance marked successfully! " << endl;
}
};


int main()
{
    Admin admin;

    if(admin.login())
    {
        cout << "Login Successful!" << endl;
        
        Attendance at;
        at.Attendancemark();
    }
    else
    {
        cout << "Invalid Credentials!" << endl;
    }

    return 0;
}
