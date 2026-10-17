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

typedef struct Action
{
    char id[ID_LEN];
    char date[DATE_LEN];
    int existed;
    char oldStatus;
    struct Action *next;
}Action;

typedef struct{
    Student *s;
    double per;
}Entry;

static Student *table[TABLE_SIZE];
static Action *undoTop = NULL;

static unsigned hashld(const char *s)
{
    unsigned long h = 5381;
    int c;
    while((c = (unsigned char)*s++))
    h = ((h << 5) + h) + c;
    return(unsigned)(h % TABLE_SIZE);
}

static Student *findStudent(const char*id)
{
    for(Student *s = table[hashld(id)]; s; s = s->next){
    if(strcmp(s->id,id) == 0){}
    return s;
}
    return NULL;
}
#endif
