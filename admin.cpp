#include<iostream>
#include<string>
using namespace std;
class Attendance
{
public:
    string StudentId;
    string Name;
    string Date;
    string Status;
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
    Attendance at;
    at.Attendancemark();
    return 0;
}


using namespace std;
class Admin{
    string Admin_password;
    string user_name;

};
