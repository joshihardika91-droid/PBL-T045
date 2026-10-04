#include<iostream>
#include<string>
#include "User.h"

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
