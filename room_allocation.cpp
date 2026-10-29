#include "room_allocation.h"
#include <fstream>
#include <sstream>
using namespace std;


void RoomAllocation::viewPendingRequests()
{
    loadRequests();

    if (requestQueue.empty())
    {
        cout << "No pending requests." << endl;
        return;
    }

    priority_queue<RoomRequest, vector<RoomRequest>, CompareRequest> temp = requestQueue;

    while (!temp.empty())
    {
        RoomRequest request = temp.top();
        temp.pop();

        cout << "Request ID: " << request.requestID
             << " | Student ID: " << request.studentID
             << " | Room: " << request.roomNo << endl;
    }
}

void RoomAllocation::processRequest()
{
    loadRequests();

    if (requestQueue.empty())
    {
        cout << "No pending requests." << endl;
        return;
    }

    RoomRequest request = requestQueue.top();

    cout << "Student ID: " << request.studentID
         << " | Room: " << request.roomNo << endl;

    int choice;
    cout << "1. Approve  2. Reject: ";
    cin >> choice;

    // find this request in the vector
    int index = -1;
    for (int i = 0; i < requests.size(); i++)
    {
        if (requests[i].requestID == request.requestID)
        {
            index = i;
        }
    }
    if (index == -1)
    {
        return;
    }

    if (choice == 2)
    {
        requests[index].status = "Rejected";
        saveRequests();
        cout << "Request rejected." << endl;
        return;
    }

    if (choice != 1)
    {
        cout << "Invalid choice." << endl;
        return;
    }

    // if approval is not possible, reject it so it doesn't block the queue
    if (studentHasRoom(request.studentID))
    {
        requests[index].status = "Rejected";
        saveRequests();
        cout << "Student already has a room. Request rejected." << endl;
        return;
    }

    RoomNode* room = findRoom(request.roomNo);
    if (room == NULL)
    {
        requests[index].status = "Rejected";
        saveRequests();
        cout << "Room no longer exists. Request rejected." << endl;
        return;
    }

    if (!updateStudentRoom(request.studentID, request.roomNo))
    {
        cout << "Student record not found." << endl;
        return;
    }

    room->studentIDs.push_back(request.studentID);
    saveRooms();

    requests[index].status = "Approved";
    saveRequests();

    cout << "Room allocated successfully." << endl;
}

void RoomAllocation::allocateRoomDirectly()
{
    int studentID;
    string roomNo;

    cout << "Student ID: ";
    cin >> studentID;

    if (studentHasRoom(studentID))
    {
        cout << "Student already has a room." << endl;
        return;
    }

    viewRooms();

    cout << "Room number: ";
    cin >> roomNo;

    RoomNode* room = findRoom(roomNo);
    if (room == NULL)
    {
        cout << "Room not found." << endl;
        return;
    }

    if (!updateStudentRoom(studentID, roomNo))
    {
        cout << "Student record not found." << endl;
        return;
    }

    room->studentIDs.push_back(studentID);
    saveRooms();

    cout << "Room allocated successfully." << endl;
}

void RoomAllocation::viewRoomResidents()
{
    string roomNo;
    cout << "Room number: ";
    cin >> roomNo;

    RoomNode* room = findRoom(roomNo);
    if (room == NULL)
    {
        cout << "Room not found." << endl;
        return;
    }

    cout << "Students in room " << roomNo << ":" << endl;

    if (room->studentIDs.size() == 0)
    {
        cout << "No students assigned." << endl;
    }
    for (int i = 0; i < room->studentIDs.size(); i++)
    {
        cout << room->studentIDs[i] << endl;
    }
}

void RoomAllocation::adminMenu()
{
    int choice = 0;

    while (choice != 7)
    {
        cout << "\n--- ROOM ADMIN MENU ---" << endl;
        cout << "1. View rooms" << endl;
        cout << "2. Add room" << endl;
        cout << "3. View pending requests" << endl;
        cout << "4. Process next request" << endl;
        cout << "5. Allocate room directly" << endl;
        cout << "6. View room residents" << endl;
        cout << "7. Return" << endl;
        cout << "Choice: ";
        cin >> choice;

        if (choice == 1)
        {
            viewRooms();
        }
        else if (choice == 2)
        {
            addRoom();
        }
        else if (choice == 3)
        {
            viewPendingRequests();
        }
        else if (choice == 4)
        {
            processRequest();
        }
        else if (choice == 5)
        {
            allocateRoomDirectly();
        }
        else if (choice == 6)
        {
            viewRoomResidents();
        }
        else if (choice != 7)
        {
            cout << "Invalid choice." << endl;
        }
    }
}