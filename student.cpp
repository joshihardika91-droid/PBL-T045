#include<iostream>
#include<unordered_map>
#include<string>
#include "User.h"
#include"student.h"
 #include<fstream>
#include<sstream>
#include "complaint.h"
<<<<<<< Updated upstream
#include"fee.h"

using namespace std;

    void Student::StudentMenu()
=======
#include "leave.h"

using namespace std;
class Student:public User
{   private:
    string StudentId;
    string Name;
    string Course;
    string RoomNo;
    ComplaintManagement complaintSystem;
    LeaveManager leaveManager;

    public:
    void setStudentDetails(string id, string name) {
    StudentId = id;
    Name = name;
}
    void menu()
>>>>>>> Stashed changes
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
<<<<<<< Updated upstream
                case 2:  cout << "Attendance Menu" << endl;
                          AttendanceMenu();
                          break;
                case 3: cout << "Fee Menu " << endl;
                          Feemenu();
                        break;
                case 4: cout << "Complaint Menu" << endl;
                          complaintSystem.StudentComplaintMenu();
=======
                case 2:  cout << "Displaying Attendance" << endl;
                       break;
                case 3:leaveManager.studentMenu(StudentId, Name);
                       break;
                case 4: cout << "Displaying Mess Schedule" << endl;
>>>>>>> Stashed changes
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

<<<<<<< Updated upstream
=======
        return false;
    }

public:

    StudentInterface() {
        loggedin = false;
    }


bool login() {
        string id;
        string pass;

        cout << "\n--------STUDENT LOGIN--------" << endl;

        cout << "Enter Student ID: ";
        cin >> id;

        cout << "Enter Password: ";
        cin >> pass;

        if(loadstudent(id) && currentstudent.password == pass) {
            loggedin = true;

            cout << "Login successful! Welcome "
                 << currentstudent.name << endl;

            return true;
        }

        cout << "Invalid ID or Password!" << endl;

        return false;
    }
    void viewProfile(){
        if(!loggedin){
            cout << "Please login first!"<<endl;
            return;}
        currentstudent.display();}

    void logout(){
        char confirm;
        cout<<"Are you sure you want to logout?(yes/no):";
        cin>>confirm;
        if(confirm=='y'||confirm=='Y'){
            ofstream fout("logout_log.txt",ios::app);

            fout<<currentstudent.id<<"logged out"<<endl;
            fout.close();

            cout<<"Goodbye"<<currentstudent.name<<"!You have been logged out."<<endl;
            currentstudent=StudentProfile();
            loggedin=false;}
            else {
            cout<<"Logout cancelled."<<endl;}
    }
>>>>>>> Stashed changes
    
    
    void Student::Feemenu(){
        
      
    }

<<<<<<< Updated upstream

    void Student::ViewProfile() {
        cout << "\n--------STUDENT PROFILE--------" << endl;
        cout << "Student ID:"<<StudentId<<endl;
        cout << "Name:"<<getName()<<endl;
        cout << "Course:"<<Course<<endl;
        cout << "Room No:"<<RoomNo<<endl;
     
        cout << "--------------------------------" << endl;
    }
=======
    string getCurrentId() {
        return currentstudent.id;}

    string getCurrentName() {
    return currentstudent.name;
}

};
>>>>>>> Stashed changes




<<<<<<< Updated upstream




















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
        
      
=======
    if (studentLogin.login())
{
    Student student;

    student.setStudentDetails(
        studentLogin.getCurrentId(),
        studentLogin.getCurrentName()
    );

    student.menu();
}
    else
    {
        cout << "Login failed. Exiting..." << endl;
>>>>>>> Stashed changes
    }


    void Student::ViewProfile() {
        cout << "\n--------STUDENT PROFILE--------" << endl;
        cout << "Student ID:"<<StudentId<<endl;
        cout << "Name:"<<getName()<<endl;
        cout << "Course:"<<Course<<endl;
        cout << "Room No:"<<RoomNo<<endl;
     
        cout << "--------------------------------" << endl;
    }