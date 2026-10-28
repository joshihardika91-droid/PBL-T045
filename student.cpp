
#include<iostream>
#include<string>
#include <fstream>
#include <sstream>
#include <vector>
#include"student.h"

 

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
    ifstream fin("student_details.txt");

    if (!fin.is_open())
    {
        cout << "Could not open student_details.txt\n";
        return false;
    }

    string line;
    string id, course, room;

    while (getline(fin, line))
    {
        stringstream ss(line);

        getline(ss, id, '|');
        getline(ss, course, '|');
        getline(ss, room, '|');

        // Skip the optional header row.
        if (id == "StudentID")
            continue;

        if (id == to_string(StudentId))
        {
            Course = course;
            RoomNo = room;
            fin.close();
            return true;
        }
    }

    fin.close();
    Course = "Not assigned";
    RoomNo = "Not assigned";
    return false;
}

bool Student::SaveStudentDetails()
{
    ifstream fin("student_details.txt");
    vector<string> lines;
    string line;
    bool found = false;
    bool hasHeader = false;

    if (fin.is_open())
    {
        while (getline(fin, line))
        {
            stringstream ss(line);
            string id;

            getline(ss, id, '|');

            if (id == "StudentID")
            {
                hasHeader = true;
                lines.push_back(line);
            }
            else if (id == to_string(StudentId))
            {
                lines.push_back(to_string(StudentId) + "|" +
                                Course + "|" + RoomNo);
                found = true;
            }
            else
            {
                lines.push_back(line);
            }
        }

        fin.close();
    }
    if (!found)
    {
        lines.push_back(to_string(StudentId) + "|" +
                        Course + "|" + RoomNo);
    }

    ofstream fout("student_details.txt", ios::trunc);

    if (!fout.is_open())
    {
        cout << "Could not save student details.\n";
        return false;
    }

    if (!hasHeader)
        fout << "StudentID|Course|RoomNo\n";

    for (const string& record : lines)
        fout << record << '\n';

    fout.close();
    return true;
}
