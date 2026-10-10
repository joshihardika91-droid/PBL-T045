#include<iostream>
#include<unordered_map>
#include<string>
#include "User.h"
#include"student.h"
 #include<fstream>
#include<sstream>

using namespace std;
class Student:public User
{   private:
    string StudentId;
    string Name;
    string Course;
    string RoomNo;

    public:
    void menu()
    {
        int choice;
        do{
            cout<<"\n--------STUDENT DASHBOARD--------"<<endl;
            cout<<"1.View profile"<<endl;
            cout<<"2.View Attendence"<<endl;
            cout<<"3.Apply for Leave"<<endl;
            cout<<"4.View Mess Schedule"<<endl;
            cout<<"5.Submit Complaint"<<endl;
            cout<<"6.View Fee Status"<<endl;
            cout<<"7.Change Password"<<endl;
            cout<<"8.Logout"<<endl;

            cout<<"Enter your choice"<<endl;
            cin>>choice;

            switch(choice)
            {

                case 1:  cout << "Displaying Profile" << endl;
                       break;
                case 2:  cout << "Displaying Attendance" << endl;
                       break;
                case 3: cout << "Leave Application" << endl;
                        break;
                case 4: cout << "Displaying Mess Schedule" << endl;
                        break;
                case 5:Complaint();
                        break;
                case 6:cout << "Displaying Fee Status" << endl;
                       break;
                case 7: ChangePassword(); //inherited from user class
                        break;
                case 8: cout << "Logging out" << endl;
                        break;
                default: cout << "Invalid choice!" << endl;
            }
            }while(choice<=8);
        }

        void Complaint(){
        string complaint;

        cout << "\n-------- COMPLAINT SECTION --------" << endl;

        cout << "Enter your complaint: ";
        cin.ignore();
        getline(cin, complaint);

        cout << "Complaint submitted successfully!" << endl;
         }

    };
    void feemanagement(){
        int ch,i;
        Fee fees[MAX];
        cout<<"\n---------FEE PORTAL----------"<<endl;
        cout<<"1.Make payment"<<endl;
        cout<<"2.Check Dues"<<endl;
        cout<<"3.Generate Invoice"<<endl;
        cout<<"Choose the options "<<endl;
        cin>>ch;
        switch(ch){
        case 1:fees[i].makepayment();
               break;
        case 2:fees[i].checkdues();
                break;
        
        }
    }
   void makepayement(){
        float amount;
        
   }

   extern unordered_map<string, string> attendanceTable;
   void ViewAttendance()
   {
        string StudentId;
        cout << "Enter Student ID: ";
        cin >> studentId;
        
        if (attendanceTable.find(studentId) == attendanceTable.end())
{
        cout << "Student is not registered!" << endl;
        return;
   

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
        getline(cin,Name);
        cout << "Enter present date: " << endl;
        cin >> Date;
        cout << "Enter status (Present/Absent): " << endl;
        cin >> Status;
        cout << "Attendance marked successfully! " << endl;
    }
    
};

int main()
{
    Admin admin;

         if(admin.login()==1)
    {
        cout << "Login Successful!" << endl;

        Attendance at;
        at.Attendancemark();
    }
    else
    {
        cout << "Invalid Credentials!" << endl;
    }


    return 0;

}
cout << "Student is Registered!" << endl;
cout << "Attendance: " << attendanceTable[studentId] << endl;


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

    void display() {
        cout << "\n--------STUDENT PROFILE--------" << endl;
        cout << "Student ID:"<<id<<endl;
        cout << "Name:"<<name<<endl;
        cout << "Course:"<<course<<endl;
        cout << "Room No:"<<room<<endl;
        cout << "Phone:"<<phone<<endl;
        cout << "Guardian Phone no:"<<guardianphone<<endl;
        cout << "Email:"<<email<<endl;
        cout << "--------------------------------" << endl;
    }
};
























class StudentInterface
{
private:
    StudentProfile currentstudent;
    bool loggedin;

    bool loadstudent(string id) {
        ifstream fin("students.txt");

        string line;

        while(getline(fin, line)) {
            stringstream ss(line);

            StudentProfile s;

            getline(ss, s.id, '|');
            getline(ss, s.name, '|');
            getline(ss, s.room, '|');
            getline(ss, s.course, '|');
            getline(ss, s.phone, '|');
            getline(ss, s.guardianphone, '|');
            getline(ss, s.email, '|');
            getline(ss, s.password, '|');

            if(s.id == id) {
                currentstudent = s;
                return true;
            }
        }

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
    

    bool isLoggedIn(){
        return loggedin;
    }

    string getCurrentId() {
        return currentstudent.id;}
};



int main()
{
    Admin admin;

         if(admin.login()==1)
    {
        cout << "Login Successful!" << endl;

        Attendance at;
        at.Attendancemark();
    }
    else
    {
        cout << "Invalid Credentials!" << endl;
    }


    return 0;
}
cout << "Student is Registered!" << endl;
cout << "Attendance: " << attendanceTable[studentId] << endl;
   }