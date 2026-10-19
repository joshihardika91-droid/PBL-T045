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
                          complaintSystem.StudentComplaintMenu();
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


















