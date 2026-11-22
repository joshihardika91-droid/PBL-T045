#include "room_allocation.h"
#include <fstream>
#include <sstream>
#include "db.h"
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

// update a student's room in the student_details table
bool updateStudentRoom(int studentID, string roomNo)
{
    vector<vector<string> > rows;
    string check = "SELECT student_id FROM student_details WHERE student_id = " + to_string(studentID);

    if (!dbQuery(check, rows) || rows.size() == 0)
    {
        return false;
    }

    string sql = "UPDATE student_details SET room_no = '" + dbEscape(roomNo) +
                 "' WHERE student_id = " + to_string(studentID);

    return dbExecute(sql);
}

// check if student already has a room
bool studentHasRoom(int studentID)
{
    vector<vector<string> > rows;
    string sql = "SELECT room_no FROM student_details WHERE student_id = " + to_string(studentID);

    if (!dbQuery(sql, rows) || rows.size() == 0)
    {
        return false;
    }

    string room = rows[0][0];

    if (room != "" && room != "Not assigned")
    {
        return true;
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
    dbBegin();

    for (int i = 0; i < root->children.size(); i++)
    {
        RoomNode* hostel = root->children[i];
        for (int j = 0; j < hostel->children.size(); j++)
        {
            RoomNode* floor = hostel->children[j];
            for (int k = 0; k < floor->children.size(); k++)
            {
                RoomNode* room = floor->children[k];

                // rooms and residents are only ever added, so INSERT OR IGNORE skips the saved ones
                string sql = "INSERT OR IGNORE INTO rooms (hostel, floor_name, room_no) VALUES ('"
                             + dbEscape(hostel->name) + "', '"
                             + dbEscape(floor->name) + "', '"
                             + dbEscape(room->name) + "')";

                if (!dbExecute(sql))
                {
                    dbRollback();
                    cout << "Could not save room details." << endl;
                    return;
                }

                for (int m = 0; m < room->studentIDs.size(); m++)
                {
                    sql = "INSERT OR IGNORE INTO room_residents (room_no, student_id) VALUES ('"
                          + dbEscape(room->name) + "', "
                          + to_string(room->studentIDs[m]) + ")";

                    if (!dbExecute(sql))
                    {
                        dbRollback();
                        cout << "Could not save room details." << endl;
                        return;
                    }
                }
            }
        }
    }

    dbCommit();
}

void RoomAllocation::loadRooms()
{
    deleteRoomTree(root);
    root = new RoomNode("Root", "Root");

    vector<vector<string> > rooms;
    if (!dbQuery("SELECT hostel, floor_name, room_no FROM rooms ORDER BY id", rooms))
    {
        return;
    }

    for (int i = 0; i < rooms.size(); i++)
    {
        addRoomToTree(rooms[i][0], rooms[i][1], rooms[i][2]);
    }

    vector<vector<string> > residents;
    if (!dbQuery("SELECT room_no, student_id FROM room_residents ORDER BY id", residents))
    {
        return;
    }

    for (int i = 0; i < residents.size(); i++)
    {
        RoomNode* room = findRoom(residents[i][0]);
        if (room != NULL)
        {
            room->studentIDs.push_back(stoi(residents[i][1]));
        }
    }
}

void RoomAllocation::saveRequests()
{
    dbBegin();

    for (int i = 0; i < requests.size(); i++)
    {
        string sql = "INSERT INTO room_requests (request_id, student_id, room_no, status) VALUES ("
                     + to_string(requests[i].requestID) + ", "
                     + to_string(requests[i].studentID) + ", '"
                     + dbEscape(requests[i].roomNo) + "', '"
                     + dbEscape(requests[i].status) + "') "
                     "ON CONFLICT(request_id) DO UPDATE SET status = excluded.status";

        if (!dbExecute(sql))
        {
            dbRollback();
            cout << "Could not save requests." << endl;
            return;
        }
    }

    dbCommit();
}

void RoomAllocation::loadRequests()
{
    requests.clear();
    requestQueue = priority_queue<RoomRequest, vector<RoomRequest>, CompareRequest>();

    vector<vector<string> > rows;
    string sql = "SELECT request_id, student_id, room_no, status FROM room_requests ORDER BY request_id";

    if (!dbQuery(sql, rows))
    {
        return;
    }

    for (int i = 0; i < rows.size(); i++)
    {
        RoomRequest request;
        request.requestID = stoi(rows[i][0]);
        request.studentID = stoi(rows[i][1]);
        request.roomNo = rows[i][2];
        request.status = rows[i][3];

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