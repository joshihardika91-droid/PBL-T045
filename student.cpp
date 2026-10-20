
#include<iostream>
#include<string>

#include"student.h"

 

using namespace std;

    void Student::StudentMenu(int s_id)
    {
        int choice;
        string Name=getName();
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
                          
                          break;
                case 3: cout << "Fee Menu " << endl;
                          feesystem.Studentfeedashboard(s_id);
                        break;
                case 4: cout << "Complaint Menu" << endl;
                          complaintSystem.StudentComplaintMenu();
                        break;
                case 5:cout <<"Leave Menu"<<endl;
                          
                          leaveManager.studentMenu(to_string(s_id),Name);
                        break;
               
                case 6: cout << "Logging out" << endl;
                        break;
                default: cout << "Invalid choice!" << endl;
            }
            }while(choice!=6);
        }

    
   


    void Student::ViewProfile() {
        cout << "\n--------STUDENT PROFILE--------" << endl;
        cout << "Student ID:"<<StudentId<<endl;
        cout << "Name:"<<getName()<<endl;
        cout << "Course:"<<Course<<endl;
        cout << "Room No:"<<RoomNo<<endl;
     
        cout << "--------------------------------" << endl;
    }