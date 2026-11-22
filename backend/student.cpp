#include<iostream>
#include<string>
#include <fstream>
#include <sstream>
#include <vector>
#include"student.h"
#include "db.h"

 

using namespace std;
void Student::SetStudentDetails(int id, string name)
{
    StudentId = id;
    setName(name);
    LoadStudentDetails();


}
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
                          studentViewAttendance();
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
    


bool Student::LoadStudentDetails()
{
    vector<vector<string> > rows;
    string sql = "SELECT course, room_no FROM student_details WHERE student_id = "
                 + to_string(StudentId);

    if (dbQuery(sql, rows) && rows.size() > 0)
    {
        Course = rows[0][0];
        RoomNo = rows[0][1];
        return true;
    }

    Course = "Not assigned";
    RoomNo = "Not assigned";
    return false;
}

bool Student::SaveStudentDetails()
{
    string sql = "INSERT INTO student_details (student_id, course, room_no) VALUES ("
                 + to_string(StudentId) + ", '"
                 + dbEscape(Course) + "', '"
                 + dbEscape(RoomNo) + "') "
                 "ON CONFLICT(student_id) DO UPDATE SET course = excluded.course, room_no = excluded.room_no";

    if (!dbExecute(sql))
    {
        cout << "Could not save student details.\n";
        return false;
    }

    return true;
}