#include "complaint.h"
#include<string>

ComplaintManagement::ComplaintManagement()
{
    complaintCount = 0;
}

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

ComplaintManagement::ComplaintManagement(){
    front = -1;
    rear = -1;
}

void ComplaintManagement::submit(){
    if (rear == 99)
    { cout << "Complaint queue is full!" << endl;
        return;
    }

    if (front == -1)
        front = 0;

    rear++;
    complaints[rear].submit_Complaint();
}
void ComplaintManagement::view_Complaints(){
    for(int i = 0; i < complaintCount; i++)
    {
        cout << "\nComplaint Number: " << i + 1 << endl;
        complaints[i].display_Complaint();
    }
}

void ComplaintManagement::resolve(){
    int n;
view_Complaints();

    cout << "\nEnter complaint number to resolve: ";
    cin >> n;
if(n > 0 && n <= complaintCount)
        complaints[n - 1].resolve_Complaint();
}