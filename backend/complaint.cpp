#include "complaint.h"
#include "db.h"

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

    if (dbId > 0) {
        dbExecute("UPDATE complaints SET status = 'Resolved' WHERE id = " + to_string(dbId));
    }

    cout << "Complaint marked as resolved" << endl;
}

void Complaint::setData(int id, string sid, string text, string st){
    dbId = id;
    studentId = sid;
    complaintText = text;
    status = st;
}

void Complaint::saveToDb(){
    string sql = "INSERT INTO complaints (student_id, complaint_text, status) VALUES ('"
                 + dbEscape(studentId) + "', '"
                 + dbEscape(complaintText) + "', '"
                 + dbEscape(status) + "')";

    if (dbExecute(sql)) {
        dbId = (int)dbLastInsertId();
    }
}
ComplaintManagement::ComplaintManagement()
{
    complaintCount = 0;
    front = -1;
    rear = -1;
}

// fills the queue again with the pending complaints saved in the database
void ComplaintManagement::loadPending()
{
    complaintCount = 0;
    front = -1;
    rear = -1;

    vector<vector<string> > rows;
    string sql = "SELECT id, student_id, complaint_text, status FROM complaints "
                 "WHERE status = 'Pending' ORDER BY id LIMIT 100";

    if (!dbQuery(sql, rows))
    {
        return;
    }

    for (int i = 0; i < rows.size(); i++)
    {
        if (front == -1)
            front = 0;

        rear = (rear + 1) % 100;
        complaints[rear].setData(stoi(rows[i][0]), rows[i][1], rows[i][2], rows[i][3]);
        complaintCount++;
    }
}

void ComplaintManagement::submit()
{
    loadPending();

    if ((rear + 1) % 100 == front)
    {
        cout << "Complaint queue is full!" << endl;
        return;
    }


    if (front == -1)
        front = 0;

    
    rear = (rear + 1) % 100;

    complaints[rear].submit_Complaint();
    complaints[rear].saveToDb();

    complaintCount++;
}

void ComplaintManagement::view_Complaints()
{
    loadPending();

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
    loadPending();

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