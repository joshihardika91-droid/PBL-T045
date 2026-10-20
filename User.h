#ifndef USER_H
#define USER_H

#include <iostream>
#include <string>
#define MAX 100
using namespace std;

class User
{
 private:
     string UserId;
     string Name;
     string Password;
     string Role;
     string Status;

     static User  *users;
     static int userCount ;
 public:
    void Register();
    string Login();
    void ChangePassword();
    void ViewPendingUsers();
    string getName(){return Name;}
    string getUserId(){return UserId;}
    string getRole(){return Role;}
    string getStatus(){return Status;}
};

#endif