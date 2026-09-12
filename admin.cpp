#include<iostream>
#include<string>
using namespace std;

class Admin
{
    private:
    string password;
    string user;

    const string admin_user= "ADMIN";
    const string admin_pass= "Hosttel@123";
    
    public:
    int login()
    {
    cout<<"------HOSTEL MANAGEMENT SYSTEM------"<<endl;
    cout<<"Admin login:"<<endl;

    cout<<"Enter Admin Username: ";
    cin>>user;
    cout<<"Enter Admin Password: ";
    cin>>password;

    if(user==admin_user && password==admin_pass)
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
public:
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