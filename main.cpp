
#include <iostream>
#include "User.h"
#include "admin.h"

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
        cout << "Student menu is not connected yet." << endl;
    }
    else if (role == "warden")
    {
        cout << "Warden menu is not connected yet." << endl;
    }

    return 0;
}
