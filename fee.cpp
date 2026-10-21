
#include "fee.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

using namespace std;

const string FEE_FILE = "fees.txt";

// Load all saved records.
void Feehandler::loadFromFile()
{
    recordcount = 0;

    for (int i = 0; i < MAX; i++)
        hashtable[i] = -1;

    ifstream file(FEE_FILE);

    if (!file.is_open())
        return;  // First run: file may not exist yet.

    string line;

    while (getline(file, line) && recordcount < MAX)
    {
        if (line.empty())
            continue;

        stringstream ss(line);
        string field;
        Fee fee;

        try
        {
            getline(ss, field, '|');
            fee.setstudentid(stoi(field));

            getline(ss, field, '|');
            fee.setfeeamount(stof(field));

            getline(ss, field, '|');
            fee.setdate(field);

            getline(ss, field, '|');
            fee.setdue(stof(field));

            getline(ss, field, '|');
            fee.setstatus(field);

            getline(ss, field, '|');
            int count = stoi(field);

            getline(ss, field, '|');
            stringstream paymentStream(field);
            string payment;

            for (int i = 0; i < count && i < 100; i++)
            {
                if (getline(paymentStream, payment, ','))
                    fee.loadpayment(stof(payment));
            }

            if (fee.getStudentID() > 0)
                feerecords[recordcount++] = fee;
        }
        catch (...)
        {
            cout << "Skipping invalid fee record in file.\n";
        }
    }

    file.close();
    createhashfunction(recordcount);
}

// Save all records, including payment history.
void Feehandler::saveToFile()
{
    ofstream file(FEE_FILE);

    if (!file.is_open())
    {
        cout << "Error: Could not save fee records.\n";
        return;
    }

    for (int i = 0; i < recordcount; i++)
    {
        Fee& fee = feerecords[i];

        file << fee.getStudentID() << '|'
             << fee.getfeeamount() << '|'
             << fee.getdate() << '|'
             << fee.getdue() << '|'
             << fee.getstatus() << '|'
             << fee.getpaycount() << '|';

        for (int j = 0; j < fee.getpaycount(); j++)
        {
            if (j > 0)
                file << ',';

            file << fee.getpayment(j);
        }

        file << '\n';
    }

    file.close();
}

void Feehandler::createhashfunction(int count)
{
    for (int i = 0; i < MAX; i++)
        hashtable[i] = -1;

    for (int i = 0; i < count; i++)
    {
        int index = feerecords[i].getStudentID() % MAX;
        int start = index;

        while (hashtable[index] != -1)
        {
            index = (index + 1) % MAX;

            if (index == start)
                return;
        }

        hashtable[index] = i;
    }
}

int Feehandler::findrecords(int id)
{
    if (id <= 0)
        return -1;

    // Refresh records so each Feehandler sees saved changes.
    loadFromFile();

    int index = id % MAX;
    int start = index;

    while (hashtable[index] != -1)
    {
        int recordIndex = hashtable[index];

        if (feerecords[recordIndex].getStudentID() == id)
            return recordIndex;

        index = (index + 1) % MAX;

        if (index == start)
            break;
    }

    return -1;
}

void Feehandler::addFeeRecord()
{
    // Avoid overwriting records saved by another object.
    loadFromFile();

    if (recordcount >= MAX)
    {
        cout << "Fee record limit reached.\n";
        return;
    }

    Fee fee;
    int sid;
    float amount;
    string date;

    cout << "Enter student ID: ";
    if (!(cin >> sid) || sid <= 0)
    {
        cin.clear();
        cin.ignore(10000, '\n');
        cout << "Invalid student ID.\n";
        return;
    }

    for (int i = 0; i < recordcount; i++)
    {
        if (feerecords[i].getStudentID() == sid)
        {
            cout << "A fee record already exists for this student.\n";
            return;
        }
    }

    cout << "Enter total fee amount: ";
    if (!(cin >> amount) || amount < 0)
    {
        cin.clear();
        cin.ignore(10000, '\n');
        cout << "Invalid fee amount.\n";
        return;
    }

    cout << "Enter date: ";
    getline(cin >> ws, date);

    fee.setstudentid(sid);
    fee.setfeeamount(amount);
    fee.setdue(amount);
    fee.setdate(date);

    if (amount == 0)
        fee.setstatus("Paid");

    feerecords[recordcount++] = fee;

    createhashfunction(recordcount);
    saveToFile();

    cout << "Fee record added and saved successfully.\n";
}

void Feehandler::displayfee()
{
    loadFromFile();

    if (recordcount == 0)
    {
        cout << "No fee records available.\n";
        return;
    }

    for (int i = 0; i < recordcount; i++)
    {
        cout << "\nStudent ID: " << feerecords[i].getStudentID()
             << "\nTotal fee: " << feerecords[i].getfeeamount()
             << "\nDue: " << feerecords[i].getdue()
             << "\nStatus: " << feerecords[i].getstatus()
             << "\nDate: " << feerecords[i].getdate() << '\n';
    }
}

void Feehandler::sortbyduefees(int count)
{
    loadFromFile();

    if (recordcount == 0)
    {
        cout << "No fee records available to sort.\n";
        return;
    }

    // Selection sort: lowest outstanding fee first.
    for (int i = 0; i < recordcount - 1; i++)
    {
        int minIndex = i;

        for (int j = i + 1; j < recordcount; j++)
        {
            if (feerecords[j].getdue() <
                feerecords[minIndex].getdue())
            {
                minIndex = j;
            }
        }

        if (minIndex != i)
        {
            Fee temp = feerecords[i];
            feerecords[i] = feerecords[minIndex];
            feerecords[minIndex] = temp;
        }
    }

    createhashfunction(recordcount);
    saveToFile();

    cout << "\nRecords sorted by outstanding dues:\n";
    displayfee();
}

void Feehandler::Makepayment(int id)
{
    int idx = findrecords(id);

    if (idx == -1)
    {
        cout << "No fee record found for this student.\n";
        return;
    }

    float amount;

    cout << "Outstanding fee: Rs. "
         << feerecords[idx].getdue() << '\n';

    if (feerecords[idx].getdue() <= 0)
    {
        cout << "All fees have been paid.\n";
        return;
    }

    cout << "Enter payment amount: Rs. ";

    if (!(cin >> amount))
    {
        cin.clear();
        cin.ignore(10000, '\n');
        cout << "Invalid payment amount.\n";
        return;
    }

    if (amount <= 0 || amount > feerecords[idx].getdue())
    {
        cout << "Invalid payment amount.\n";
        return;
    }

    if (!feerecords[idx].addpayment(amount))
    {
        cout << "Payment history is full.\n";
        return;
    }

    float remaining = feerecords[idx].getdue() - amount;
    feerecords[idx].setdue(remaining);

    if (remaining == 0)
        feerecords[idx].setstatus("Paid");
    else
        feerecords[idx].setstatus("Partial");

    saveToFile();

    cout << "Payment successful!\n";
    cout << "Remaining due: Rs. " << remaining << '\n';
}

void Feehandler::viewpayments(int id)
{
    int idx = findrecords(id);

    if (idx == -1)
    {
        cout << "No fee record found for this student.\n";
        return;
    }

    Fee& fee = feerecords[idx];

    cout << "\n------ TRANSACTION HISTORY ------\n";

    if (fee.getpaycount() == 0)
    {
        cout << "No payment history available.\n";
    }
    else
    {
        for (int i = 0; i < fee.getpaycount(); i++)
        {
            cout << "Payment " << i + 1 << ": Rs. "
                 << fee.getpayment(i) << '\n';
        }
    }

    cout << "Total paid: Rs. "
         << fee.getfeeamount() - fee.getdue() << '\n';

    cout << "Due fees: Rs. " << fee.getdue() << '\n';
}

void Feehandler::Studentfeedashboard(int id)
{
    int ch;

    do
    {
        cout << "\n---- Student Fee Portal ----\n";
        cout << "1. Make Payment\n";
        cout << "2. View Previous Payments\n";
        cout << "3. View Fee Details\n";
        cout << "4. Return to Student Menu\n";
        cout << "Enter choice: ";
        cin >> ch;

        switch (ch)
        {
        case 1:
            Makepayment(id);
            break;

        case 2:
            viewpayments(id);
            break;

        case 3:
        {
            int idx = findrecords(id);

            if (idx == -1)
                cout << "No fee record found for this student.\n";
            else
            {
                Fee& fee = feerecords[idx];

                cout << "\nTotal fee: Rs. " << fee.getfeeamount()
                     << "\nDue: Rs. " << fee.getdue()
                     << "\nStatus: " << fee.getstatus()
                     << "\nDate: " << fee.getdate() << '\n';
            }
            break;
        }

        case 4:
            return;

        default:
            cout << "Invalid choice.\n";
        }

    } while (ch != 4);
}

void Feehandler::AdminfeeDashboard()
{
    int ch;

    do
    {
        cout << "\n---- Admin Fee Portal ----\n";
        cout << "1. Sort Records by Dues\n";
        cout << "2. View All Records\n";
        cout << "3. Add Record\n";
        cout << "4. Return to Admin Menu\n";
        cout << "Enter choice: ";
        cin >> ch;

        switch (ch)
        {
        case 1:
            sortbyduefees(getrecordcount());
            break;

        case 2:
            displayfee();
            break;

        case 3:
            addFeeRecord();
            break;

        case 4:
            return;

        default:
            cout << "Invalid choice.\n";
        }

    } while (ch != 4);
}
