#ifndef COMPLAINT_H
#define COMPLAINT_H

#include <iostream>
#include <string>
using namespace std;

class Complaint
{
private:
    string studentId;
    string complaintText;
    string status;
    int dbId;

public:
    Complaint() { dbId = 0; }
    void setData(int id, string sid, string text, string st);
    void saveToDb();
    void submit_Complaint();
    void display_Complaint();
    void resolve_Complaint();

};

class ComplaintManagement
{
private:
    Complaint complaints[100];
    int complaintCount;
        int front, rear;

    void loadPending();

public:
    ComplaintManagement();

    void submit();
    void view_Complaints();
    void resolve();
    void StudentComplaintMenu();
void AdminComplaintMenu();

};
#endif