
#ifndef FEE_H
#define FEE_H

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include "user.h"

using namespace std;

class Fee
{
private:
    int StudentID;
    float feeamount;
    string date;
    float due;
    string Status;
    double payments[100];
    int paycount;

public:
    Fee()
    {
        StudentID = 0;
        feeamount = 0;
        date = "";
        due = 0;
        Status = "Pending";
        paycount = 0;

        for (int i = 0; i < 100; i++)
            payments[i] = 0;
    }

    int getStudentID() { return StudentID; }
    void setstudentid(int id) { StudentID = id; }

    void setfeeamount(float amount) { feeamount = amount; }
    float getfeeamount() { return feeamount; }

    void setdue(float amount) { due = amount; }
    float getdue() { return due; }

    void setdate(string d) { date = d; }
    string getdate() { return date; }

    void setstatus(string s) { Status = s; }
    string getstatus() { return Status; }

    int getpaycount() { return paycount; }

    bool addpayment(float amount)
    {
        if (paycount >= 100)
            return false;

        payments[paycount++] = amount;
        return true;
    }

    float getpayment(int i)
    {
        if (i < 0 || i >= paycount)
            return 0;

        return payments[i];
    }

    // Used when loading payment history from file.
    bool loadpayment(float amount)
    {
        return addpayment(amount);
    }
};

class Feehandler
{
private:
    Fee* feerecords;
    int* hashtable;
    int recordcount;

    void createhashfunction(int count);
    void merge(Fee arr[], int left, int mid, int right);
    void mergeSort(Fee arr[], int left, int right);

public:
    Feehandler()
    {
        feerecords = new Fee[MAX];
        hashtable = new int[MAX];
        recordcount = 0;

        for (int i = 0; i < MAX; i++)
            hashtable[i] = -1;

        loadFromFile();
    }

    ~Feehandler()
    {
        delete[] feerecords;
        delete[] hashtable;
    }

    int getrecordcount() { return recordcount; }

    void loadFromFile();
    bool saveToFile();

    
    int findrecords(int id);
    void displayfee();
    void addFeeRecord();
    void sortbyduefeesMerge();
    void Studentfeedashboard(int id);
    void AdminfeeDashboard();
    void Makepayment(int id);
    void viewpayments(int id);
};

#endif
