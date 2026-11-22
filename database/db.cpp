#include <iostream>
#include <string>
#include <vector>
#include <cstdlib>
#include "sqlite3.h"
#include "db.h"

using namespace std;

static sqlite3 *conn = NULL;

static void createTables()
{
    const char *queries[] =
    {
        "CREATE TABLE IF NOT EXISTS users ("
        
        "id INTEGER PRIMARY KEY AUTOINCREMENT, "
        "user_id VARCHAR(20) NOT NULL UNIQUE, "
        "name VARCHAR(100) NOT NULL, "
        "password VARCHAR(100) NOT NULL, "
        "role VARCHAR(20) NOT NULL, "
        "status VARCHAR(20) NOT NULL)",

        "CREATE TABLE IF NOT EXISTS student_details ("
        "student_id INTEGER PRIMARY KEY, "
        "course VARCHAR(100) NOT NULL DEFAULT 'Not assigned', "
        "room_no VARCHAR(20) NOT NULL DEFAULT 'Not assigned')",

        "CREATE TABLE IF NOT EXISTS rooms ("
        "id INTEGER PRIMARY KEY AUTOINCREMENT, "
        "hostel VARCHAR(50) NOT NULL, "
        "floor_name VARCHAR(20) NOT NULL, "
        "room_no VARCHAR(20) NOT NULL UNIQUE)",

        "CREATE TABLE IF NOT EXISTS room_residents ("
        "id INTEGER PRIMARY KEY AUTOINCREMENT, "
        "room_no VARCHAR(20) NOT NULL, "
        "student_id INTEGER NOT NULL, "
        "UNIQUE (room_no, student_id), "
        "FOREIGN KEY (room_no) REFERENCES rooms(room_no))",

        "CREATE TABLE IF NOT EXISTS room_requests ("
        "request_id INTEGER PRIMARY KEY, "
        "student_id INTEGER NOT NULL, "
        "room_no VARCHAR(20) NOT NULL, "
        "status VARCHAR(20) NOT NULL)",

        "CREATE TABLE IF NOT EXISTS attendance ("
        "student_id VARCHAR(20) NOT NULL, "
        "student_name VARCHAR(50) NOT NULL, "
        "att_date VARCHAR(10) NOT NULL, "
        "status CHAR(1) NOT NULL, "
        "PRIMARY KEY (student_id, att_date))",

        "CREATE TABLE IF NOT EXISTS fees ("
        "student_id INTEGER PRIMARY KEY, "
        "fee_amount REAL NOT NULL, "
        "fee_date VARCHAR(50) NOT NULL, "
        "due REAL NOT NULL, "
        "status VARCHAR(20) NOT NULL, "
        "pos INTEGER NOT NULL DEFAULT 0)",

        "CREATE TABLE IF NOT EXISTS fee_payments ("
        "student_id INTEGER NOT NULL, "
        "pay_no INTEGER NOT NULL, "
        "amount REAL NOT NULL, "
        "PRIMARY KEY (student_id, pay_no), "
        "FOREIGN KEY (student_id) REFERENCES fees(student_id))",

        "CREATE TABLE IF NOT EXISTS leaves ("
        "leave_id INTEGER PRIMARY KEY, "
        "type INTEGER NOT NULL, "
        "student_id VARCHAR(20) NOT NULL, "
        "student_name VARCHAR(100) NOT NULL, "
        "reason TEXT NOT NULL, "
        "from_date VARCHAR(20) NOT NULL, "
        "to_date VARCHAR(20) NOT NULL, "
        "status VARCHAR(20) NOT NULL)",

        "CREATE TABLE IF NOT EXISTS complaints ("
        "id INTEGER PRIMARY KEY AUTOINCREMENT, "
        "student_id VARCHAR(20) NOT NULL, "
        "complaint_text TEXT NOT NULL, "
        "status VARCHAR(20) NOT NULL)"
    };

    int total = sizeof(queries) / sizeof(queries[0]);

    for (int i = 0; i < total; i++)
    {
        char *error = NULL;

        if (sqlite3_exec(conn, queries[i], NULL, NULL, &error) != SQLITE_OK)
        {
            cout << "Could not create tables: " << error << endl;
            sqlite3_free(error);
            exit(1);
        }
    }
}

static void connectToDb()
{
    if (conn != NULL)
    {
        return;
    }

    // HM_DB_FILE can be used to point to another file, normally not needed
    const char *file = getenv("HM_DB_FILE");

    if (file == NULL)
    {
        file = DB_FILE;
    }

    if (sqlite3_open(file, &conn) != SQLITE_OK)
    {
        cout << "Could not open database file " << file << ": " << sqlite3_errmsg(conn) << endl;
        exit(1);
    }

    // make SQLite check the FOREIGN KEY links between tables
    sqlite3_exec(conn, "PRAGMA foreign_keys = ON", NULL, NULL, NULL);

    createTables();
    atexit(dbClose);
}

void dbClose()
{
    if (conn != NULL)
    {
        sqlite3_close(conn);
        conn = NULL;
    }
}

// in SQLite a single quote inside text is written as two quotes
string dbEscape(const string &text)
{
    string result = "";

    for (int i = 0; i < (int)text.length(); i++)
    {
        if (text[i] == '\'')
        {
            result += "''";
        }
        else
        {
            result += text[i];
        }
    }

    return result;
}

bool dbExecute(const string &sql)
{
    connectToDb();

    char *error = NULL;

    if (sqlite3_exec(conn, sql.c_str(), NULL, NULL, &error) != SQLITE_OK)
    {
        cout << "Database error: " << error << endl;
        sqlite3_free(error);
        return false;
    }

    return true;
}

bool dbQuery(const string &sql, vector<vector<string> > &rows)
{
    connectToDb();
    rows.clear();

    sqlite3_stmt *stmt = NULL;

    if (sqlite3_prepare_v2(conn, sql.c_str(), -1, &stmt, NULL) != SQLITE_OK)
    {
        cout << "Database error: " << sqlite3_errmsg(conn) << endl;
        return false;
    }

    int columns = sqlite3_column_count(stmt);

    while (sqlite3_step(stmt) == SQLITE_ROW)
    {
        vector<string> oneRow;

        for (int i = 0; i < columns; i++)
        {
            const unsigned char *text = sqlite3_column_text(stmt, i);

            if (text == NULL)
            {
                oneRow.push_back("");
            }
            else
            {
                oneRow.push_back((const char *)text);
            }
        }

        rows.push_back(oneRow);
    }

    sqlite3_finalize(stmt);
    return true;
}

long long dbLastInsertId()
{
    connectToDb();
    return (long long)sqlite3_last_insert_rowid(conn);
}

bool dbBegin()
{
    return dbExecute("BEGIN");
}

bool dbCommit()
{
    return dbExecute("COMMIT");
}

void dbRollback()
{
    dbExecute("ROLLBACK");
}