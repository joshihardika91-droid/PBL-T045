#include<iostream>
#include<unordered_map>
#include<string>
#include"admin.h"
#include"User.h"
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

void adminMenu(){
    int choice;
    do{
        cout<<"\n-------ADMIN MENU-------"<<endl;
        cout<<"1. Manage Attendance"<<endl;
        cout<<"2. Manage Mess Menu"<<endl;
        cout<<"3. Approve Leave"<<endl;
        cout<<"4. View Complaints"<<endl;
        cout<<"5. Logout"<<endl;

        cout<<"Enter choice: ";
        cin>>choice;

        switch(choice){
            case 1:
                cout<<"Attendance Management"<<endl;
                break;
            case 2:
                cout<<"Mess Menu Management"<< endl;
                break;
            case 3:
                cout<<"Leave Approval"<<endl;
                break;
            case 4:
                cout<<"Complaint Management"<<endl;
                break;
            case 5:
                cout<<"Logged Out Successfully!!"<<endl;
                break;
            default:
                cout<<"Invalid Choice!!"<<endl;
        }

    } while(choice != 5);
}








