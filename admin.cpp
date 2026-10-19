#include<iostream>
#include<unordered_map>
#include<string>
#include"admin.h"
#include"User.h"
#include"attendance.h"
using namespace std;
#include "complaint.h"
#include "leave.h"
ComplaintManagement complaintSystem;
LeaveManager leaveManager;

     int Admin:: login()
        {
        cout<<"------HOSTEL MANAGEMENT SYSTEM------"<<endl;
        cout<<"Admin login:"<<endl;

        cout<<"Enter Admin Username: ";
        cin>>user;
        cout<<"Enter Admin Password: ";
        cin>>password;

        if(user==admin_user && password==admin_pass)
        return 1;
       else
        return 0;
        }

        
        int Admin::  Authorise(){
            string checkname,checkpass;
            cout<<"Enter User name for verification";
            cin>>checkname;
            cout<<"Enter User name for verification";
            cin>>checkpass;
            if(checkname==admin_user&&admin_pass==checkpass){
                return 1;
            }
            cout<<"Not Authorized";
            return 0;

        }

    void Admin :: ChangePassword()
    {
        string oldpass,newpass;
        cout<<"Enter old Password"<<endl;
        cin>>oldpass;

        if(oldpass == password)
        {
            cout<<"Enter New Password"<<endl;
            cin>>newpass;

            password=newpass;
            cout<<"Password is successfully updated!"<<endl;
        

        }
        else
        {
            cout<<"Wrong old Password"<<endl;
        }
    }

 


unordered_map<string, string>attendanceTable;
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
        attendanceTable[StudentId] = Status;
        cout << "Attendance marked successfully! " << endl;
    }
    
};




void Admin::adminMenu(){

    int choice;
    Attendance at;

    do
    {
        cout << "\n-------ADMIN MENU-------" << endl;
        cout << "1. Manage Attendance" << endl;
        cout << "2. Manage Mess Menu" << endl;
        cout << "3. Approve Leave" << endl;
        cout << "4. View Complaint Menu" << endl;
        cout << "5. Logout" << endl;

        cout << "Enter choice: ";
        cin >> choice;

        switch (choice)
        {
            case 1:
                at.Attendancemark();
                break;

            case 2:
                cout << "Mess Menu Management" << endl;
                break;

            case 3:
                cout << "\nAdmin Leave menu" << endl;
                leaveManager.adminMenu();
                break;

            case 4:
                cout << "\nAdmin Complaint menu" << endl;
               complaintSystem.AdminComplaintMenu();
               break;

            case 5:
                cout << "Logged Out Successfully!" << endl;
                break;

            default:
                cout << "Invalid Choice!" << endl;
        }

    } while (choice != 5);
}







