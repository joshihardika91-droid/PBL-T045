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
 
typedef struct Student
{
    char id[ID_LEN];
    char name[NAME_LEN];
    Record *head;
    struct Student *next;
}Student;

typedef struct ACTION
{
    char id[ID_LEN];
    char date[DATA_LEN];
    int existed;
    char oldStatus;
    struct Actin *next;
}Action;

typedef struct 
{
    Student *s;
    double per;
}Entry;

