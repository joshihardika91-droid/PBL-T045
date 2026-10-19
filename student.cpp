#include <iostream>
#include <unordered_map>
#include <string>
#include "User.h"
#include "student.h"
#include <fstream>
#include <sstream>
#include "complaint.h"
#include "leave.h"
LeaveManager leaveManager;

using namespace std;
class Student : public User
{
private:
    string StudentId;
    string Name;
    string Course;
    string RoomNo;
    ComplaintManagement complaintSystem;

public:
    void menu()
    {
        int choice;
        do
        {
            cout << "\n--------STUDENT DASHBOARD--------" << endl;
            cout << "1.View profile" << endl;
            cout << "2.View Attendence" << endl;
            cout << "3.Apply for Leave" << endl;
            cout << "4.View Mess Schedule" << endl;
            cout << "5.Submit Complaint" << endl;
            cout << "6.View Fee Status" << endl;
            cout << "7.Change Password" << endl;
            cout << "8.Logout" << endl;

            cout << "Enter your choice" << endl;
            cin >> choice;

            switch (choice)
            {

            case 1:
                cout << "Displaying Profile" << endl;
                break;
            case 2:
                cout << "Displaying Attendance" << endl;
                break;
            case 3:
                leaveManager.studentMenu(StudentId, Name);
                break;
            case 4:
                cout << "Displaying Mess Schedule" << endl;
                break;
            case 5:
                SubmitComplaint();
                break;
            case 6:
                cout << "Displaying Fee Status" << endl;
                break;
            case 7:
                ChangePassword(); // inherited from user class
                break;
            case 8:
                cout << "Logging out" << endl;
                break;
            default:
                cout << "Invalid choice!" << endl;
            }
        } while (choice != 8);
    }

    void SubmitComplaint()
    {
        cout << "\n-------- COMPLAINT SECTION --------" << endl;
        complaintSystem.submit();
    }

    void feemanagement()
    {
        int ch, i;
        Fee fees[MAX];
        cout << "\n---------FEE PORTAL----------" << endl;
        cout << "1.Make payment" << endl;
        cout << "2.Check Dues" << endl;
        cout << "3.Generate Invoice" << endl;
        cout << "Choose the options " << endl;
        cin >> ch;
        switch (ch)
        {
        case 1:
            fees[i].makepayment();
            break;
        case 2:
            fees[i].checkdues();
            break;
        }
    }
    void makepayement()
    {
        float amount;
    }
};
extern unordered_map<string, string> attendanceTable;
void ViewAttendance()
{
    string StudentId;
    cout << "Enter Student ID: ";
    cin >> StudentId;

    if (attendanceTable.find(StudentId) == attendanceTable.end())
    {
        cout << "Student is not registered!" << endl;
        return;
    }
    cout << "Student is Registered!" << endl;
    cout << "Attendance: " << attendanceTable[StudentId] << endl;
}

class Attendance
{
private:
    string StudentId;
    string Name;
    string Date;
    string Status;

public:
    void Attendancemark()
    {
        cout << "Enter student id: " << endl;
        cin >> StudentId;
        cout << "Enter student name: " << endl;
        cin.ignore();
        getline(cin, Name);
        cout << "Enter present date: " << endl;
        cin >> Date;
        cout << "Enter status (Present/Absent): " << endl;
        cin >> Status;
        cout << "Attendance marked successfully! " << endl;
    }
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

    void display()
    {
        cout << "\n--------STUDENT PROFILE--------" << endl;
        cout << "Student ID:" << id << endl;
        cout << "Name:" << name << endl;
        cout << "Course:" << course << endl;
        cout << "Room No:" << room << endl;
        cout << "Phone:" << phone << endl;
        cout << "Guardian Phone no:" << guardianphone << endl;
        cout << "Email:" << email << endl;
        cout << "--------------------------------" << endl;
    }
























#include<iostream>
#include<unordered_map>
#include<string>
#include "User.h"
#include"student.h"
 #include<fstream>
#include<sstream>
#include "complaint.h"
#include"fee.h"

using namespace std;

    void Student::StudentMenu()
    {
        int choice;
        do{
            cout<<"\n--------STUDENT DASHBOARD--------"<<endl;
            cout<<"1.View profile"<<endl;
            cout<<"2.Attendance Menu"<<endl;
            cout<<"3.Fee Menu"<<endl;
            cout<<"4.Complaint Menu"<<endl;
            cout<<"5.Leave"<<endl;
            
            cout<<"6.Logout"<<endl;

            cout<<"Enter your choice"<<endl;
            cin>>choice;

            switch(choice)
            {

                case 1:  cout << "Displaying Profile" << endl;
                           ViewProfile();
                       break;
                case 2:  cout << "Attendance Menu" << endl;
                          AttendanceMenu();
                          break;
                case 3: cout << "Fee Menu " << endl;
                          Feemenu();
                        break;
                case 4: cout << "Complaint Menu" << endl;
                          ComplaintMenu();
                        break;
                case 5:cout <<"Leave Menu"<<endl;
                         LeaveMenu();
                        break;
               
                case 6: cout << "Logging out" << endl;
                        break;
                default: cout << "Invalid choice!" << endl;
            }
            }while(choice!=6);
        }

    
    
    void Student::Feemenu(){
        
      
    }


    void Student::ViewProfile() {
        cout << "\n--------STUDENT PROFILE--------" << endl;
        cout << "Student ID:"<<StudentId<<endl;
        cout << "Name:"<<getName()<<endl;
        cout << "Course:"<<Course<<endl;
        cout << "Room No:"<<RoomNo<<endl;
     
        cout << "--------------------------------" << endl;
    }
























#include<iostream>
#include<unordered_map>
#include<string>
#include "User.h"
#include"student.h"
 #include<fstream>
#include<sstream>
#include "complaint.h"
#include"fee.h"

using namespace std;

    void Student::StudentMenu()
    {
        int choice;
        do{
            cout<<"\n--------STUDENT DASHBOARD--------"<<endl;
            cout<<"1.View profile"<<endl;
            cout<<"2.Attendance Menu"<<endl;
            cout<<"3.Fee Menu"<<endl;
            cout<<"4.Complaint Menu"<<endl;
            cout<<"5.Leave"<<endl;
            
            cout<<"6.Logout"<<endl;

            cout<<"Enter your choice"<<endl;
            cin>>choice;

            switch(choice)
            {

                case 1:  cout << "Displaying Profile" << endl;
                           ViewProfile();
                       break;
                case 2:  cout << "Attendance Menu" << endl;
                          AttendanceMenu();
                          break;
                case 3: cout << "Fee Menu " << endl;
                          Feemenu();
                        break;
                case 4: cout << "Complaint Menu" << endl;
                          ComplaintMenu();
                        break;
                case 5:cout <<"Leave Menu"<<endl;
                         LeaveMenu();
                        break;
               
                case 6: cout << "Logging out" << endl;
                        break;
                default: cout << "Invalid choice!" << endl;
            }
            }while(choice!=6);
        }

    
    
    void Student::Feemenu(){
        
      
    }


    void Student::ViewProfile() {
        cout << "\n--------STUDENT PROFILE--------" << endl;
        cout << "Student ID:"<<StudentId<<endl;
        cout << "Name:"<<getName()<<endl;
        cout << "Course:"<<Course<<endl;
        cout << "Room No:"<<RoomNo<<endl;
     
        cout << "--------------------------------" << endl;
    }