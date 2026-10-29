#include "room_allocation.h"
#include <fstream>
#include <sstream>
using namespace std;


// delete the whole tree
void deleteRoomTree(RoomNode* node)
{
    if (node == NULL)
    {
        return;
    }
    for (int i = 0; i < node->children.size(); i++)
    {
        deleteRoomTree(node->children[i]);
    }
    delete node;
}

// search a room by room number in the tree
RoomNode* searchRoom(RoomNode* node, string roomNo)
{
    if (node->type == "Room" && node->name == roomNo)
    {
        return node;
    }
    for (int i = 0; i < node->children.size(); i++)
    {
        RoomNode* found = searchRoom(node->children[i], roomNo);
        if (found != NULL)
        {
            return found;
        }
    }
    return NULL;
}

// update a student's room in student_details.txt
bool updateStudentRoom(int studentID, string roomNo)
{
    ifstream fin("student_details.txt");
    if (!fin)
    {
        return false;
    }

    vector<string> lines;
    string line;
    bool found = false;

    while (getline(fin, line))
    {
        stringstream ss(line);
        string id, course, room;

        getline(ss, id, '|');
        getline(ss, course, '|');
        getline(ss, room, '|');

        if (id == to_string(studentID))
        {
            lines.push_back(id + "|" + course + "|" + roomNo);
            found = true;
        }
        else
        {
            lines.push_back(line);
        }
    }
    fin.close();

    if (!found)
    {
        return false;
    }

    ofstream fout("student_details.txt");
    if (!fout)
    {
        return false;
    }
    for (int i = 0; i < lines.size(); i++)
    {
        fout << lines[i] << endl;
    }
    fout.close();
    return true;
}

// check if student already has a room
bool studentHasRoom(int studentID)
{
    ifstream fin("student_details.txt");
    string line;

    while (getline(fin, line))
    {
        stringstream ss(line);
        string id, course, room;

        getline(ss, id, '|');
        getline(ss, course, '|');
        getline(ss, room, '|');

        if (id == to_string(studentID))
        {
            if (room != "" && room != "Not assigned")
            {
                return true;
            }
            return false;
        }
    }
    return false;
}

RoomNode::RoomNode(string t, string n)
{
    type = t;
    name = n;
}

RoomAllocation::RoomAllocation()
{
    root = new RoomNode("Root", "Root");
    nextRequestID = 1;
    loadRooms();
    loadRequests();
}

RoomAllocation::~RoomAllocation()
{
    deleteRoomTree(root);
}

// Hostel -> Floor -> Room
void RoomAllocation::addRoomToTree(string hostel, string floor, string roomNo)
{
    RoomNode* hostelNode = NULL;
    for (int i = 0; i < root->children.size(); i++)
    {
        if (root->children[i]->name == hostel)
        {
            hostelNode = root->children[i];
        }
    }
    if (hostelNode == NULL)
    {
        hostelNode = new RoomNode("Hostel", hostel);
        root->children.push_back(hostelNode);
    }

    RoomNode* floorNode = NULL;
    for (int i = 0; i < hostelNode->children.size(); i++)
    {
        if (hostelNode->children[i]->name == floor)
        {
            floorNode = hostelNode->children[i];
        }
    }
    if (floorNode == NULL)
    {
        floorNode = new RoomNode("Floor", floor);
        hostelNode->children.push_back(floorNode);
    }

    floorNode->children.push_back(new RoomNode("Room", roomNo));
}

RoomNode* RoomAllocation::findRoom(string roomNo)
{
    return searchRoom(root, roomNo);
}

void RoomAllocation::displayTree(RoomNode* node, int level)
{
    if (node != root)
    {
        for (int i = 0; i < level; i++)
        {
            cout << "  ";
        }
        cout << node->type << ": " << node->name << endl;

        if (node->type == "Room")
        {
            cout << "  Residents: ";
            if (node->studentIDs.size() == 0)
            {
                cout << "None";
            }
            for (int i = 0; i < node->studentIDs.size(); i++)
            {
                cout << node->studentIDs[i] << " ";
            }
            cout << endl;
        }
    }

    for (int i = 0; i < node->children.size(); i++)
    {
        displayTree(node->children[i], level + 1);
    }
}

void RoomAllocation::saveRooms()
{
    ofstream fout("room_details.txt");
    if (!fout)
    {
        cout << "Could not save room details." << endl;
        return;
    }

    fout << "HostelName|Floor|RoomNo|StudentIDs" << endl;

    for (int i = 0; i < root->children.size(); i++)
    {
        RoomNode* hostel = root->children[i];
        for (int j = 0; j < hostel->children.size(); j++)
        {
            RoomNode* floor = hostel->children[j];
            for (int k = 0; k < floor->children.size(); k++)
            {
                RoomNode* room = floor->children[k];

                fout << hostel->name << "|" << floor->name << "|" << room->name << "|";

                for (int m = 0; m < room->studentIDs.size(); m++)
                {
                    if (m > 0)
                    {
                        fout << ",";
                    }
                    fout << room->studentIDs[m];
                }
                fout << endl;
            }
        }
    }
    fout.close();
}
void RoomAllocation::loadRooms()
{
    deleteRoomTree(root);
    root = new RoomNode("Root", "Root");

    ifstream fin("room_details.txt");
    string line;

    while (getline(fin, line))
    {
        stringstream ss(line);
        string hostel, floor, roomNo, students;

        getline(ss, hostel, '|');
        getline(ss, floor, '|');
        getline(ss, roomNo, '|');
        getline(ss, students);

        if (hostel == "HostelName" || hostel == "")
        {
            continue;
        }

        addRoomToTree(hostel, floor, roomNo);
        RoomNode* room = findRoom(roomNo);

        stringstream studentStream(students);
        string id;
        while (getline(studentStream, id, ','))
        {
            if (id != "")
            {
                room->studentIDs.push_back(stoi(id));
            }
        }
    }
    fin.close();
}

void RoomAllocation::saveRequests()
{
    ofstream fout("room_requests.txt");
    if (!fout)
    {
        cout << "Could not save requests." << endl;
        return;
    }

    fout << "RequestID|StudentID|RequestedRoom|Status" << endl;

    for (int i = 0; i < requests.size(); i++)
    {
        fout << requests[i].requestID << "|"
             << requests[i].studentID << "|"
             << requests[i].roomNo << "|"
             << requests[i].status << endl;
    }
    fout.close();
}

void RoomAllocation::loadRequests()
{
    requests.clear();
    requestQueue = priority_queue<RoomRequest, vector<RoomRequest>, CompareRequest>();

    ifstream fin("room_requests.txt");
    string line;

    while (getline(fin, line))
    {
        stringstream ss(line);
        string requestID, studentID, roomNo, status;

        getline(ss, requestID, '|');
        getline(ss, studentID, '|');
        getline(ss, roomNo, '|');
        getline(ss, status, '|');

        if (requestID == "RequestID" || requestID == "")
        {
            continue;
        }

        RoomRequest request;
        request.requestID = stoi(requestID);
        request.studentID = stoi(studentID);
        request.roomNo = roomNo;
        request.status = status;

        requests.push_back(request);

        if (request.requestID >= nextRequestID)
        {
            nextRequestID = request.requestID + 1;
        }
        if (request.status == "Pending")
        {
            requestQueue.push(request);
        }
    }
    fin.close();
}

void RoomAllocation::requestRoom(int studentID)
{
    if (studentHasRoom(studentID))
    {
        cout << "You already have a room assigned." << endl;
        return;
    }

    viewRooms();

    string roomNo;
    cout << "Enter preferred room number: ";
    cin >> roomNo;

    if (findRoom(roomNo) == NULL)
    {
        cout << "Room does not exist." << endl;
        return;
    }

    for (int i = 0; i < requests.size(); i++)
    {
        if (requests[i].studentID == studentID && requests[i].status == "Pending")
        {
            cout << "You already have a pending request." << endl;
            return;
        }
    }

    RoomRequest request;
    request.requestID = nextRequestID;
    nextRequestID++;
    request.studentID = studentID;
    request.roomNo = roomNo;
    request.status = "Pending";

    requests.push_back(request);
    requestQueue.push(request);
    saveRequests();

    cout << "Room request submitted." << endl;
}

void RoomAllocation::viewRequestStatus(int studentID)
{
    bool found = false;

    for (int i = 0; i < requests.size(); i++)
    {
        if (requests[i].studentID == studentID)
        {
            cout << "Request ID: " << requests[i].requestID
                 << " | Room: " << requests[i].roomNo
                 << " | Status: " << requests[i].status << endl;
            found = true;
        }
    }

    if (!found)
    {
        cout << "No room requests found." << endl;
    }
}

void RoomAllocation::studentMenu(int studentID)
{
    int choice = 0;

    while (choice != 4)
    {
        cout << "\n--- ROOM MENU ---" << endl;
        cout << "1. View rooms" << endl;
        cout << "2. Request a room" << endl;
        cout << "3. View request status" << endl;
        cout << "4. Return" << endl;
        cout << "Choice: ";
        cin >> choice;

        if (choice == 1)
        {
            viewRooms();
        }
        else if (choice == 2)
        {
            requestRoom(studentID);
        }
        else if (choice == 3)
        {
            viewRequestStatus(studentID);
        }
        else if (choice != 4)
        {
            cout << "Invalid choice." << endl;
        }
    }
}

void RoomAllocation::viewRooms()
{
    loadRooms();

    if (root->children.size() == 0)
    {
        cout << "No rooms have been added yet." << endl;
        return;
    }

    cout << "\n--- HOSTEL ROOM DETAILS ---" << endl;
    displayTree(root, 0);
}

void RoomAllocation::addRoom()
{
    string hostel, floor, roomNo;

    cout << "Hostel name: ";
    cin >> ws;
    getline(cin, hostel);

    cout << "Floor: ";
    getline(cin, floor);

    cout << "Room number: ";
    getline(cin, roomNo);

    loadRooms();

    if (findRoom(roomNo) != NULL)
    {
        cout << "Room number already exists." << endl;
        return;
    }

    addRoomToTree(hostel, floor, roomNo);
    saveRooms();

    cout << "Room added." << endl;
}
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