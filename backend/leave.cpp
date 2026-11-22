#include "leave.h"
#include "db.h"

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

string Leave::getSaveSql() {
    return "INSERT INTO leaves (leave_id, type, student_id, student_name, reason, from_date, to_date, status) VALUES ("
           + to_string(id) + ", "
           + to_string(type) + ", '"
           + dbEscape(studentId) + "', '"
           + dbEscape(studentName) + "', '"
           + dbEscape(reason) + "', '"
           + dbEscape(fromDate) + "', '"
           + dbEscape(toDate) + "', '"
           + dbEscape(status) + "') "
           "ON CONFLICT(leave_id) DO UPDATE SET status = excluded.status";
}

LeaveManager::LeaveManager() {
    nextId = 1;
    loadData();
}

void LeaveManager::loadData() {
    vector<vector<string> > rows;
    string sql = "SELECT leave_id, type, student_id, student_name, reason, from_date, to_date, status "
                 "FROM leaves ORDER BY leave_id";

    if (!dbQuery(sql, rows)) {
        return;
    }

    for (int i = 0; i < rows.size(); i++) {
        Leave l(stoi(rows[i][0]), rows[i][2], rows[i][3], rows[i][4],
                rows[i][5], rows[i][6], stoi(rows[i][1]));
        l.setStatus(rows[i][7]);

        allLeaves.push_back(l);
        if (l.getStatus() == "Pending") {
            pending.push(make_pair(l.getType(), -l.getId()));
        }
    }

    nextId = allLeaves.size() + 1;
}


void LeaveManager::saveData() {
    dbBegin();

    for (int i = 0; i < allLeaves.size(); i++) {
        if (!dbExecute(allLeaves[i].getSaveSql())) {
            dbRollback();
            return;
        }
    }

    dbCommit();
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


void LeaveManager::processNextLeave() {
    if (pending.empty()) {
        cout << "No pending leaves." << endl;
        return;
    }

    int id = -pending.top().second;
    allLeaves[id - 1].display();

    int choice;
    cout << "1. Approve" << endl;
    cout << "2. Reject" << endl;
    cout << "3. Skip" << endl;
    cout << "Enter choice: ";
    cin >> choice;
    cin.ignore();

    if (choice == 1) {
        allLeaves[id - 1].setStatus("Approved");
        pending.pop();
        saveData();
        cout << "Leave approved." << endl;
    } else if (choice == 2) {
        allLeaves[id - 1].setStatus("Rejected");
        pending.pop();
        saveData();
        cout << "Leave rejected." << endl;
    } else {
        cout << "Leave is still pending." << endl;
    }
}


void LeaveManager::viewAllLeaves() {
    if (allLeaves.empty()) {
        cout << "No leave records." << endl;
        return;
    }
    for (int i = 0; i < allLeaves.size(); i++) {
        allLeaves[i].display();
    }
}


void LeaveManager::studentMenu(string studentId, string studentName) {
    int choice;
    do {
        cout << endl;
        cout << "1. Apply for leave" << endl;
        cout << "2. View my leaves" << endl;
        cout << "3. Back" << endl;
        cout << "Enter choice: ";
        cin >> choice;
        cin.ignore();

        if (choice == 1) {
            applyLeave(studentId, studentName);
        } else if (choice == 2) {
            viewMyLeaves(studentId);
        } else if (choice != 3) {
            cout << "Invalid choice." << endl;
        }
    } while (choice != 3);
}

void LeaveManager::adminMenu() {
    int choice;
    do {
        cout << endl;
        cout << "1. View pending leaves" << endl;
        cout << "2. Approve / reject next leave" << endl;
        cout << "3. View all leaves" << endl;
        cout << "4. Back" << endl;
        cout << "Enter choice: ";
        cin >> choice;
        cin.ignore();

        if (choice == 1) {
            viewPendingLeaves();
        } else if (choice == 2) {
            processNextLeave();
        } else if (choice == 3) {
            viewAllLeaves();
        } else if (choice != 4) {
            cout << "Invalid choice." << endl;
        }
    } while (choice != 4);
}