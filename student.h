#ifndef STUDENT_H
#define STUDENT_H

#include "User.h"
#define MAX 100
class Student:public User
{   private:
    string StudentId;
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
     void feemanagement();
     void makepayement();
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
             
};
class StudentProfile
{
public:
    string id;
    string name;
    string room;
    string course;
    string phone;
    string guardianphone;
    string email;
    string password;

    void display();
};

class Attendance
{
private:
    string StudentId;
    string Name;
    string Date;
    string Status;
public:
void Attendancemark();
};


class StudentInterface
{
private:
    StudentProfile currentstudent;
    bool loggedin;
public:
StudentInterface();
    bool loadstudent(string id);
     bool isLoggedIn();
      string getCurrentId();
      void logout();
      void viewProfile();
      bool login() ;

};
#endif