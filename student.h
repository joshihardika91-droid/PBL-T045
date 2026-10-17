#ifndef STUDENT_H
#define STUDENT_H

#include "User.h"
#define MAX 100
class Student : public User
{
private:
    int  StudentId;
    string Name;
    string Course;
    string RoomNo;
    ComplaintManagement complaintSystem;


public:
    void Register();
    int Login();
    void ViewProfile();
    void SubmitComplaint();
    void menu();
    int getStudentID(){return StudentId;}
};
class Fee:public Student{
    private :string status;
             float feeamount;
             string date;
             float due;
    public:void checkdues();
            void makepayment();
            void generateinvoice();
            void setdue(float);
            void setstatus(string);
            void setdate(string);
            string getdate(){return date;}
            string getstatus(){return status;}
            float getdue(){return due;}
            float getfeeamount(){return feeamount;}
             
};;

#endif