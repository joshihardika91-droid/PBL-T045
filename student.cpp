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

    void Student:: menu()
class Student:public User
{   private:
    string StudentId;
    string Name;
    string Course;
    string RoomNo;
    ComplaintManagement complaintSystem;

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

        void Student::SubmitComplaint()
  {
    cout << "\n-------- COMPLAINT SECTION --------" << endl;
    complaintSystem.submit();
   }
    
    void Student::feemanagement(){
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
   void Student::makepayement(){
        float amount;
        
   }

   extern unordered_map<string, string> attendanceTable;
   void ViewAttendance()
   {
        string StudentId;
        cout << "Enter Student ID: ";
        cin >> StudentId;
    
    
    void Student::Feemenu(){
        
      
    }
    cout << "Student is Registered!" << endl;
cout << "Attendance: " << attendanceTable[StudentId] << endl;
   }

   

 void Attendance:: Attendancemark()
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





   void StudentProfile::display() {


    void Student::ViewProfile() {
        cout << "\n--------STUDENT PROFILE--------" << endl;
        cout << "Student ID:"<<StudentId<<endl;
        cout << "Name:"<<getName()<<endl;
        cout << "Course:"<<Course<<endl;
        cout << "Room No:"<<RoomNo<<endl;
     
        cout << "--------------------------------" << endl;
    }














    bool StudentInterface:: loadstudent(string id) {
        ifstream fin("students.txt");





            if(s.id == id) {
                currentstudent = s;
                return true;
            }
        }

        return false;
    }



    StudentInterface:: StudentInterface() {
        loggedin = false;
    }

bool   StudentInterface:: login() {
        string id;
        string pass;




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
    void  StudentInterface::  viewProfile(){
        if(!loggedin){
            cout << "Please login first!"<<endl;
            return;}
        currentstudent.display();}

    void   StudentInterface:: logout(){
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
    

    bool   StudentInterface:: isLoggedIn(){
        return loggedin;
    }

    string  StudentInterface::  getCurrentId() {
        return currentstudent.id;}




int main()
{
    StudentInterface studentLogin;

    if (studentLogin.login())
    {
        Student student;
        student.menu();
    }
    else
    {
        cout << "Login failed. Exiting..." << endl;
    }

