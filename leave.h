#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX 100
#define HASH_SIZE 50

struct Leave
{
    int appid;
    char studentid[20];
    char studentname[50];
    char fromdate[20];
    char todate[20];
    char reason[100];
    char leavetype[20];
    char status[20];
    int priority;
};



