
#include "fee.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

using namespace std;

const string FEE_FILE = "fees.txt";
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
            catch (const invalid_argument& e)
            {
                cerr << "Invalid data in fee record: "
                    << e.what() << '\n';
            }
            catch (const out_of_range& e)
            {
                cerr << "Number out of range in fee record: "
                    << e.what() << '\n';
        }
    }
    file.close();
    createhashfunction(recordcount);
}

bool Feehandler::saveToFile()
{
    ofstream file(FEE_FILE);

    if (!file.is_open())
    {
        cerr << "Error: Could not open fee file for saving.\n";
        return false;
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

        if (!file)
        {
            cerr << "Error: Failed while writing fee records.\n";
            return false;
        }
    }

    file.close();

    if (file.fail())
    {
        cerr << "Error: Failed to close fee file properly.\n";
        return false;
    }

    return true;
}
void Feehandler::createhashfunction(int count)
{
    for (int i = 0; i < MAX; i++)
        hashtable[i] = -1;
    if (count < 0 || count > MAX)
    {
        cerr << "Invalid record count for hash table.\n";
        return;
    }

    for (int i = 0; i < count; i++)
    {
        int id = feerecords[i].getStudentID();
        if (id <= 0)
        {
            cerr << "Invalid Student ID. Record skipped in hash table.\n";
            continue;
        }
        int index = id % MAX;
        int start = index;
        while (hashtable[index] != -1)
        {
            index = (index + 1) % MAX;
            if (index == start)
            {
                cerr << "Hash table is full. Record could not be indexed.\n";
                return;
            }
        }
        hashtable[index] = i;
    }
}
int Feehandler::findrecords(int id)
{
    if (id <= 0)
        return -1;
    

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
    if (!saveToFile())
{
    --recordcount;
    createhashfunction(recordcount);
    cout << "Fee record was not saved.\n";
    return;
}

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

void Feehandler::merge(Fee arr[], int left, int mid, int right)
{
    int n1 = mid - left + 1;
    int n2 = right - mid;

    Fee* L = new Fee[n1];
    Fee* R = new Fee[n2];

    for (int i = 0; i < n1; i++)
        L[i] = arr[left + i];

    for (int j = 0; j < n2; j++)
        R[j] = arr[mid + 1 + j];

    int i = 0, j = 0, k = left;

    while (i < n1 && j < n2)
    {
        if (L[i].getdue() <= R[j].getdue())
            arr[k++] = L[i++];
        else
            arr[k++] = R[j++];
    }

    while (i < n1)
        arr[k++] = L[i++];

    while (j < n2)
        arr[k++] = R[j++];

    delete[] L;
    delete[] R;
}
void Feehandler::mergeSort(Fee arr[], int left, int right)
{
    if (left >= right)
        return;

    int mid = left + (right - left) / 2;
    mergeSort(arr, left, mid);
    mergeSort(arr, mid + 1, right);
    merge(arr, left, mid, right);
}

void Feehandler::sortbyduefeesMerge()
{
    loadFromFile();

    if (recordcount == 0)
    {
        cout << "No fee records available.\n";
        return;
    }
    mergeSort(feerecords, 0, recordcount - 1);
    createhashfunction(recordcount);

    if (!saveToFile())
    {
        cout << "Error saving sorted fee records.\n";
        return;
    }
    cout << "\nFee records sorted by due amount using Merge Sort:\n";
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
   Fee oldFee = feerecords[idx];

        if (!feerecords[idx].addpayment(amount))
        {
            cout << "Payment history is full.\n";
            return;
        }

        float remaining = feerecords[idx].getdue() - amount;

        if (remaining < 0.001f)
            remaining = 0;

        feerecords[idx].setdue(remaining);
        if (remaining == 0)
            feerecords[idx].setstatus("Paid");
        else
            feerecords[idx].setstatus("Partial");

        if (!saveToFile())
        {
            feerecords[idx] = oldFee;
            cout << "Payment could not be saved. Previous state restored in memory.\n";
            return;
        }
        cout << "Payment successful!\n";
        cout << "Remaining due: Rs. "
            << feerecords[idx].getdue() << '\n';
}
void Feehandler::viewpayments(int id)
{
    int idx = findrecords(id);

    if (idx == -1)
    {   cout << "No fee record found for this student.\n";
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
            sortbyduefeesMerge();
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
