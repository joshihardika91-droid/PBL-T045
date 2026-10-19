#include "attendance.h"
#include <iostream>

using namespace std;

static Student *findStudentLocal(const char *id)
{
    for (Student *s = table[hashld(id)]; s; s = s->next)
    {
        if (strcmp(s->id, id) == 0)
        {
            return s;
        }
    }

    return NULL;
}

static Student *createStudent(const char *id, const char *name)
{
    Student *s = (Student *)calloc(1, sizeof(Student));

    if (s == NULL)
    {
        return NULL;
    }

    strncpy(s->id, id, ID_LEN - 1);
    strncpy(s->name, name, NAME_LEN - 1);

    unsigned i = hashld(s->id);

    s->next = table[i];
    table[i] = s;

    return s;
}

static int setRecord(Student *s, const char *date, char status, char *old)
{
    Record *prev = NULL;
    Record *cur = s->head;

    while (cur && strcmp(cur->date, date) < 0)
    {
        prev = cur;
        cur = cur->next;
    }

    if (cur && strcmp(cur->date, date) == 0)
    {
        *old = cur->status;
        cur->status = status;

        return 1;
    }

    Record *r = (Record *)malloc(sizeof(Record));

    if (r == NULL)
    {
        return -1;
    }

    strncpy(r->date, date, DATE_LEN - 1);
    r->date[DATE_LEN - 1] = '\0';

    r->status = status;
    r->next = cur;

    if (prev != NULL)
    {
        prev->next = r;
    }
    else
    {
        s->head = r;
    }

    *old = 0;

    return 0;
}

static void removeRecord(Student *s, const char *date)
{
    Record *prev = NULL;
    Record *cur = s->head;

    while (cur && strcmp(cur->date, date) != 0)
    {
        prev = cur;
        cur = cur->next;
    }

    if (cur == NULL)
    {
        return;
    }

    if (prev != NULL)
    {
        prev->next = cur->next;
    }
    else
    {
        s->head = cur->next;
    }

    free(cur);
}

static int isValidDate(const char *d)
{
    if (strlen(d) != 10 || d[4] != '-' || d[7] != '-')
    {
        return 0;
    }

    for (int i = 0; i < 10; i++)
    {
        if (i != 4 && i != 7 && !isdigit((unsigned char)d[i]))
        {
            return 0;
        }
    }

    int m = atoi(d + 5);
    int day = atoi(d + 8);

    return m >= 1 && m <= 12 && day >= 1 && day <= 31;
}

static char normalizeStatus(const char *s)
{
    char t[16];
    int i;

    for (i = 0; s[i] && i < 15; i++)
    {
        t[i] = (char)tolower((unsigned char)s[i]);
    }

    t[i] = '\0';

    if (strcmp(t, "p") == 0 || strcmp(t, "present") == 0)
    {
        return 'P';
    }

    if (strcmp(t, "a") == 0 || strcmp(t, "absent") == 0)
    {
        return 'A';
    }

    return 0;
}

static void readLine(const char *prompt, char *buf, int size)
{
    printf("%s", prompt);

    if (fgets(buf, size, stdin) == NULL)
    {
        buf[0] = '\0';
        return;
    }

    buf[strcspn(buf, "\r\n")] = '\0';
}

static int readInt(const char *prompt)
{
    char buf[32];

    readLine(prompt, buf, sizeof(buf));

    return atoi(buf);
}

static void pushAction(const char *id, const char *date, int existed, char old)
{
    Action *a = (Action *)malloc(sizeof(Action));

    if (a == NULL)
    {
        return;
    }

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
                          const char *date, char status)
{
    if (id[0] == '\0' || !isValidDate(date) ||
        (status != 'P' && status != 'A'))
    {
        return 0;
    }

    Student *s = findStudentLocal(id);

    if (s == NULL)
    {
        s = createStudent(id, name);

        if (s == NULL)
        {
            return 0;
        }
    }

    char old;

    int existed = setRecord(s, date, status, &old);

    if (existed == -1)
    {
        return 0;
    }

    pushAction(id, date, existed, old);

    return 1;
}

static int undoLast(void)
{
    if (undoTop == NULL)
    {
        return 0;
    }

    Action *a = undoTop;
    undoTop = a->next;

    Student *s = findStudentLocal(a->id);

    if (s != NULL)
    {
        if (a->existed)
        {
            char tmp;

            setRecord(s, a->date, a->oldStatus, &tmp);
        }
        else
        {
            removeRecord(s, a->date);
        }
    }

    free(a);

    return 1;
}

static double percentage(const Student *s)
{
    int total = 0;
    int present = 0;

    for (Record *r = s->head; r; r = r->next)
    {
        total++;

        if (r->status == 'P')
        {
            present++;
        }
    }

    return total ? present * 100.0 / total : 0.0;
}

static int viewStudent(const char *id)
{
    Student *s = findStudentLocal(id);

    if (s == NULL)
    {
        return 0;
    }

    int total = 0;
    int present = 0;

    printf("\nStudent ID : %s\nName       : %s\n", s->id, s->name);
    printf("--------------------------------\n");
    printf("%-15s%s\n", "Date", "Status");

    for (Record *r = s->head; r; r = r->next)
    {
        printf("%-15s%s\n", r->date,
               r->status == 'P' ? "Present" : "Absent");

        total++;

        if (r->status == 'P')
        {
            present++;
        }
    }

    printf("--------------------------------\n");
    printf("Total classes : %d\n", total);
    printf("Present       : %d\n", present);
    printf("Percentage    : %.2f%%\n", percentage(s));

    return 1;
}

static void viewByDate(const char *date)
{
    int any = 0;

    printf("\nAttendance on %s\n", date);

    for (int i = 0; i < TABLE_SIZE; i++)
    {
        for (Student *s = table[i]; s; s = s->next)
        {
            for (Record *r = s->head; r; r = r->next)
            {
                if (strcmp(r->date, date) == 0)
                {
                    printf("%-10s %-20s %s\n",
                           s->id,
                           s->name,
                           r->status == 'P' ? "Present" : "Absent");

                    any = 1;
                }
            }
        }
    }

    if (!any)
    {
        printf("No records for this date.\n");
    }
}

static int cmpEntry(const void *a, const void *b)
{
    double x = ((const Entry *)a)->per;
    double y = ((const Entry *)b)->per;

    return (x > y) - (x < y);
}

static void showDefaulters(double threshold)
{
    int cap = 16;
    int n = 0;

    Entry *arr = (Entry *)malloc(cap * sizeof(Entry));

    if (arr == NULL)
    {
        printf("Memory allocation failed!\n");
        return;
    }

    for (int i = 0; i < TABLE_SIZE; i++)
    {
        for (Student *s = table[i]; s; s = s->next)
        {
            double p = percentage(s);

            if (p < threshold)
            {
                if (n == cap)
                {
                    cap *= 2;

                    Entry *temp = (Entry *)realloc(
                        arr, cap * sizeof(Entry));

                    if (temp == NULL)
                    {
                        free(arr);
                        printf("Memory allocation failed!\n");
                        return;
                    }

                    arr = temp;
                }

                arr[n].s = s;
                arr[n].per = p;

                n++;
            }
        }
    }

    if (n == 0)
    {
        printf("No defaulters.\n");
        free(arr);
        return;
    }

    qsort(arr, n, sizeof(Entry), cmpEntry);

    printf("\nStudents below %.1f%%\n", threshold);

    for (int i = 0; i < n; i++)
    {
        printf("%-10s %-20s %.2f%%\n",
               arr[i].s->id,
               arr[i].s->name,
               arr[i].per);
    }

    free(arr);
}

void attendanceSave(void)
{
    FILE *f = fopen(FILE_NAME, "w");

    if (f == NULL)
    {
        printf("Could not save file!\n");
        return;
    }

    for (int i = 0; i < TABLE_SIZE; i++)
    {
        for (Student *s = table[i]; s; s = s->next)
        {
            for (Record *r = s->head; r; r = r->next)
            {
                fprintf(f, "%s|%s|%s|%c\n",
                        s->id,
                        s->name,
                        r->date,
                        r->status);
            }
        }
    }

    fclose(f);
}

void attendanceLoad(void)
{
    FILE *f = fopen(FILE_NAME, "r");

    if (f == NULL)
    {
        return;
    }

    char line[200];

    while (fgets(line, sizeof(line), f))
    {
        char *id = strtok(line, "|\n");
        char *name = strtok(NULL, "|\n");
        char *date = strtok(NULL, "|\n");
        char *st = strtok(NULL, "|\n");

        if (id == NULL || name == NULL ||
            date == NULL || st == NULL)
        {
            continue;
        }

        Student *s = findStudentLocal(id);

        if (s == NULL)
        {
            s = createStudent(id, name);

            if (s == NULL)
            {
                continue;
            }
        }

        char old;

        setRecord(s, date, st[0], &old);
    }

    fclose(f);
}

void attendanceCleanup(void)
{
    for (int i = 0; i < TABLE_SIZE; i++)
    {
        Student *s = table[i];

        while (s != NULL)
        {
            Record *r = s->head;

            while (r != NULL)
            {
                Record *temp = r;
                r = r->next;

                free(temp);
            }

            Student *tempStudent = s;
            s = s->next;

            free(tempStudent);
        }

        table[i] = NULL;
    }

    while (undoTop != NULL)
    {
        Action *a = undoTop;
        undoTop = a->next;

        free(a);
    }
}

void adminAttendanceMenu(void)
{
    int ch;

    char id[ID_LEN];
    char name[NAME_LEN];
    char date[DATE_LEN + 5];
    char st[16];

    cin.ignore(1000, '\n');

    do
    {
        printf("\n===== ATTENDANCE MODULE =====\n"
               "1. Mark attendance\n"
               "2. View student attendance\n"
               "3. View attendance by date\n"
               "4. Show defaulters\n"
               "5. Undo last entry\n"
               "0. Back\n");

        ch = readInt("Choice: ");

        if (ch == 1)
        {
            readLine("Enter student ID: ", id, sizeof(id));
            readLine("Enter student name: ", name, sizeof(name));
            readLine("Enter date (YYYY-MM-DD): ", date, sizeof(date));
            readLine("Enter status (Present/Absent): ", st, sizeof(st));

            if (markAttendance(id, name, date, normalizeStatus(st)))
            {
                attendanceSave();

                printf("Attendance marked successfully!\n");
            }
            else
            {
                printf("Invalid input or memory allocation failure!\n");
            }
        }
        else if (ch == 2)
        {
            readLine("Enter student ID: ", id, sizeof(id));

            if (!viewStudent(id))
            {
                printf("No attendance record found!\n");
            }
        }
        else if (ch == 3)
        {
            readLine("Enter date (YYYY-MM-DD): ", date, sizeof(date));

            if (isValidDate(date))
            {
                viewByDate(date);
            }
            else
            {
                printf("Invalid date!\n");
            }
        }
        else if (ch == 4)
        {
            double threshold = readInt(
                "Enter minimum percentage (e.g. 75): ");

            if (threshold >= 0 && threshold <= 100)
            {
                showDefaulters(threshold);
            }
            else
            {
                printf("Percentage must be between 0 and 100.\n");
            }
        }
        else if (ch == 5)
        {
            if (undoLast())
            {
                attendanceSave();

                printf("Last entry undone.\n");
            }
            else
            {
                printf("Nothing to undo.\n");
            }
        }

    } while (ch != 0);
}

void studentViewAttendance(void)
{
    char id[ID_LEN];

    cin.ignore(1000, '\n');

    readLine("Enter Student ID: ", id, sizeof(id));

    if (!viewStudent(id))
    {
        printf("No attendance record found for this ID!\n");
    }
}