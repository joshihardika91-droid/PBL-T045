#include "complaint.h"

ComplaintManagement::ComplaintManagement()
{
    complaintCount = 0;
}

void Complaint::submit_Complaint()
{
    cout << "Enter Student ID: ";
    cin >> studentId;
   cout << "Enter Complaint: ";
    cin.ignore();
    getline(cin, complaintText);
    status = "Pending";
    cout << "Complaint submitted successfully!" << endl;
}
void Complaint::display_Complaint()
{
    cout << "\nStudent ID: " << studentId << endl;
    cout << "Complaint: " << complaintText << endl;
    cout << "Status: " << status << endl;
}

void Complaint::resolve_Complaint()
{
    status = "Resolved";
    cout << "Complaint marked as resolved!" << endl;
}