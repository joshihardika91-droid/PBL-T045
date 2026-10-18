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

static int isValidDate(const char *d) {
    if (strlen(d) != 10 || d[4] != '-' || d[7] != '-') {
        return 0;
    }
    for (int i = 0; i < 10; i++) {
        if (i != 4 && i != 7 && !isdigit((unsigned char)d[i])) {
            return 0;
        }
    }
    int m = atoi(d + 5), day = atoi(d + 8);
    return m >= 1 && m <= 12 && day >= 1 && day <= 31;
}

static char normalizeStatus(const char *s) {
    char t[16];
    int i;
    for (i = 0; s[i] && i < 15; i++) {
        t[i] = (char)tolower((unsigned char)s[i]);
    }
    t[i] = '\0';
    if (!strcmp(t, "p") || !strcmp(t, "present")) {
        return 'P';
    }
    if (!strcmp(t, "a") || !strcmp(t, "absent")) {
        return 'A';
    }
    return 0;
}

static void readLine(const char *prompt, char *buf, int size) {
    printf("%s", prompt);
    if (!fgets(buf, size, stdin)) {
        buf[0] = '\0';
        return;
    }
    buf[strcspn(buf, "\r\n")] = '\0';
}

static int readInt(const char *prompt) {
    char buf[32];
    readLine(prompt, buf, sizeof(buf));
    return atoi(buf);
}

static void pushAction(const char *id, const char *date, int existed, char old) {
    Action *a = (Action *)malloc(sizeof(Action));
    strncpy(a->id, id, ID_LEN - 1);
    a->id[ID_LEN - 1] = '\0';
    strncpy(a->date, date, DATE_LEN - 1);
    a->date[DATE_LEN - 1] = '\0';
    a->existed = existed;
    a->oldStatus = old;
    a->next = undoTop;
    undoTop = a;
}

static int markAttendance(const char *id, const char *name,
                          const char *date, char status) {
    if (id[0] == '\0' || !isValidDate(date) || (status != 'P' && status != 'A')) {
        return 0;
    }

    Student *s = findStudent(id);
    if (!s) {
        s = createStudent(id, name);
    }

    char old;
    int existed = setRecord(s, date, status, &old);
    pushAction(id, date, existed, old);
    return 1;
}

static int undoLast(void) {
    if (!undoTop) {
        return 0;
    }
    Action *a = undoTop;
    undoTop = a->next;

    Student *s = findStudent(a->id);
    if (s) {
        if (a->existed) {
            char tmp;
            setRecord(s, a->date, a->oldStatus, &tmp);
        } else {
            removeRecord(s, a->date);
        }
    }
    free(a);
    return 1;
}

static double percentage(const Student *s) {
    int total = 0, present = 0;
    for (Record *r = s->head; r; r = r->next) {
        total++;
        if (r->status == 'P') {
            present++;
        }
    }
    return total ? present * 100.0 / total : 0.0;
}

static int viewStudent(const char *id) {
    Student *s = findStudent(id);
    if (!s) {
        return 0;
    }

    int total = 0, present = 0;
    printf("\nStudent ID : %s\nName       : %s\n", s->id, s->name);
    printf("-----------------------------\n");
    printf("%-15s%s\n", "Date", "Status");
    for (Record *r = s->head; r; r = r->next) {
        printf("%-15s%s\n", r->date, r->status == 'P' ? "Present" : "Absent");
        total++;
        if (r->status == 'P') {
            present++;
        }
    }
    printf("-----------------------------\n");
    printf("Total classes : %d\nPresent       : %d\n", total, present);
    printf("Percentage    : %.2f%%\n", percentage(s));
    return 1;
}

static void viewByDate(const char *date) {
    int any = 0;
    printf("\nAttendance on %s\n", date);
    for (int i = 0; i < TABLE_SIZE; i++) {
        for (Student *s = table[i]; s; s = s->next) {
            for (Record *r = s->head; r; r = r->next) {
                if (strcmp(r->date, date) == 0) {
                    printf("%-10s %-20s %s\n", s->id, s->name,
                           r->status == 'P' ? "Present" : "Absent");
                    any = 1;
                }
            }
        }
    }
    if (!any) {
        printf("No records for this date.\n");
    }
}

static int cmpEntry(const void *a, const void *b) {
    double x = ((const Entry *)a)->per;
    double y = ((const Entry *)b)->per;
    return (x > y) - (x < y);
}

static void showDefaulters(double threshold) {
    int cap = 16, n = 0;
    Entry *arr = (Entry *)malloc(cap * sizeof(Entry));

    for (int i = 0; i < TABLE_SIZE; i++) {
        for (Student *s = table[i]; s; s = s->next) {
            double p = percentage(s);
            if (p < threshold) {
                if (n == cap) {
                    cap *= 2;
                    arr = (Entry *)realloc(arr, cap * sizeof(Entry));
                }
                arr[n].s = s;
                arr[n].per = p;
                n++;
            }
        }
    }

    if (n == 0) {
        printf("No defaulters.\n");
        free(arr);
        return;
    }

    qsort(arr, n, sizeof(Entry), cmpEntry);
    printf("\nStudents below %.1f%%\n", threshold);
    for (int i = 0; i < n; i++) {
        printf("%-10s %-20s %.2f%%\n", arr[i].s->id, arr[i].s->name, arr[i].per);
    }
    free(arr);
}