
#ifndef ROOM_ALLOCATION_H
#define ROOM_ALLOCATION_H

#include <iostream>
#include <fstream>
#include <string>
using namespace std;

typedef struct StudentNode
{
    int Id;
    StudentNode* next;
}sn;

typedef struct RoomNode
{
    int room_no;
    int capacity;
    int occupied;

    sn* students;
    RoomNode* next;

   
}rn;
#endif
