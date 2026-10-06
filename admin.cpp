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
        return 1;
        else
        return 0;
        }
        int Authorise(){
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

    void ChangePassword()
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


void studentMenu()
{
    int choice;
    do{
        cout<<"\n-------STUDENT MENU-------"<<endl;
        cout<<"1. View Attendance"<<endl;
        cout<<"2. View Mess Menu"<<endl;
        cout<<"3. Apply for Leave"<<endl;
        cout<<"4. Register Complaint"<<endl;
        cout<<"5. Logout"<<endl;

        cout<<"Enter choice: ";
        cin>>choice;

        switch(choice)
        {
            case 1:
                cout<<"Attendance Section"<<endl;
                break;
            case 2:
                cout<<"Mess Menu Section"<<endl;
                break;
            case 3:
                cout<<"Leave Application Section"<<endl;
                break;
            case 4:
                cout<<"Complaint Section"<< endl;
                break;
            case 5:
                cout<<"Logged Out Successfully!"<< endl;
                break;
            default:
                cout<<"Invalid Choice!!"<< endl;
        }
    }while(choice!= 5);
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

int main()
{
    Admin admin;

         if(admin.login()==1)
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





