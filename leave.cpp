#include "leave.h"

Leave::Leave() {
    id = 0;
    type = 1;
    status = "Pending";
}

Leave::Leave(int id, string studentId, string studentName, string reason,
             string fromDate, string toDate, int type) {
    this->id = id;
    this->studentId = studentId;
    this->studentName = studentName;
    this->reason = reason;
    this->fromDate = fromDate;
    this->toDate = toDate;
    this->type = type;
    status = "Pending";
}

int Leave::getId() {
    return id;
}

string Leave::getStudentId() {
    return studentId;
}

int Leave::getType() {
    return type;
}

string Leave::getStatus() {
    return status;
}

void Leave::setStatus(string newStatus) {
    status = newStatus;
}

void Leave::display() {
    cout << "--------------------------" << endl;
    cout << "Leave ID   : " << id << endl;
    cout << "Student ID : " << studentId << endl;
    cout << "Name       : " << studentName << endl;
    cout << "Reason     : " << reason << endl;
    cout << "From       : " << fromDate << endl;
    cout << "To         : " << toDate << endl;
    if (type == 2) {
        cout << "Type       : Urgent" << endl;
    } else {
        cout << "Type       : Normal" << endl;
    }
    cout << "Status     : " << status << endl;
}

void Leave::saveToFile(ofstream &fout) {
    fout << id << endl;
    fout << type << endl;
    fout << studentId << endl;
    fout << studentName << endl;
    fout << reason << endl;
    fout << fromDate << endl;
    fout << toDate << endl;
    fout << status << endl;
}

bool Leave::loadFromFile(ifstream &fin) {
    fin >> id;
    if (fin.fail()) {
        return false;
    }
    fin >> type;
    fin.ignore();
    getline(fin, studentId);
    getline(fin, studentName);
    getline(fin, reason);
    getline(fin, fromDate);
    getline(fin, toDate);
    getline(fin, status);
    return true;
}

LeaveManager::LeaveManager() {
    nextId = 1;
    loadData();
}

void LeaveManager::loadData() {
    ifstream fin("leaves.txt");
    if (!fin) {
        return;
    }

    Leave l;
    while (l.loadFromFile(fin)) {
        allLeaves.push_back(l);
        if (l.getStatus() == "Pending") {
            pending.push(make_pair(l.getType(), -l.getId()));
        }
    }
    fin.close();

    nextId = allLeaves.size() + 1;
}