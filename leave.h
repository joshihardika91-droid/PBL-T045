#ifndef LEAVE_H
#define LEAVE_H

#include <iostream>
#include <string>
#include <vector>
#include <queue>
#include <fstream>
using namespace std;

class Leave {
private:
    int id;
    string studentId;
    string studentName;
    string reason;
    string fromDate;
    string toDate;
    int type;
    string status;

public:
    Leave();
    Leave(int id, string studentId, string studentName, string reason,
          string fromDate, string toDate, int type);

    int getId();
    string getStudentId();
    int getType();
    string getStatus();
    void setStatus(string newStatus);

    void display();
    void saveToFile(ofstream &fout);
    bool loadFromFile(ifstream &fin);
};

class LeaveManager {
private:
    vector<Leave> allLeaves;
    priority_queue<pair<int, int> > pending;
    int nextId;

    void loadData();
    void saveData();

public:
    LeaveManager();

    void applyLeave(string studentId, string studentName);
    void viewMyLeaves(string studentId);

    void viewPendingLeaves();
    void processNextLeave();
    void viewAllLeaves();

    void studentMenu(string studentId, string studentName);
    void adminMenu();
};

#endif



