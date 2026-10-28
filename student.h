#ifndef STUDENT_H
#define STUDENT_H

#include"user.h"
#include"complaint.h"
#include"leave.h"
#include"attendance.h"
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
    Student(){ StudentId = 0;
    Course = "Not assigned";
    RoomNo = "Not assigned";}
    void SetCourse(string course) {
    Course = course;
        }

        string getCourse() {
            return Course;
        }
    void ViewProfile();
    void SetStudentDetails(int id, string name);
    void StudentMenu(int );
    int getStudentID(){return StudentId;}
    
bool LoadStudentDetails();
bool SaveStudentDetails();
};

#endif