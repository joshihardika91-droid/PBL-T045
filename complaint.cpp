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