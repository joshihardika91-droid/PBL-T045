#ifndef ADMIN_H
#define ADMIN_H
#include<iostream>
using namespace std;
class Admin
{
    private:
            string password;
            string user;

            const string admin_user= "ADMIN";
            const string admin_pass= "Hosttel@123";
    public :int login();};
#endif 