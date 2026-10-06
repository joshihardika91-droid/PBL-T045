#ifndef STUDENT_H
#define STUDENT_H

#include "User.h"
#define MAX 100
class Student : public User
{
private:
    string StudentId;
    string Name;
    string Course;
    string RoomNo;

public:
    void Register();
    int Login();
    void ViewProfile();
    void Complaint();
    void menu();
};
class Fee:public Student{
    private :string status;
             float amount;
             string date;
             float due;
    public:void checkdues();
            void makepayment();
            
             
};

#endif