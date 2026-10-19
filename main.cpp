#include <iostream>
#include <string>
#include "User.h"
#include "admin.h"
#include "student.h"

using namespace std;

int main()
{
    User user;
    string role = user.Login();

    if (role == "admin")
    {
        Admin admin;
        admin.adminMenu();
    }
    else if (role == "student")
    {
        Student student;
        student.StudentMenu();
    }
    else if (role == "warden")
    {
        cout << "Warden menu is not connected yet." << endl;
    }

    return 0;
}