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
    char date;
    char status
    struct Record *next;
}Record;
 
typedef struct Student
{
    char id;
    char name;
    Record *head;
    struct Student *next;
}Student;


}
