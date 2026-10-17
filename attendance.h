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

static Student *table[TABLE_SIZE];
static Action  *undoTop = NULL;

static unsigned hashId(const char *s) {
    unsigned long h = 5381;
    int c;
    while ((c = (unsigned char)*s++)) {
        h = ((h << 5) + h) + c;
    }
    return (unsigned)(h % TABLE_SIZE);
}

static Student *findStudent(const char *id) {
    for (Student *s = table[hashId(id)]; s; s = s->next) {
        if (strcmp(s->id, id) == 0) {
            return s;
        }
    }
    return NULL;
}

static Student *createStudent(const char *id, const char *name) {
    Student *s = (Student *)calloc(1, sizeof(Student));
    strncpy(s->id, id, ID_LEN - 1);
    strncpy(s->name, name, NAME_LEN - 1);
    unsigned i = hashId(s->id);
    s->next = table[i];
    table[i] = s;
    return s;
}

static int setRecord(Student *s, const char *date, char status, char *old) {
    Record *prev = NULL, *cur = s->head;
    while (cur && strcmp(cur->date, date) < 0) {
        prev = cur;
        cur = cur->next;
    }

    if (cur && strcmp(cur->date, date) == 0) {
        *old = cur->status;
        cur->status = status;
        return 1;
    }

    Record *r = (Record *)malloc(sizeof(Record));
    strncpy(r->date, date, DATE_LEN - 1);
    r->date[DATE_LEN - 1] = '\0';
    r->status = status;
    r->next = cur;
    if (prev) {
        prev->next = r;
    } else {
        s->head = r;
    }
    *old = 0;
    return 0;
}

static void removeRecord(Student *s, const char *date) {
    Record *prev = NULL, *cur = s->head;
    while (cur && strcmp(cur->date, date) != 0) {
        prev = cur;
        cur = cur->next;
    }
    if (!cur) {
        return;
    }
    if (prev) {
        prev->next = cur->next;
    } else {
        s->head = cur->next;
    }
    free(cur);
}