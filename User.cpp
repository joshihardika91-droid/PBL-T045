#include<iostream>
#include<string>
using namespace std;

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
    
