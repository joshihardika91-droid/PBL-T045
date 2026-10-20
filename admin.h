#ifndef ADMIN_H
#define ADMIN_H
#include<iostream>
using namespace std;
#include "complaint.h"
#include "leave.h"
#include "fee.h"
#include "attendance.h"
#include"user.h"
class Admin : public User
{
    private:
            string password;
            string user;

            const string admin_user= "ADMIN";
            const string admin_pass= "Hosttel@123";
            ComplaintManagement complaintSystem;
            LeaveManager leaveManager;
            Feehandler feesystem;
    public:
    int login();
     int Authorise();
     void adminMenu();
    void ChangePassword();


        
        
    };
#endif 