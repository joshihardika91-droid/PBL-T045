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






class User
{
    private:
     string StudentId;
     string Name;
     string Password;
     string Role;

     User  *users;
     int userCount = 0;
    public:
        void Register()
        {
            string id;
            cout << "Enter student id: " << endl;
            cin >> id;

            int exists = 0;
            for(int i = 0; i < userCount; i++)
            {
                if(users[i].StudentId == id)
                {
                    exists = 1;
                    break;
                }
            }
            if(exists == 1)
            {
                cout << "This Id is already registered!" << endl;
            }
        else
        {
            users[userCount].StudentId = id;
            
            cout << "Enter name: " << endl;
            cin.ignore();
            getline(cin>>ws, users[userCount].Name);
            
            users[userCount].StudentId = id;

            cout << "Enter password: " << endl;
            cin >> users[userCount].Password;

            cout << "Enter role (admin/student): " << endl;
            cin >> users[userCount].Role;

        cout << "Registration successful!" << endl;
        userCount++;
    }
}

 void ChangePassword()
    {
        string oldpass,newpass;
        cout<<"Enter old Password"<<endl;
        cin>>oldpass;

        if(oldpass == Password)
        {
            cout<<"Enter New Password"<<endl;
            cin>>newpass;

            Password=newpass;
            cout<<"Password is successfully updated!"<<endl;
        

        }
        else
        {
            cout<<"Wrong old Password"<<endl;
        }
    }

};
    
