#include "complaint.h"

void Complaint::submit_Complaint(){
    cout << "Enter Student ID: ";
    cin >> studentId;
   cout << "Enter Complaint: ";
    cin.ignore();
    getline(cin, complaintText);
    status = "Pending";
    cout << "Complaint submitted successfully!" << endl;
}

void Complaint::display_Complaint(){
    cout << "\nStudent ID: " << studentId << endl;
    cout << "Complaint: " << complaintText << endl;
    cout << "Status: " << status << endl;
}

void Complaint::resolve_Complaint(){
    status = "Resolved";
    cout << "Complaint marked as resolved" << endl;
}
ComplaintManagement::ComplaintManagement()
{
    complaintCount = 0;
    front = -1;
    rear = -1;
}

void ComplaintManagement::submit()
{
    
    if ((rear + 1) % 100 == front)
    {
        cout << "Complaint queue is full!" << endl;
        return;
    }


    if (front == -1)
        front = 0;

    
    rear = (rear + 1) % 100;

    complaints[rear].submit_Complaint();

    complaintCount++;
}

void ComplaintManagement::view_Complaints()
{
    if (front == -1)
    {
        cout << "No complaints available!" << endl;
        return;
    }

    int i = front;
    int count = 1;

    while (true)
    {
        cout << "\nComplaint Number: " << count << endl;
        complaints[i].display_Complaint();

        if (i == rear)
            break;

        i = (i + 1) % 100;
        count++;
    }
}

void ComplaintManagement::resolve()
{
    if (front == -1)
    {
        cout << "No complaints to resolve!" << endl;
        return;
    }

    complaints[front].resolve_Complaint();

    
    if (front == rear)
    {
        front = rear = -1;
    }
    else
    {
        front = (front + 1) % 100;
    }

    complaintCount--;
}


ComplaintManagement complaintSystem;

void ComplaintManagement:: StudentComplaintMenu()
{
    int choice;

    do
    {
        cout << "\n----- STUDENT COMPLAINT MENU -----" << endl;
        cout << "1. Submit Complaint" << endl;
        cout << "2. Back" << endl;
        cout << "Enter choice: ";
        cin >> choice;

        switch (choice)
        {
            case 1:
                complaintSystem.submit();
                break;

            case 2:
                cout << "Returning to Student Menu..." << endl;
                return;

            default:
                cout << "Invalid choice!" << endl;
        }

    } while (choice != 2);
}


void ComplaintManagement:: AdminComplaintMenu()
{
    int choice;

    do
    {
        cout << "\n----- ADMIN COMPLAINT MENU -----" << endl;
        cout << "1. View Complaints" << endl;
        cout << "2. Resolve Complaint" << endl;
        cout << "3. Back" << endl;
        cout << "Enter choice: ";
        cin >> choice;

        switch (choice)
        {
            case 1:
                complaintSystem.view_Complaints();
                break;

            case 2:
                complaintSystem.resolve();
                break;

            case 3:
                cout << "Returning to Admin Menu..." << endl;
                return;
            

            default:
                cout << "Invalid choice!" << endl;
        }

    } while (choice != 3);

}