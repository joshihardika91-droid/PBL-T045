#include<iostream>
#include<string>
#include"admin.h"

using namespace std;



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

 






void Admin::adminMenu(){

    int choice;

    do
    {
        cout << "\n-------ADMIN MENU-------" << endl;
        cout << "1. Manage Attendance" << endl;
        cout << "2. Manage Fee" << endl;
        cout << "3. Manage Leave" << endl;
        cout << "4. Manage Complaints" << endl;
        cout << "5. Logout" << endl;

        cout << "Enter choice: ";
        cin >> choice;

        switch (choice)
        {
            case 1:
                
                break;

            case 2:
                feesystem.AdminfeeDashboard();
                break;

            case 3:
                
                leaveManager.adminMenu();
                break;

            case 4:
                
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







