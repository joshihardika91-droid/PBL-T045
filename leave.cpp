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


void LeaveManager::saveData() {
    ofstream fout("leaves.txt");
    for (int i = 0; i < allLeaves.size(); i++) {
        allLeaves[i].saveToFile(fout);
    }
    fout.close();
}

void LeaveManager::applyLeave(string studentId, string studentName) {
    string reason, fromDate, toDate;
    int type;

    cout << "Enter reason: ";
    getline(cin, reason);
    cout << "Enter start date (DD-MM-YYYY): ";
    getline(cin, fromDate);
    cout << "Enter end date (DD-MM-YYYY): ";
    getline(cin, toDate);
    cout << "Leave type (1 = Normal, 2 = Urgent): ";
    cin >> type;
    cin.ignore();

    if (type != 2) {
        type = 1;
    }

    Leave l(nextId, studentId, studentName, reason, fromDate, toDate, type);
    allLeaves.push_back(l);
    pending.push(make_pair(type, -nextId));
    nextId++;
    saveData();

    cout << "Leave applied. Your Leave ID is " << l.getId() << endl;
}

void LeaveManager::viewMyLeaves(string studentId) {
    bool found = false;
    for (int i = 0; i < allLeaves.size(); i++) {
        if (allLeaves[i].getStudentId() == studentId) {
            allLeaves[i].display();
            found = true;
        }
    }
    if (found == false) {
        cout << "You have no leave applications." << endl;
    }
}

void LeaveManager::viewPendingLeaves() {
    if (pending.empty()) {
        cout << "No pending leaves." << endl;
        return;
    }

    priority_queue<pair<int, int> > temp = pending;
    while (!temp.empty()) {
        int id = -temp.top().second;
        allLeaves[id - 1].display();
        temp.pop();
    }
}
