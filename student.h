#ifndef STUDENT_H
#define STUDENT_H

#include"user.h"
#include"complaint.h"
#include"leave.h"

#include"fee.h"
class Student : public User
{
private:
    int StudentId;
    string Course;
    string RoomNo;
    ComplaintManagement complaintSystem;
    Feehandler feesystem;
    LeaveManager leaveManager;


public:
    
    void ViewProfile();
   
    void StudentMenu(int );
    int getStudentID(){return StudentId;}
};



#endif