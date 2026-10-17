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

struct Leave leaves[MAX];
int count = 0;

void applyleave()
{
    struct Leave l;
    int choice;

    l.appid = count + 1;

    printf("\nEnter Student ID: ");
    scanf("%s", l.studentid);

    printf("Enter Student Name: ");
    scanf(" %[^\n]", l.studentname);

    printf("Enter From Date: ");
    scanf("%s", l.fromdate);

    printf("Enter To Date: ");
    scanf("%s", l.todate);

    printf("Enter Reason: ");
    scanf(" %[^\n]", l.reason);

    printf("\nSelect Leave Type:");
    printf("\n1. Normal Leave");
    printf("\n2. Medical Leave");
    printf("\n3. Emergency Leave");
    printf("\nEnter your choice: ");
    scanf("%d", &choice);

    if(choice == 1)
    {
        strcpy(l.leavetype, "Normal");
        l.priority = 2;
    }
    else if(choice == 2)
    {
        strcpy(l.leavetype, "Medical");
        l.priority = 1;
    }
    else if(choice == 3)
    {
        strcpy(l.leavetype, "Emergency");
        l.priority = 1;
    }
    else
    {
        printf("\nInvalid choice.");
        return;
    }

    strcpy(l.status, "Pending");

    leaves[count] = l;
    count++;

    printf("\nLeave application submitted successfully.");
    printf("\nApplication ID: %d\n", l.appid);
}


void displayLeaves()
{
    int i;

    if(count == 0)
    {
        printf("\nNo leave applications found.\n");
        return;
    }

    printf("\n----- Leave Applications -----\n");

    for(i = 0; i < count; i++)
    {
        printf("\nApplication ID: %d", leaves[i].appid);
        printf("\nStudent ID: %s", leaves[i].studentid);
        printf("\nStudent Name: %s", leaves[i].studentname);
        printf("\nFrom Date: %s", leaves[i].fromdate);
        printf("\nTo Date: %s", leaves[i].todate);
        printf("\nReason: %s", leaves[i].reason);
        printf("\nLeave Type: %s", leaves[i].leavetype);
        printf("\nStatus: %s", leaves[i].status);
        printf("\nPriority: %d", leaves[i].priority);
        printf("\n-----------------------------");
    }
}

