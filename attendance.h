#ifndef ATTENDANCE_H
#define ATTENDANCE_H

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<ctype.h>

#define TABLE_SIZE 101
#define ID_LEN 20
#define NAME_LEN 50
#define DATE_LEN 11
#define FILE_NAME "attendance.txt"

typedef struct Record
{
    char date[DATE_LEN];
    char status;
    struct Record *next;
}Record;
 
typedef struct AttendanceStudent
{
    char id[ID_LEN];
    char name[NAME_LEN];
    Record *head;
    struct AttendanceStudent *next;
}AttendanceStudent;

typedef struct Action
{
    char id[ID_LEN];
    char date[DATE_LEN];
    int existed;
    char oldStatus;
    struct Action *next;
}Action;

typedef struct Entry
{
    AttendanceStudent *s;
    double per;
}Entry;


void attendanceSave(void);
void attendanceLoad(void);
void attendanceCleanup(void);
void adminAttendanceMenu(void);
void studentViewAttendance(void);
#endif
