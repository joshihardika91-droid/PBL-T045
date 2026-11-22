// HostelMate web server
// ---------------------------------------------------------------
// Connects hostelmate_app.html (frontend) to the same SQLite database
// (hostelmate.db) that the console version of HostelMate uses.
// It uses the existing db.cpp / db.h and the same tables and rules as
// the console modules (user, student, room, attendance, fee, leave,
// complaint, mess), but talks JSON over HTTP instead of cin/cout.
//
// Build (Windows / MinGW):
//   g++ -std=c++11 server.cpp db.cpp sqlite3.o -o HostelMateServer.exe -lws2_32
// Build (Linux / macOS):
//   g++ -std=c++11 server.cpp db.cpp sqlite3.o -o hostelmate_server -lpthread -ldl
//
// Run it from the folder that contains hostelmate.db and
// hostelmate_app.html, then open http://localhost:8080

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <ctime>
#include <cmath>
#include <string>
#include <vector>
#include <map>
#include <sstream>
#include <fstream>
#include <algorithm>
#include <random>
#include "db.h"

#ifdef _WIN32
  #include <winsock2.h>
  #include <ws2tcpip.h>
  typedef SOCKET sock_t;
  #define CLOSESOCK closesocket
  #define BAD_SOCK INVALID_SOCKET
#else
  #include <sys/types.h>
  #include <sys/socket.h>
  #include <sys/time.h>
  #include <netinet/in.h>
  #include <arpa/inet.h>
  #include <unistd.h>
  #include <pthread.h>
  typedef int sock_t;
  #define CLOSESOCK close
  #define BAD_SOCK (-1)
#endif

using namespace std;

#define FRONTEND_FILE "Frontend/hostelmate_app.html"

#define ADMIN_USER "ADMIN"
#define ADMIN_PASS "Hosttel@123"
#define RUPEE "\xE2\x82\xB9"

typedef vector<vector<string> > Rows;

// ---------------------------------------------------------------
// small helpers
// ---------------------------------------------------------------
struct Req
{
    string method, path, token, rawBody;
    map<string, string> body;
};

struct Res
{
    int status;
    string body;
    string type;
    Res() : status(200), type("application/json; charset=utf-8") {}
};

struct Session
{
    bool admin;
    string id;
    string name;
};

static map<string, Session> sessions;

static string trim(const string &s)
{
    size_t a = s.find_first_not_of(" \t\r\n");
    if (a == string::npos) return "";
    size_t b = s.find_last_not_of(" \t\r\n");
    return s.substr(a, b - a + 1);
}

static string jq(const string &s)
{
    string o = "\"";
    for (size_t i = 0; i < s.size(); i++)
    {
        unsigned char c = (unsigned char)s[i];
        switch (c)
        {
            case '"':  o += "\\\""; break;
            case '\\': o += "\\\\"; break;
            case '\n': o += "\\n"; break;
            case '\r': o += "\\r"; break;
            case '\t': o += "\\t"; break;
            default:
                if (c < 0x20) { char b[8]; sprintf(b, "\\u%04x", c); o += b; }
                else o += (char)c;
        }
    }
    return o + "\"";
}

static string num(double v)
{
    char b[48];
    sprintf(b, "%.2f", v);
    return b;
}

static string jmoney(const string &dbText)
{
    return num(atof(dbText.c_str()));
}

static Res fail(int status, const string &msg)
{
    Res r;
    r.status = status;
    r.body = "{\"error\":" + jq(msg) + "}";
    return r;
}

static Res okJson(const string &json)
{
    Res r;
    r.body = json;
    return r;
}

static Res okMsg(const string &msg)
{
    return okJson("{\"ok\":true,\"message\":" + jq(msg) + "}");
}

static bool q(const string &sql, Rows &rows)
{
    return dbQuery(sql, rows);
}

static string getBody(const Req &r, const char *key)
{
    map<string, string>::const_iterator it = r.body.find(key);
    return it == r.body.end() ? string("") : it->second;
}

// positive whole number without leading zero, max 9 digits
static bool isId(const string &s)
{
    if (s.empty() || s.size() > 9 || s[0] == '0') return false;
    for (size_t i = 0; i < s.size(); i++)
        if (s[i] < '0' || s[i] > '9') return false;
    return true;
}

static bool parseNumber(const string &s, double &out)
{
    if (s.empty()) return false;
    char *end = NULL;
    out = strtod(s.c_str(), &end);
    if (end == s.c_str() || *end != '\0') return false;
    return !(out != out) && fabs(out) < 1e12;
}

// YYYY-MM-DD, same rule as the attendance module
static bool isValidDate(const string &d)
{
    if (d.size() != 10 || d[4] != '-' || d[7] != '-') return false;
    for (int i = 0; i < 10; i++)
        if (i != 4 && i != 7 && (d[i] < '0' || d[i] > '9')) return false;
    int m = atoi(d.c_str() + 5);
    int day = atoi(d.c_str() + 8);
    return m >= 1 && m <= 12 && day >= 1 && day <= 31;
}

// ---------------------------------------------------------------
// minimal JSON reader: one flat object  {"a":"x","b":12,"c":true}
// ---------------------------------------------------------------
static void putUtf8(string &o, unsigned cp)
{
    if (cp < 0x80) o += (char)cp;
    else if (cp < 0x800) { o += (char)(0xC0 | (cp >> 6)); o += (char)(0x80 | (cp & 0x3F)); }
    else if (cp < 0x10000) { o += (char)(0xE0 | (cp >> 12)); o += (char)(0x80 | ((cp >> 6) & 0x3F)); o += (char)(0x80 | (cp & 0x3F)); }
    else { o += (char)(0xF0 | (cp >> 18)); o += (char)(0x80 | ((cp >> 12) & 0x3F)); o += (char)(0x80 | ((cp >> 6) & 0x3F)); o += (char)(0x80 | (cp & 0x3F)); }
}

static bool hex4(const string &s, size_t i, unsigned &v)
{
    if (i + 4 > s.size()) return false;
    v = 0;
    for (int k = 0; k < 4; k++)
    {
        char c = s[i + k];
        v <<= 4;
        if (c >= '0' && c <= '9') v |= c - '0';
        else if (c >= 'a' && c <= 'f') v |= c - 'a' + 10;
        else if (c >= 'A' && c <= 'F') v |= c - 'A' + 10;
        else return false;
    }
    return true;
}

static bool readString(const string &s, size_t &i, string &out)
{
    if (i >= s.size() || s[i] != '"') return false;
    i++;
    out.clear();
    while (i < s.size())
    {
        char c = s[i++];
        if (c == '"') return true;
        if (c != '\\') { out += c; continue; }
        if (i >= s.size()) return false;
        char e = s[i++];
        switch (e)
        {
            case 'n': out += '\n'; break;
            case 'r': out += '\r'; break;
            case 't': out += '\t'; break;
            case 'b': out += '\b'; break;
            case 'f': out += '\f'; break;
            case '/': out += '/'; break;
            case '\\': out += '\\'; break;
            case '"': out += '"'; break;
            case 'u':
            {
                unsigned cp;
                if (!hex4(s, i, cp)) return false;
                i += 4;
                if (cp >= 0xD800 && cp < 0xDC00 && i + 6 <= s.size() && s[i] == '\\' && s[i + 1] == 'u')
                {
                    unsigned lo;
                    if (hex4(s, i + 2, lo) && lo >= 0xDC00 && lo < 0xE000)
                    {
                        cp = 0x10000 + ((cp - 0xD800) << 10) + (lo - 0xDC00);
                        i += 6;
                    }
                }
                putUtf8(out, cp);
                break;
            }
            default: return false;
        }
    }
    return false;
}

static void skipWs(const string &s, size_t &i)
{
    while (i < s.size() && (s[i] == ' ' || s[i] == '\t' || s[i] == '\r' || s[i] == '\n')) i++;
}

static bool parseJsonObject(const string &s, map<string, string> &out)
{
    size_t i = 0;
    skipWs(s, i);
    if (i >= s.size() || s[i] != '{') return false;
    i++;
    skipWs(s, i);
    if (i < s.size() && s[i] == '}') return true;

    while (i < s.size())
    {
        string key, val;
        skipWs(s, i);
        if (!readString(s, i, key)) return false;
        skipWs(s, i);
        if (i >= s.size() || s[i] != ':') return false;
        i++;
        skipWs(s, i);
        if (i < s.size() && s[i] == '"')
        {
            if (!readString(s, i, val)) return false;
        }
        else
        {
            size_t st = i;
            while (i < s.size() && s[i] != ',' && s[i] != '}' && s[i] != ' ' && s[i] != '\r' && s[i] != '\n' && s[i] != '\t') i++;
            val = s.substr(st, i - st);
            if (val == "null") val = "";
        }
        out[key] = val;
        skipWs(s, i);
        if (i < s.size() && s[i] == ',') { i++; continue; }
        if (i < s.size() && s[i] == '}') return true;
        return false;
    }
    return false;
}

// ---------------------------------------------------------------
// global lock + tokens
// ---------------------------------------------------------------
#ifdef _WIN32
static CRITICAL_SECTION gLock;
static void lockInit()  { InitializeCriticalSection(&gLock); }
static void lockOn()    { EnterCriticalSection(&gLock); }
static void lockOff()   { LeaveCriticalSection(&gLock); }
#else
static pthread_mutex_t gLock = PTHREAD_MUTEX_INITIALIZER;
static void lockInit()  {}
static void lockOn()    { pthread_mutex_lock(&gLock); }
static void lockOff()   { pthread_mutex_unlock(&gLock); }
#endif

static string makeToken()
{
    static mt19937_64 gen;
    static bool seeded = false;
    if (!seeded)
    {
        unsigned long long seed = (unsigned long long)time(NULL) * 6364136223846793005ULL
                                  ^ (unsigned long long)clock() ^ (unsigned long long)(size_t)&seeded;
        FILE *f = fopen("/dev/urandom", "rb");
        if (f)
        {
            unsigned long long r = 0;
            if (fread(&r, sizeof(r), 1, f) == 1) seed ^= r;
            fclose(f);
        }
        gen.seed(seed);
        seeded = true;
    }
    char buf[40];
    sprintf(buf, "%016llx%016llx", (unsigned long long)gen(), (unsigned long long)gen());
    return buf;
}

static Session *getSession(const Req &r)
{
    map<string, Session>::iterator it = sessions.find(r.token);
    return it == sessions.end() ? NULL : &it->second;
}

// returns true when allowed. otherwise "out" holds the error
static bool need(const Req &r, bool adminOnly, bool studentOnly, Session *&s, Res &out)
{
    s = getSession(r);
    if (!s) { out = fail(401, "Please log in again."); return false; }
    if (adminOnly && !s->admin) { out = fail(403, "Admin access required."); return false; }
    if (studentOnly && s->admin) { out = fail(403, "This action is for students only."); return false; }
    return true;
}

// ---------------------------------------------------------------
// mess table (the console Mess class only kept data in memory)
// ---------------------------------------------------------------
static const char *DAYS[7] = {"Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday", "Sunday"};

static void ensureMess()
{
    dbExecute("CREATE TABLE IF NOT EXISTS mess ("
              "day_no INTEGER NOT NULL, meal_no INTEGER NOT NULL, "
              "menu TEXT NOT NULL, timing TEXT NOT NULL, "
              "PRIMARY KEY (day_no, meal_no))");

    Rows rows;
    if (!q("SELECT COUNT(*) FROM mess", rows) || rows.empty() || rows[0][0] != "0") return;

    // same defaults as mess.cpp
    const char *menu[7][4] = {
        {"Paratha, Curd", "Rice, Dal, Roti, Sabzi", "Samosa, Tea", "Rice, Dal, Roti, Paneer"},
        {"Poha, Milk", "Rice, Rajma, Roti, Salad", "Biscuits, Tea", "Roti, Dal, Mix Veg"},
        {"Aloo Paratha, Curd", "Rice, Dal, Chole, Roti", "Pakora, Tea", "Rice, Dal, Roti, Sabzi"},
        {"Bread, Butter, Milk", "Rice, Dal, Roti, Paneer", "Sandwich, Tea", "Roti, Dal, Aloo Sabzi"},
        {"Poha, Tea", "Rice, Dal, Roti, Chole", "Samosa, Tea", "Rice, Dal, Roti, Mix Veg"},
        {"Paratha, Curd", "Rice, Dal, Roti, Sabzi", "Bread Pakora, Tea", "Pulao, Raita"},
        {"Puri, Sabzi", "Rice, Dal, Paneer, Roti", "Cake, Tea", "Special Dinner"}};
    const char *timing[4] = {"8:00 AM - 9:00 AM", "1:00 PM - 2:00 PM", "5:00 PM - 6:00 PM", "8:00 PM - 9:00 PM"};

    dbBegin();
    for (int d = 0; d < 7; d++)
        for (int m = 0; m < 4; m++)
        {
            string sql = "INSERT INTO mess (day_no, meal_no, menu, timing) VALUES (" + to_string(d) + ", " + to_string(m) +
                         ", '" + dbEscape(menu[d][m]) + "', '" + dbEscape(timing[m]) + "')";
            if (!dbExecute(sql)) { dbRollback(); return; }
        }
    dbCommit();
}

// ---------------------------------------------------------------
// handlers: login / registration / profile
// ---------------------------------------------------------------
static Res hAdminLogin(const Req &req)
{
    if (getBody(req, "username") == ADMIN_USER && getBody(req, "password") == ADMIN_PASS)
    {
        Session s; s.admin = true; s.id = ""; s.name = "Administrator";
        string t = makeToken();
        sessions[t] = s;
        return okJson("{\"token\":" + jq(t) + ",\"role\":\"admin\"}");
    }
    return fail(401, "Invalid admin username or password!");
}

static Res hStudentLogin(const Req &req)
{
    string id = trim(getBody(req, "id"));
    string pass = getBody(req, "password");

    Rows rows;
    if (!q("SELECT user_id, name, password, status FROM users WHERE role = 'student' AND user_id = '" + dbEscape(id) + "'", rows))
        return fail(500, "Database error.");

    if (rows.empty() || rows[0][2] != pass) return fail(401, "Invalid Student ID or Password.");
    if (rows[0][3] == "Pending") return fail(403, "Account pending admin approval.");
    if (rows[0][3] != "Approved") return fail(403, "Your registration was rejected. Contact the hostel admin.");

    string course = "Not assigned", room = "Not assigned";
    Rows det;
    if (q("SELECT course, room_no FROM student_details WHERE student_id = " + to_string(atoi(id.c_str())), det) && !det.empty())
    {
        course = det[0][0];
        room = det[0][1];
    }

    Session s; s.admin = false; s.id = rows[0][0]; s.name = rows[0][1];
    string t = makeToken();
    sessions[t] = s;
    return okJson("{\"token\":" + jq(t) + ",\"role\":\"student\",\"id\":" + jq(s.id) + ",\"name\":" + jq(s.name) +
                  ",\"course\":" + jq(course) + ",\"roomNo\":" + jq(room) + "}");
}

static Res hRegister(const Req &req)
{
    string id = trim(getBody(req, "id"));
    string name = trim(getBody(req, "name"));
    string pass = getBody(req, "password");

    if (id.empty() || name.empty() || pass.empty()) return fail(400, "Please fill all fields.");
    if (!isId(id)) return fail(400, "Student ID must be a positive number (e.g. 15).");
    if (name.size() > 100 || pass.size() > 100) return fail(400, "Name or password is too long.");

    Rows rows;
    if (!q("SELECT 1 FROM users WHERE user_id = '" + dbEscape(id) + "'", rows)) return fail(500, "Database error.");
    if (!rows.empty()) return fail(409, "User ID already exists.");

    string sql = "INSERT INTO users (user_id, name, password, role, status) VALUES ('" + dbEscape(id) + "', '" +
                 dbEscape(name) + "', '" + dbEscape(pass) + "', 'student', 'Pending')";
    if (!dbExecute(sql)) return fail(500, "Could not save registration.");
    return okMsg("Registration submitted. Wait for admin approval.");
}

static Res hLogout(const Req &req)
{
    sessions.erase(req.token);
    return okMsg("Logged out.");
}

static Res hProfile(const Req &req)
{
    Session *s; Res out;
    if (!need(req, false, true, s, out)) return out;

    Rows rows;
    if (!q("SELECT name, status FROM users WHERE user_id = '" + dbEscape(s->id) + "'", rows) || rows.empty())
        return fail(404, "Student not found.");

    string course = "Not assigned", room = "Not assigned";
    Rows det;
    if (q("SELECT course, room_no FROM student_details WHERE student_id = " + to_string(atoi(s->id.c_str())), det) && !det.empty())
    {
        course = det[0][0];
        room = det[0][1];
    }
    return okJson("{\"id\":" + jq(s->id) + ",\"name\":" + jq(rows[0][0]) + ",\"course\":" + jq(course) +
                  ",\"roomNo\":" + jq(room) + ",\"status\":" + jq(rows[0][1]) + "}");
}

static Res hChangePassword(const Req &req)
{
    Session *s; Res out;
    if (!need(req, false, true, s, out)) return out;

    string oldP = getBody(req, "oldPassword");
    string newP = getBody(req, "newPassword");

    Rows rows;
    if (!q("SELECT password FROM users WHERE user_id = '" + dbEscape(s->id) + "'", rows) || rows.empty())
        return fail(404, "Student not found.");
    if (rows[0][0] != oldP) return fail(400, "Current password incorrect.");
    if (newP.empty()) return fail(400, "New password cannot be empty.");
    if (newP.size() > 100) return fail(400, "New password is too long.");

    if (!dbExecute("UPDATE users SET password = '" + dbEscape(newP) + "' WHERE user_id = '" + dbEscape(s->id) + "'"))
        return fail(500, "Could not update password.");
    return okMsg("Password updated successfully!");
}

// ---------------------------------------------------------------
// handlers: student approvals
// ---------------------------------------------------------------
static Res hStudents(const Req &req)
{
    Session *s; Res out;
    if (!need(req, true, false, s, out)) return out;

    Rows rows;
    if (!q("SELECT u.user_id, u.name, u.status, COALESCE(d.course, 'Not assigned'), COALESCE(d.room_no, 'Not assigned') "
           "FROM users u LEFT JOIN student_details d ON d.student_id = CAST(u.user_id AS INTEGER) "
           "WHERE u.role = 'student' ORDER BY u.id", rows))
        return fail(500, "Database error.");

    string j = "[";
    for (size_t i = 0; i < rows.size(); i++)
    {
        if (i) j += ",";
        j += "{\"id\":" + jq(rows[i][0]) + ",\"name\":" + jq(rows[i][1]) + ",\"status\":" + jq(rows[i][2]) +
             ",\"course\":" + jq(rows[i][3]) + ",\"roomNo\":" + jq(rows[i][4]) + "}";
    }
    return okJson(j + "]");
}

static Res hApprove(const Req &req)
{
    Session *s; Res out;
    if (!need(req, true, false, s, out)) return out;

    string id = trim(getBody(req, "id"));
    string course = trim(getBody(req, "course"));
    if (course.empty()) return fail(400, "Please enter the student's course.");
    if (course.size() > 100) return fail(400, "Course name is too long.");
    if (!isId(id)) return fail(400, "Invalid student ID.");

    Rows rows;
    if (!q("SELECT status FROM users WHERE role = 'student' AND user_id = '" + dbEscape(id) + "'", rows)) return fail(500, "Database error.");
    if (rows.empty() || rows[0][0] != "Pending") return fail(404, "Student not found in pending registrations.");

    dbBegin();
    string sql = "INSERT INTO student_details (student_id, course, room_no) VALUES (" + id + ", '" + dbEscape(course) +
                 "', 'Not assigned') ON CONFLICT(student_id) DO UPDATE SET course = excluded.course";
    if (!dbExecute(sql) || !dbExecute("UPDATE users SET status = 'Approved' WHERE user_id = '" + dbEscape(id) + "'"))
    {
        dbRollback();
        return fail(500, "Could not approve student.");
    }
    dbCommit();
    return okMsg("Student approved successfully.");
}

static Res hReject(const Req &req)
{
    Session *s; Res out;
    if (!need(req, true, false, s, out)) return out;

    string id = trim(getBody(req, "id"));
    Rows rows;
    if (!q("SELECT status FROM users WHERE role = 'student' AND user_id = '" + dbEscape(id) + "'", rows)) return fail(500, "Database error.");
    if (rows.empty() || rows[0][0] != "Pending") return fail(404, "Student not found in pending registrations.");

    if (!dbExecute("UPDATE users SET status = 'Rejected' WHERE user_id = '" + dbEscape(id) + "'"))
        return fail(500, "Could not reject student.");
    return okMsg("Student rejected.");
}

// ---------------------------------------------------------------
// handlers: rooms  (Hostel -> Floor -> Room, request queue)
// ---------------------------------------------------------------
static bool studentHasRoom(const string &id)
{
    Rows rows;
    if (!q("SELECT room_no FROM student_details WHERE student_id = " + id, rows) || rows.empty()) return false;
    return rows[0][0] != "" && rows[0][0] != "Not assigned";
}

static Res hRooms(const Req &req)
{
    Session *s; Res out;
    if (!need(req, false, false, s, out)) return out;

    Rows rooms, res;
    if (!q("SELECT hostel, floor_name, room_no FROM rooms ORDER BY id", rooms)) return fail(500, "Database error.");
    if (!q("SELECT room_no, student_id FROM room_residents ORDER BY id", res)) return fail(500, "Database error.");

    string j = "[";
    for (size_t i = 0; i < rooms.size(); i++)
    {
        if (i) j += ",";
        j += "{\"hostel\":" + jq(rooms[i][0]) + ",\"floor\":" + jq(rooms[i][1]) + ",\"roomNo\":" + jq(rooms[i][2]) + ",\"students\":[";
        bool first = true;
        for (size_t k = 0; k < res.size(); k++)
            if (res[k][0] == rooms[i][2])
            {
                if (!first) j += ",";
                j += jq(res[k][1]);
                first = false;
            }
        j += "]}";
    }
    return okJson(j + "]");
}

static Res hAddRoom(const Req &req)
{
    Session *s; Res out;
    if (!need(req, true, false, s, out)) return out;

    string hostel = trim(getBody(req, "hostel"));
    string floor = trim(getBody(req, "floor"));
    string roomNo = trim(getBody(req, "roomNo"));
    if (hostel.empty() || floor.empty() || roomNo.empty()) return fail(400, "Please fill hostel, floor and room number.");
    if (hostel.size() > 50 || floor.size() > 20 || roomNo.size() > 20) return fail(400, "Hostel, floor or room number is too long.");

    Rows rows;
    if (!q("SELECT 1 FROM rooms WHERE room_no = '" + dbEscape(roomNo) + "'", rows)) return fail(500, "Database error.");
    if (!rows.empty()) return fail(409, "Room number already exists.");

    string sql = "INSERT INTO rooms (hostel, floor_name, room_no) VALUES ('" + dbEscape(hostel) + "', '" +
                 dbEscape(floor) + "', '" + dbEscape(roomNo) + "')";
    if (!dbExecute(sql)) return fail(500, "Could not add room.");
    return okMsg("Room " + roomNo + " added to " + hostel + "!");
}

static Res hRoomRequests(const Req &req)
{
    Session *s; Res out;
    if (!need(req, true, false, s, out)) return out;

    Rows rows;
    if (!q("SELECT request_id, student_id, room_no, status FROM room_requests WHERE status = 'Pending' ORDER BY request_id", rows))
        return fail(500, "Database error.");

    string j = "[";
    for (size_t i = 0; i < rows.size(); i++)
    {
        if (i) j += ",";
        j += "{\"id\":" + rows[i][0] + ",\"studentId\":" + jq(rows[i][1]) + ",\"roomNo\":" + jq(rows[i][2]) +
             ",\"status\":" + jq(rows[i][3]) + "}";
    }
    return okJson(j + "]");
}

static Res hSubmitRoomRequest(const Req &req)
{
    Session *s; Res out;
    if (!need(req, false, true, s, out)) return out;

    string roomNo = trim(getBody(req, "roomNo"));
    if (roomNo.empty()) return fail(400, "Please enter a room number.");
    if (studentHasRoom(s->id)) return fail(409, "You already have a room assigned.");

    Rows rows;
    if (!q("SELECT 1 FROM rooms WHERE room_no = '" + dbEscape(roomNo) + "'", rows)) return fail(500, "Database error.");
    if (rows.empty()) return fail(404, "Room does not exist.");

    if (!q("SELECT 1 FROM room_requests WHERE student_id = " + s->id + " AND status = 'Pending'", rows)) return fail(500, "Database error.");
    if (!rows.empty()) return fail(409, "You already have a pending request.");

    string sql = "INSERT INTO room_requests (request_id, student_id, room_no, status) VALUES "
                 "((SELECT COALESCE(MAX(request_id), 0) + 1 FROM room_requests), " + s->id + ", '" + dbEscape(roomNo) + "', 'Pending')";
    if (!dbExecute(sql)) return fail(500, "Could not submit request.");
    return okMsg("Room request submitted!");
}

static Res hProcessRoomRequest(const Req &req)
{
    Session *s; Res out;
    if (!need(req, true, false, s, out)) return out;

    string id = trim(getBody(req, "id"));
    string action = getBody(req, "action");
    if (!isId(id) || (action != "approve" && action != "reject")) return fail(400, "Invalid request.");

    Rows rows;
    if (!q("SELECT student_id, room_no, status FROM room_requests WHERE request_id = " + id, rows)) return fail(500, "Database error.");
    if (rows.empty()) return fail(404, "Request not found.");
    if (rows[0][2] != "Pending") return fail(409, "This request was already processed.");

    string studentId = rows[0][0];
    string roomNo = rows[0][1];

    if (action == "reject")
    {
        if (!dbExecute("UPDATE room_requests SET status = 'Rejected' WHERE request_id = " + id)) return fail(500, "Database error.");
        return okMsg("Request rejected.");
    }

    // approval is not possible -> reject so it does not block the queue (same as console version)
    if (studentHasRoom(studentId))
    {
        dbExecute("UPDATE room_requests SET status = 'Rejected' WHERE request_id = " + id);
        return fail(409, "Student already has a room. Request rejected.");
    }

    Rows r2;
    if (!q("SELECT 1 FROM rooms WHERE room_no = '" + dbEscape(roomNo) + "'", r2)) return fail(500, "Database error.");
    if (r2.empty())
    {
        dbExecute("UPDATE room_requests SET status = 'Rejected' WHERE request_id = " + id);
        return fail(409, "Room no longer exists. Request rejected.");
    }

    if (!q("SELECT 1 FROM student_details WHERE student_id = " + studentId, r2)) return fail(500, "Database error.");
    if (r2.empty()) return fail(404, "Student record not found.");

    dbBegin();
    if (!dbExecute("UPDATE student_details SET room_no = '" + dbEscape(roomNo) + "' WHERE student_id = " + studentId) ||
        !dbExecute("INSERT OR IGNORE INTO room_residents (room_no, student_id) VALUES ('" + dbEscape(roomNo) + "', " + studentId + ")") ||
        !dbExecute("UPDATE room_requests SET status = 'Approved' WHERE request_id = " + id))
    {
        dbRollback();
        return fail(500, "Could not allocate room.");
    }
    dbCommit();
    return okMsg("Room allocated successfully.");
}

// ---------------------------------------------------------------
// handlers: attendance (with undo stack like the console version)
// ---------------------------------------------------------------
struct UndoAction
{
    string id, date;
    bool existed;
    string oldStatus;
};
static vector<UndoAction> undoStack;

static Res hAttendanceList(const Req &req)
{
    Session *s; Res out;
    if (!need(req, false, false, s, out)) return out;

    string sql = "SELECT student_id, student_name, att_date, status FROM attendance ";
    if (!s->admin) sql += "WHERE student_id = '" + dbEscape(s->id) + "' ";
    sql += "ORDER BY student_id, att_date";

    Rows rows;
    if (!q(sql, rows)) return fail(500, "Database error.");

    string j = "[";
    for (size_t i = 0; i < rows.size(); i++)
    {
        if (i) j += ",";
        j += "{\"id\":" + jq(rows[i][0]) + ",\"name\":" + jq(rows[i][1]) + ",\"date\":" + jq(rows[i][2]) +
             ",\"status\":" + jq(rows[i][3] == "P" ? "Present" : "Absent") + "}";
    }
    return okJson(j + "]");
}

static Res hMarkAttendance(const Req &req)
{
    Session *s; Res out;
    if (!need(req, true, false, s, out)) return out;

    string id = trim(getBody(req, "id"));
    string date = trim(getBody(req, "date"));
    string st = getBody(req, "status");

    if (id.empty() || id.size() > 19) return fail(400, "Please enter a valid student ID.");
    if (!isValidDate(date)) return fail(400, "Invalid date. Use YYYY-MM-DD.");
    string code;
    if (st == "Present") code = "P";
    else if (st == "Absent") code = "A";
    else return fail(400, "Status must be Present or Absent.");

    string name = "Student " + id;
    Rows rows;
    if (q("SELECT name FROM users WHERE user_id = '" + dbEscape(id) + "'", rows) && !rows.empty()) name = rows[0][0];
    if (name.size() > 49) name = name.substr(0, 49);

    UndoAction a;
    a.id = id; a.date = date; a.existed = false; a.oldStatus = "";
    if (q("SELECT status FROM attendance WHERE student_id = '" + dbEscape(id) + "' AND att_date = '" + dbEscape(date) + "'", rows) && !rows.empty())
    {
        a.existed = true;
        a.oldStatus = rows[0][0];
    }

    string sql = "INSERT INTO attendance (student_id, student_name, att_date, status) VALUES ('" + dbEscape(id) + "', '" +
                 dbEscape(name) + "', '" + dbEscape(date) + "', '" + code + "') "
                 "ON CONFLICT(student_id, att_date) DO UPDATE SET status = excluded.status, student_name = excluded.student_name";
    if (!dbExecute(sql)) return fail(500, "Could not save attendance.");

    undoStack.push_back(a);
    return okMsg("Attendance marked for " + id + " as " + st + ".");
}

static Res hUndoAttendance(const Req &req)
{
    Session *s; Res out;
    if (!need(req, true, false, s, out)) return out;

    if (undoStack.empty()) return fail(409, "Nothing to undo.");
    UndoAction a = undoStack.back();
    undoStack.pop_back();

    bool ok;
    if (a.existed)
        ok = dbExecute("UPDATE attendance SET status = '" + dbEscape(a.oldStatus) + "' WHERE student_id = '" + dbEscape(a.id) +
                       "' AND att_date = '" + dbEscape(a.date) + "'");
    else
        ok = dbExecute("DELETE FROM attendance WHERE student_id = '" + dbEscape(a.id) + "' AND att_date = '" + dbEscape(a.date) + "'");
    if (!ok) return fail(500, "Could not undo.");
    return okMsg("Undid last attendance mark.");
}

// ---------------------------------------------------------------
// handlers: mess
// ---------------------------------------------------------------
static Res hMessGet(const Req &req)
{
    Session *s; Res out;
    if (!need(req, false, false, s, out)) return out;

    Rows rows;
    if (!q("SELECT day_no, meal_no, menu, timing FROM mess ORDER BY day_no, meal_no", rows)) return fail(500, "Database error.");

    static const char *keys[4] = {"breakfast", "lunch", "snacks", "dinner"};
    string j = "[";
    for (int d = 0; d < 7; d++)
    {
        if (d) j += ",";
        j += "{\"day\":" + jq(DAYS[d]);
        for (int m = 0; m < 4; m++)
        {
            string text = "";
            for (size_t i = 0; i < rows.size(); i++)
                if (rows[i][0] == to_string(d) && rows[i][1] == to_string(m))
                    text = rows[i][2] + " (" + rows[i][3] + ")";
            j += string(",\"") + keys[m] + "\":" + jq(text);
        }
        j += "}";
    }
    return okJson(j + "]");
}

// "Rice, Dal (1:00 PM - 2:00 PM)" -> menu + timing
static void splitMeal(const string &text, string &menu, string &timing, bool &hasTiming)
{
    string t = trim(text);
    size_t open = t.rfind('(');
    if (!t.empty() && t[t.size() - 1] == ')' && open != string::npos)
    {
        menu = trim(t.substr(0, open));
        timing = trim(t.substr(open + 1, t.size() - open - 2));
        hasTiming = !timing.empty();
    }
    else
    {
        menu = t;
        timing = "";
        hasTiming = false;
    }
}

static Res hMessSave(const Req &req)
{
    Session *s; Res out;
    if (!need(req, true, false, s, out)) return out;

    static const char *keys[4] = {"breakfast", "lunch", "snacks", "dinner"};

    dbBegin();
    for (int d = 0; d < 7; d++)
        for (int m = 0; m < 4; m++)
        {
            string key = string(keys[m]) + to_string(d);
            if (req.body.find(key) == req.body.end()) continue;

            string menu, timing;
            bool hasTiming;
            splitMeal(getBody(req, key.c_str()), menu, timing, hasTiming);
            if (menu.empty()) { dbRollback(); return fail(400, string("Menu for ") + DAYS[d] + " " + keys[m] + " cannot be empty."); }
            if (menu.size() > 200 || timing.size() > 50) { dbRollback(); return fail(400, "A menu or timing entry is too long."); }

            string sql = "UPDATE mess SET menu = '" + dbEscape(menu) + "'";
            if (hasTiming) sql += ", timing = '" + dbEscape(timing) + "'";
            sql += " WHERE day_no = " + to_string(d) + " AND meal_no = " + to_string(m);
            if (!dbExecute(sql)) { dbRollback(); return fail(500, "Could not save mess menu."); }
        }
    dbCommit();
    return okMsg("Mess timetable saved!");
}

// ---------------------------------------------------------------
// handlers: fees
// ---------------------------------------------------------------
static Res hFeesList(const Req &req)
{
    Session *s; Res out;
    if (!need(req, false, false, s, out)) return out;

    string sql = "SELECT f.student_id, COALESCE(u.name, 'Student ' || f.student_id), f.fee_amount, f.due, f.status, f.fee_date "
                 "FROM fees f LEFT JOIN users u ON u.user_id = CAST(f.student_id AS TEXT) ";
    if (!s->admin) sql += "WHERE f.student_id = " + to_string(atoi(s->id.c_str())) + " ";
    sql += "ORDER BY f.pos, f.student_id";

    Rows rows, pays;
    if (!q(sql, rows)) return fail(500, "Database error.");
    if (!q("SELECT student_id, amount FROM fee_payments ORDER BY student_id, pay_no", pays)) return fail(500, "Database error.");

    string j = "[";
    for (size_t i = 0; i < rows.size(); i++)
    {
        if (i) j += ",";
        j += "{\"id\":" + jq(rows[i][0]) + ",\"name\":" + jq(rows[i][1]) + ",\"total\":" + jmoney(rows[i][2]) +
             ",\"due\":" + jmoney(rows[i][3]) + ",\"status\":" + jq(rows[i][4]) + ",\"date\":" + jq(rows[i][5]) + ",\"payments\":[";
        bool first = true;
        for (size_t k = 0; k < pays.size(); k++)
            if (pays[k][0] == rows[i][0])
            {
                if (!first) j += ",";
                j += jmoney(pays[k][1]);
                first = false;
            }
        j += "]}";
    }
    return okJson(j + "]");
}

static Res hAddFee(const Req &req)
{
    Session *s; Res out;
    if (!need(req, true, false, s, out)) return out;

    string id = trim(getBody(req, "id"));
    string date = trim(getBody(req, "date"));
    double total;

    if (!isId(id)) return fail(400, "Invalid student ID.");
    if (!parseNumber(getBody(req, "total"), total) || total < 0) return fail(400, "Invalid fee amount.");
    if (date.empty() || date.size() > 50) return fail(400, "Please enter the fee date.");
    total = floor(total * 100 + 0.5) / 100;

    Rows rows;
    if (!q("SELECT 1 FROM fees WHERE student_id = " + id, rows)) return fail(500, "Database error.");
    if (!rows.empty()) return fail(409, "A fee record already exists for this student.");

    string sql = "INSERT INTO fees (student_id, fee_amount, fee_date, due, status, pos) VALUES (" + id + ", " + num(total) + ", '" +
                 dbEscape(date) + "', " + num(total) + ", '" + (total == 0 ? "Paid" : "Pending") + "', "
                 "(SELECT COALESCE(MAX(pos), -1) + 1 FROM fees))";
    if (!dbExecute(sql)) return fail(500, "Could not save fee record.");
    return okMsg("Fee record added!");
}

static Res hPayFee(const Req &req)
{
    Session *s; Res out;
    if (!need(req, false, true, s, out)) return out;

    double amount;
    if (!parseNumber(getBody(req, "amount"), amount) || amount <= 0) return fail(400, "Invalid payment amount.");
    amount = floor(amount * 100 + 0.5) / 100;
    if (amount <= 0) return fail(400, "Invalid payment amount.");

    string id = to_string(atoi(s->id.c_str()));
    Rows rows;
    if (!q("SELECT due FROM fees WHERE student_id = " + id, rows)) return fail(500, "Database error.");
    if (rows.empty()) return fail(404, "No fee record found for this student.");

    double due = atof(rows[0][0].c_str());
    if (due <= 0) return fail(409, "All fees have been paid.");
    if (amount > due + 0.001) return fail(400, string("Amount exceeds due ") + RUPEE + num(due));

    Rows cnt;
    if (!q("SELECT COUNT(*) FROM fee_payments WHERE student_id = " + id, cnt) || cnt.empty()) return fail(500, "Database error.");
    int payNo = atoi(cnt[0][0].c_str()) + 1;

    double remaining = due - amount;
    if (remaining < 0.005) remaining = 0;
    remaining = floor(remaining * 100 + 0.5) / 100;
    string status = remaining == 0 ? "Paid" : "Partial";

    dbBegin();
    if (!dbExecute("INSERT INTO fee_payments (student_id, pay_no, amount) VALUES (" + id + ", " + to_string(payNo) + ", " + num(amount) + ")") ||
        !dbExecute("UPDATE fees SET due = " + num(remaining) + ", status = '" + status + "' WHERE student_id = " + id))
    {
        dbRollback();
        return fail(500, "Payment could not be saved.");
    }
    dbCommit();
    return okMsg("Payment of " + string(RUPEE) + num(amount) + " successful!");
}

// merge sort (highest due first), same algorithm as the console version
struct FeeRow { string id; double due; };

static void mergeFees(vector<FeeRow> &a, int l, int m, int r)
{
    vector<FeeRow> L(a.begin() + l, a.begin() + m + 1);
    vector<FeeRow> R(a.begin() + m + 1, a.begin() + r + 1);
    size_t i = 0, j = 0;
    int k = l;
    while (i < L.size() && j < R.size())
    {
        if (L[i].due >= R[j].due) a[k++] = L[i++];
        else a[k++] = R[j++];
    }
    while (i < L.size()) a[k++] = L[i++];
    while (j < R.size()) a[k++] = R[j++];
}

static void mergeSortFees(vector<FeeRow> &a, int l, int r)
{
    if (l >= r) return;
    int m = l + (r - l) / 2;
    mergeSortFees(a, l, m);
    mergeSortFees(a, m + 1, r);
    mergeFees(a, l, m, r);
}

static Res hSortFees(const Req &req)
{
    Session *s; Res out;
    if (!need(req, true, false, s, out)) return out;

    Rows rows;
    if (!q("SELECT student_id, due FROM fees ORDER BY pos, student_id", rows)) return fail(500, "Database error.");
    if (rows.empty()) return okMsg("No fee records available.");

    vector<FeeRow> recs;
    for (size_t i = 0; i < rows.size(); i++)
    {
        FeeRow f; f.id = rows[i][0]; f.due = atof(rows[i][1].c_str());
        recs.push_back(f);
    }
    mergeSortFees(recs, 0, (int)recs.size() - 1);

    dbBegin();
    for (size_t i = 0; i < recs.size(); i++)
        if (!dbExecute("UPDATE fees SET pos = " + to_string((int)i) + " WHERE student_id = " + recs[i].id))
        {
            dbRollback();
            return fail(500, "Error saving sorted fee records.");
        }
    dbCommit();
    return okMsg("Fee records sorted by due amount using Merge Sort.");
}

// ---------------------------------------------------------------
// handlers: leaves (urgent first, then oldest first)
// ---------------------------------------------------------------
static Res hLeavesList(const Req &req)
{
    Session *s; Res out;
    if (!need(req, false, false, s, out)) return out;

    string sql = "SELECT leave_id, type, student_id, student_name, reason, from_date, to_date, status FROM leaves ";
    if (!s->admin) sql += "WHERE student_id = '" + dbEscape(s->id) + "' ";
    sql += "ORDER BY type DESC, leave_id ASC";

    Rows rows;
    if (!q(sql, rows)) return fail(500, "Database error.");

    string j = "[";
    for (size_t i = 0; i < rows.size(); i++)
    {
        if (i) j += ",";
        j += "{\"id\":" + rows[i][0] + ",\"type\":" + rows[i][1] + ",\"studentId\":" + jq(rows[i][2]) + ",\"name\":" + jq(rows[i][3]) +
             ",\"reason\":" + jq(rows[i][4]) + ",\"from\":" + jq(rows[i][5]) + ",\"to\":" + jq(rows[i][6]) +
             ",\"status\":" + jq(rows[i][7]) + "}";
    }
    return okJson(j + "]");
}

static Res hApplyLeave(const Req &req)
{
    Session *s; Res out;
    if (!need(req, false, true, s, out)) return out;

    string from = trim(getBody(req, "from"));
    string to = trim(getBody(req, "to"));
    string reason = trim(getBody(req, "reason"));
    int type = atoi(getBody(req, "type").c_str()) == 2 ? 2 : 1;

    if (from.empty() || to.empty() || reason.empty()) return fail(400, "Please fill all leave details.");
    if (!isValidDate(from) || !isValidDate(to)) return fail(400, "Invalid date. Use YYYY-MM-DD.");
    if (to < from) return fail(400, "End date cannot be before start date.");
    if (reason.size() > 500) return fail(400, "Reason is too long.");

    string sql = "INSERT INTO leaves (leave_id, type, student_id, student_name, reason, from_date, to_date, status) VALUES ("
                 "(SELECT COALESCE(MAX(leave_id), 0) + 1 FROM leaves), " + to_string(type) + ", '" + dbEscape(s->id) + "', '" +
                 dbEscape(s->name) + "', '" + dbEscape(reason) + "', '" + dbEscape(from) + "', '" + dbEscape(to) + "', 'Pending')";
    if (!dbExecute(sql)) return fail(500, "Could not save leave application.");
    return okMsg("Leave application submitted!");
}

static Res hDecideLeave(const Req &req)
{
    Session *s; Res out;
    if (!need(req, true, false, s, out)) return out;

    string id = trim(getBody(req, "id"));
    string status = getBody(req, "status");
    if (!isId(id) || (status != "Approved" && status != "Rejected")) return fail(400, "Invalid request.");

    Rows rows;
    if (!q("SELECT status FROM leaves WHERE leave_id = " + id, rows)) return fail(500, "Database error.");
    if (rows.empty()) return fail(404, "Leave not found.");
    if (rows[0][0] != "Pending") return fail(409, "This leave was already decided.");

    if (!dbExecute("UPDATE leaves SET status = '" + status + "' WHERE leave_id = " + id)) return fail(500, "Could not update leave.");
    return okMsg("Leave " + status + ".");
}

// ---------------------------------------------------------------
// handlers: complaints (FIFO queue)
// ---------------------------------------------------------------
static Res hComplaintsList(const Req &req)
{
    Session *s; Res out;
    if (!need(req, false, false, s, out)) return out;

    string sql = "SELECT id, student_id, complaint_text, status FROM complaints ";
    if (!s->admin) sql += "WHERE student_id = '" + dbEscape(s->id) + "' ";
    sql += "ORDER BY id";

    Rows rows;
    if (!q(sql, rows)) return fail(500, "Database error.");

    string j = "[";
    for (size_t i = 0; i < rows.size(); i++)
    {
        if (i) j += ",";
        j += "{\"id\":" + rows[i][0] + ",\"studentId\":" + jq(rows[i][1]) + ",\"details\":" + jq(rows[i][2]) +
             ",\"status\":" + jq(rows[i][3]) + "}";
    }
    return okJson(j + "]");
}

static Res hSubmitComplaint(const Req &req)
{
    Session *s; Res out;
    if (!need(req, false, true, s, out)) return out;

    string details = trim(getBody(req, "details"));
    if (details.empty()) return fail(400, "Please describe your complaint.");
    if (details.size() > 1000) return fail(400, "Complaint is too long.");

    Rows rows;
    if (!q("SELECT COUNT(*) FROM complaints WHERE status = 'Pending'", rows) || rows.empty()) return fail(500, "Database error.");
    if (atoi(rows[0][0].c_str()) >= 100) return fail(409, "Complaint queue is full!");

    string sql = "INSERT INTO complaints (student_id, complaint_text, status) VALUES ('" + dbEscape(s->id) + "', '" +
                 dbEscape(details) + "', 'Pending')";
    if (!dbExecute(sql)) return fail(500, "Could not save complaint.");
    return okMsg("Complaint registered in queue!");
}

static Res hResolveComplaint(const Req &req, bool next)
{
    Session *s; Res out;
    if (!need(req, true, false, s, out)) return out;

    string id = trim(getBody(req, "id"));
    Rows rows;
    string sql = "SELECT id, student_id FROM complaints WHERE status = 'Pending' ";
    if (next) sql += "ORDER BY id LIMIT 1";
    else
    {
        if (!isId(id)) return fail(400, "Invalid complaint.");
        sql += "AND id = " + id;
    }

    if (!q(sql, rows)) return fail(500, "Database error.");
    if (rows.empty()) return fail(404, next ? "No complaints to resolve!" : "Complaint not found or already resolved.");

    if (!dbExecute("UPDATE complaints SET status = 'Resolved' WHERE id = " + rows[0][0])) return fail(500, "Could not resolve complaint.");
    return okJson("{\"ok\":true,\"studentId\":" + jq(rows[0][1]) + ",\"message\":" +
                  jq("Resolved complaint from Student " + rows[0][1]) + "}");
}

// ---------------------------------------------------------------
// router
// ---------------------------------------------------------------
static Res serveFrontend()
{
    ifstream f(FRONTEND_FILE, ios::binary);
    Res r;
    if (!f)
    {
        r.status = 404;
        r.type = "text/plain; charset=utf-8";
        r.body = string("Could not find ") + FRONTEND_FILE + ". Put it in the same folder as the server and restart.";
        return r;
    }
    stringstream ss;
    ss << f.rdbuf();
    r.type = "text/html; charset=utf-8";
    r.body = ss.str();
    return r;
}

static Res route(const Req &req)
{
    const string &m = req.method;
    const string &p = req.path;

    if (m == "GET" && (p == "/" || p == "/index.html" || p == "/" FRONTEND_FILE)) return serveFrontend();
    if (p.compare(0, 5, "/api/") != 0) return fail(404, "Not found.");

    if (m == "GET"  && p == "/api/ping")                    return okMsg("HostelMate server is running.");

    if (m == "POST" && p == "/api/admin/login")             return hAdminLogin(req);
    if (m == "POST" && p == "/api/student/login")           return hStudentLogin(req);
    if (m == "POST" && p == "/api/register")                return hRegister(req);
    if (m == "POST" && p == "/api/logout")                  return hLogout(req);

    if (m == "GET"  && p == "/api/profile")                 return hProfile(req);
    if (m == "POST" && p == "/api/password")                return hChangePassword(req);

    if (m == "GET"  && p == "/api/students")                return hStudents(req);
    if (m == "POST" && p == "/api/students/approve")        return hApprove(req);
    if (m == "POST" && p == "/api/students/reject")         return hReject(req);

    if (m == "GET"  && p == "/api/rooms")                   return hRooms(req);
    if (m == "POST" && p == "/api/rooms")                   return hAddRoom(req);
    if (m == "GET"  && p == "/api/room-requests")           return hRoomRequests(req);
    if (m == "POST" && p == "/api/room-requests")           return hSubmitRoomRequest(req);
    if (m == "POST" && p == "/api/room-requests/process")   return hProcessRoomRequest(req);

    if (m == "GET"  && p == "/api/attendance")              return hAttendanceList(req);
    if (m == "POST" && p == "/api/attendance")              return hMarkAttendance(req);
    if (m == "POST" && p == "/api/attendance/undo")         return hUndoAttendance(req);

    if (m == "GET"  && p == "/api/mess")                    return hMessGet(req);
    if (m == "POST" && p == "/api/mess")                    return hMessSave(req);

    if (m == "GET"  && p == "/api/fees")                    return hFeesList(req);
    if (m == "POST" && p == "/api/fees")                    return hAddFee(req);
    if (m == "POST" && p == "/api/fees/pay")                return hPayFee(req);
    if (m == "POST" && p == "/api/fees/sort")               return hSortFees(req);

    if (m == "GET"  && p == "/api/leaves")                  return hLeavesList(req);
    if (m == "POST" && p == "/api/leaves")                  return hApplyLeave(req);
    if (m == "POST" && p == "/api/leaves/decide")           return hDecideLeave(req);

    if (m == "GET"  && p == "/api/complaints")              return hComplaintsList(req);
    if (m == "POST" && p == "/api/complaints")              return hSubmitComplaint(req);
    if (m == "POST" && p == "/api/complaints/resolve")      return hResolveComplaint(req, false);
    if (m == "POST" && p == "/api/complaints/resolve-next") return hResolveComplaint(req, true);

    return fail(404, "Unknown API endpoint.");
}

// ---------------------------------------------------------------
// HTTP plumbing
// ---------------------------------------------------------------
static const char *statusText(int code)
{
    switch (code)
    {
        case 200: return "OK";
        case 204: return "No Content";
        case 400: return "Bad Request";
        case 401: return "Unauthorized";
        case 403: return "Forbidden";
        case 404: return "Not Found";
        case 409: return "Conflict";
        case 413: return "Payload Too Large";
        default:  return "Internal Server Error";
    }
}

static void sendAll(sock_t c, const string &data)
{
    size_t sent = 0;
    while (sent < data.size())
    {
        int n = send(c, data.data() + sent, (int)(data.size() - sent), 0);
        if (n <= 0) return;
        sent += n;
    }
}

static void sendResponse(sock_t c, const Res &r)
{
    ostringstream h;
    h << "HTTP/1.1 " << r.status << " " << statusText(r.status) << "\r\n"
      << "Content-Type: " << r.type << "\r\n"
      << "Content-Length: " << r.body.size() << "\r\n"
      << "Cache-Control: no-store\r\n"
      << "Access-Control-Allow-Origin: *\r\n"
      << "Access-Control-Allow-Headers: Content-Type, X-Token\r\n"
      << "Access-Control-Allow-Methods: GET, POST, OPTIONS\r\n"
      << "Connection: close\r\n\r\n";
    sendAll(c, h.str() + r.body);
}

static string lower(string s)
{
    for (size_t i = 0; i < s.size(); i++)
        if (s[i] >= 'A' && s[i] <= 'Z') s[i] = (char)(s[i] - 'A' + 'a');
    return s;
}

// returns 0 = ok, 1 = connection problem (ignore), 2 = too large
static int readRequest(sock_t c, Req &req)
{
    string buf;
    char tmp[4096];
    size_t hdrEnd;

    while ((hdrEnd = buf.find("\r\n\r\n")) == string::npos)
    {
        int n = recv(c, tmp, sizeof(tmp), 0);
        if (n <= 0) return 1;
        buf.append(tmp, n);
        if (buf.size() > 65536) return 2;
    }

    string head = buf.substr(0, hdrEnd);
    string body = buf.substr(hdrEnd + 4);

    size_t lineEnd = head.find("\r\n");
    string first = head.substr(0, lineEnd);
    istringstream fl(first);
    string target, version;
    fl >> req.method >> target >> version;
    if (req.method.empty() || target.empty()) return 1;

    size_t qm = target.find('?');
    req.path = qm == string::npos ? target : target.substr(0, qm);

    size_t contentLength = 0;
    size_t pos = lineEnd == string::npos ? head.size() : lineEnd + 2;
    while (pos < head.size())
    {
        size_t e = head.find("\r\n", pos);
        if (e == string::npos) e = head.size();
        string line = head.substr(pos, e - pos);
        pos = e + 2;
        size_t colon = line.find(':');
        if (colon == string::npos) continue;
        string name = lower(trim(line.substr(0, colon)));
        string value = trim(line.substr(colon + 1));
        if (name == "content-length") contentLength = (size_t)atol(value.c_str());
        else if (name == "x-token") req.token = value;
    }

    if (contentLength > (1u << 20)) return 2;
    while (body.size() < contentLength)
    {
        int n = recv(c, tmp, sizeof(tmp), 0);
        if (n <= 0) return 1;
        body.append(tmp, n);
    }
    req.rawBody = body.substr(0, contentLength);
    return 0;
}

static void handleClient(sock_t c)
{
#ifdef _WIN32
    DWORD tv = 5000;
    setsockopt(c, SOL_SOCKET, SO_RCVTIMEO, (const char *)&tv, sizeof(tv));
#else
    struct timeval tv; tv.tv_sec = 5; tv.tv_usec = 0;
    setsockopt(c, SOL_SOCKET, SO_RCVTIMEO, (const char *)&tv, sizeof(tv));
#endif

    Req req;
    int rc = readRequest(c, req);
    if (rc == 1) { CLOSESOCK(c); return; }
    if (rc == 2) { sendResponse(c, fail(413, "Request too large.")); CLOSESOCK(c); return; }

    Res res;
    if (req.method == "OPTIONS")
    {
        res.status = 204;
    }
    else
    {
        bool bodyOk = true;
        if (req.method == "POST" && !trim(req.rawBody).empty())
            bodyOk = parseJsonObject(req.rawBody, req.body);

        if (!bodyOk)
        {
            res = fail(400, "Invalid JSON body.");
        }
        else
        {
            lockOn();
            res = route(req);
            lockOff();
        }
    }

    sendResponse(c, res);
    CLOSESOCK(c);
}

#ifdef _WIN32
static DWORD WINAPI clientThread(LPVOID arg)
{
    handleClient((sock_t)(size_t)arg);
    return 0;
}
static void startThread(sock_t c)
{
    HANDLE h = CreateThread(NULL, 0, clientThread, (LPVOID)(size_t)c, 0, NULL);
    if (h) CloseHandle(h); else CLOSESOCK(c);
}
#else
static void *clientThread(void *arg)
{
    handleClient((sock_t)(size_t)arg);
    return NULL;
}
static void startThread(sock_t c)
{
    pthread_t t;
    if (pthread_create(&t, NULL, clientThread, (void *)(size_t)c) == 0) pthread_detach(t);
    else CLOSESOCK(c);
}
#endif

int main(int argc, char **argv)
{
    int port = 8080;
    if (getenv("HM_PORT")) port = atoi(getenv("HM_PORT"));
    if (argc > 1) port = atoi(argv[1]);
    if (port <= 0 || port > 65535) port = 8080;

    // by default only this computer can connect. set HM_HOST=0.0.0.0 to allow the LAN
    const char *host = getenv("HM_HOST") ? getenv("HM_HOST") : "127.0.0.1";

#ifdef _WIN32
    WSADATA wsa;
    if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
    {
        cout << "Could not start Winsock." << endl;
        return 1;
    }
#endif
    lockInit();

    dbExecute("SELECT 1");   // opens hostelmate.db and creates the tables if needed
    ensureMess();

    sock_t srv = socket(AF_INET, SOCK_STREAM, 0);
    if (srv == BAD_SOCK) { cout << "Could not create socket." << endl; return 1; }

    int yes = 1;
    setsockopt(srv, SOL_SOCKET, SO_REUSEADDR, (const char *)&yes, sizeof(yes));

    sockaddr_in addr;
    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_port = htons((unsigned short)port);
    addr.sin_addr.s_addr = inet_addr(host);

    if (bind(srv, (sockaddr *)&addr, sizeof(addr)) != 0 || listen(srv, 32) != 0)
    {
        cout << "Could not listen on " << host << ":" << port << " (is another copy already running?)" << endl;
        return 1;
    }

    cout << "HostelMate server is running." << endl;
    cout << "Open http://localhost:" << port << " in your browser. Press Ctrl+C to stop." << endl;

    while (true)
    {
        sock_t c = accept(srv, NULL, NULL);
        if (c == BAD_SOCK) continue;
        startThread(c);
    }
    return 0;
}
