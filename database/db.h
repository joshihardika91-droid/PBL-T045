#ifndef DB_H
#define DB_H

#include <iostream>
#include <string>
#include <vector>
using namespace std;

// the database is one file, it is created next to the .exe on first run
#define DB_FILE "hostelmate.db"

// run INSERT / UPDATE / DELETE. returns false on error
bool dbExecute(const string &sql);

// run SELECT. every row is a vector of column values (NULL becomes "")
bool dbQuery(const string &sql, vector<vector<string> > &rows);

// makes text safe to put inside '...' in a query
string dbEscape(const string &text);

// id given by AUTOINCREMENT to the last inserted row
long long dbLastInsertId();

bool dbBegin();
bool dbCommit();
void dbRollback();
void dbClose();

#endif