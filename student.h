#ifndef STUDENT_H
#define STUDENT_H

#include "User.h"
#define MAX 100
class Student : public User
{
private:
    int StudentId;
    string Course;
    string RoomNo;
    ComplaintManagement complaintSystem;


public:
    
    void ViewProfile();
    void ComplaintMenu();
    void Feemenu();
    void LeaveMenu();
    void AttendanceMenu();
    void StudentMenu();
    int getStudentID(){return StudentId;}
};



#endif