#ifndef USER_H
#define USER_H

#include <iostream>
#include <string>
using namespace std;

class User
{
protected:
    string Password;

public:
    void ChangePassword();
};

#endif