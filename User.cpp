#include <iostream>
#include <string>
#include "User.h"
using namespace std;
User* User::users=nullptr;
int  User::userCount=0;


    
        void User::Register()
        {
            string id;
            cout << "Enter student id: " << endl;
            cin >> id;

            int exists = 0;
            for(int i = 0; i < userCount; i++)
            {
                if(users[i].UserId == id)
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
        {    User newUser;
            newUser.UserId = id;
            
            cout << "Enter name: " << endl;
            
            getline(cin>>ws, newUser.Name);
            
        

            cout << "Enter password: " << endl;
            cin >> newUser.Password;

            cout << "Enter role (admin/warden/student): " << endl;
            cin >> newUser.Role;
            if(newUser.Role != "admin" &&
   newUser.Role != "warden" &&
   newUser.Role != "student")
{
    cout << "Invalid role!" << endl;
    return;
}
            User *temp = new User[userCount + 1];

    for(int i = 0; i < userCount; i++)
    {
        temp[i] = users[i];
    }

    temp[userCount] = newUser;

    delete[] users;
    users = temp;

        cout << "Registration successful!" << endl;
        userCount++;
    }
}

 void User::ChangePassword()
    {
        string id,oldpass,newpass;
        cout<<"Enter ID"<<endl;
        cin>>id;
        for(int i=0;i<userCount;i++){
        if(users[i].UserId==id)
        {  
             cout<<"Enter Old Password"<<endl;
            cin>>oldpass;
            if(oldpass==users[i].Password){
            cout<<"Enter New Password"<<endl;
            cin>>newpass;

            users[i].Password=newpass;
            cout<<"Password is successfully updated!"<<endl;}
        

        
        else
        {
            cout<<"Wrong old Password"<<endl;
        }
        return;}}
    
   cout<<"User ID could not be found";}


   string User::Login()
    {
        string id, pass;

        cout << "Enter user ID: " << endl;
        cin >> id;

        cout << "Enter password: " << endl;
        cin >> pass;

        for(int i = 0; i < userCount; i++)
        {
            if(users[i].UserId == id &&
               users[i].Password == pass)
            {
                cout << "Login successful!" << endl;
                return users[i].Role;
            }
        }

        cout << "Invalid credentials!" << endl;
        return "";
    }
    
