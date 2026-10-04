#ifndef STUDENT_H
#define STUDENT_H

#include "User.h"

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

#endif