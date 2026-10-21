
#include <iostream>
#include <string>
#include "User.h"
#include "admin.h"
#include "student.h"

using namespace std;

int main()
{
    int choice;

    while (true)
    {
        cout << "\n----WELCOME TO HOSTELMATE------\n";
        cout << "1. Student Registration\n";
        cout << "2. Student Login\n";
        cout << "3. Admin Login\n";
        cout << "4. Exit\n";
        cout << "Enter choice: ";
        cin >> choice;

        if (choice == 1)
        {
            User user;
            user.Register();
        }
        else if (choice == 2)
        {
            User user;
            string role = user.Login();

            if (role == "student")
            {
                Student student;
                int s_id = stoi(user.getUserId());

                student.SetStudentDetails(
                    s_id, user.getName()
                );

                student.StudentMenu(s_id);
            }
        }
        else if (choice == 3)
        {
            Admin admin;

            if (admin.login() == 1)
            {
                cout << "Admin login successful!\n";
                admin.adminMenu();
            }
            else
            {
                cout << "Invalid admin username or password!\n";
            }
        }
        else if (choice == 4)
        {
            cout << "Exiting Hostel-Mate.\n";
            break;
        }
        else
        {
            cout << "Invalid choice!\n";
        }
    }

    return 0;
}
