#include "complaint.h"
#include<string>


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
