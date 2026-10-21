
#include <iostream>
#include <string>
#include "admin.h"

using namespace std;

int Admin::login()
{
    cout << "\n------ HOSTEL MANAGEMENT SYSTEM ------\n";
    cout << "Admin Login\n";

    cout << "Enter Admin Username: ";
    cin >> user;

    cout << "Enter Admin Password: ";
    cin >> password;

    if (user == admin_user && password == admin_pass)
    {
        cout << "Admin login successful!\n";
        return 1;
    }

    cout << "Invalid admin credentials!\n";
    return 0;
}

int Admin::Authorise()
{
    string checkname, checkpass;

    cout << "Enter Admin Username: ";
    cin >> checkname;

    cout << "Enter Admin Password: ";
    cin >> checkpass;

    if (checkname == admin_user && checkpass == admin_pass)
        return 1;

    cout << "Not authorised.\n";
    return 0;
}

void Admin::ChangePassword()
{
    cout << "This demo uses a predefined admin password.\n";
    cout << "Changing it here will not persist after restarting.\n";
}

void Admin::adminMenu()
{
    int choice;

    do
    {
        cout << "\n------- ADMIN MENU -------\n";
        cout << "1. Manage Attendance\n";
        cout << "2. Manage Fee\n";
        cout << "3. Manage Leave\n";
        cout << "4. Manage Complaints\n";
        cout << "5. View Pending Students\n";
        cout << "6. Approve/Reject Student\n";
        cout << "7. Logout\n";

        cout << "Enter choice: ";
        cin >> choice;

        switch (choice)
        {
        case 1:
              adminAttendanceMenu();
            break;

        case 2:
            feesystem.AdminfeeDashboard();
            break;

        case 3:
            leaveManager.adminMenu();
            break;

        case 4:
            complaintSystem.AdminComplaintMenu();
            break;

        case 5:
        {
            User user;
            user.ViewPendingUsers();
            break;
        }

        case 6:
            UpdateStatus();
            break;

        case 7:
            cout << "Logged out successfully!\n";
            break;

        default:
            cout << "Invalid choice!\n";
        }

    } while (choice != 7);
}
