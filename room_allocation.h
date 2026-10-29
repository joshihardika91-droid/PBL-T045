
#ifndef ROOM_ALLOCATION_H
#define ROOM_ALLOCATION_H

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <queue>
using namespace std;

struct RoomNode
{
    string type;
    string name;
    vector<int> studentIDs;
    vector<RoomNode*> children;

    RoomNode(string t, string n);
};

struct RoomRequest
{
    int requestID;
    int studentID;
    string roomNo;
    string status;
};

struct CompareRequest
{
    bool operator()(RoomRequest a, RoomRequest b)
    {
        return a.requestID > b.requestID;
    }
};

class RoomAllocation
{
private:
    RoomNode* root;
    vector<RoomRequest> requests;

    priority_queue<RoomRequest,
                   vector<RoomRequest>,
                   CompareRequest> requestQueue;

    int nextRequestID;

    void addRoomToTree(string hostel, string floor, string roomNo);
    RoomNode* findRoom(string roomNo);
    void displayTree(RoomNode* node, int level);
    void saveRooms();
    void loadRooms();
    void saveRequests();
    void loadRequests();

public:
    RoomAllocation();
    ~RoomAllocation();

    RoomAllocation(const RoomAllocation&) = delete;
    RoomAllocation& operator=(const RoomAllocation&) = delete;

    void studentMenu(int studentID);
    void adminMenu();

    void requestRoom(int studentID);
    void viewRequestStatus(int studentID);

    void addRoom();
    void viewRooms();
    void viewPendingRequests();
    void processRequest();
    void allocateRoomDirectly();
    void viewRoomResidents();
};


#endif
