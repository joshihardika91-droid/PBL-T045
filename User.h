#ifndef USER_H
#define USER_H

#include <iostream>
#include <string>
using namespace std;

class User
{
 private:
     string UserId;
     string Name;
     string Password;
     string Role;

     static User  *users;
     static int userCount ;
 public:
    void Register();
    string Login();
    void ChangePassword();

};

#endif