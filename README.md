# Hostel-Mate

## 1. Project Description

Hostel-Mate is a web-based hostel management system designed to simplify and automate hostel-related activities. It provides a centralized platform for managing student records, attendance, room allocation, fee records, leave applications, complaints, and mess information.

The system aims to reduce manual paperwork, improve efficiency, and make hostel administration more organized.

## 2. Problem Statement and Objectives

### Problem Statement
Traditional hostel management involves several manual processes, including maintaining student records, managing attendance, allocating rooms, handling fee payments, processing leave applications, and resolving complaints. These processes are time-consuming, difficult to maintain, and prone to human errors.

Hostel-Mate addresses these challenges by providing a digital platform that streamlines hostel operations and improves communication between students and administrators.

### Objectives
- To automate routine hostel management activities.
- To maintain student and hostel records efficiently.
- To simplify attendance and fee management.
- To provide an organized room allocation system.
- To manage leave applications and complaints.
- To improve accessibility of hostel-related information.

## 3. Team Members



Hardika Joshi - Team Leader 
 Anushka Sanewal - Code Contributor / Database
 Anshika Srinet - Code Contributor / Frontend
 Pratiksha Raj - Code Contributor 

## 4. Technologies Stack

| Technology / Tool | Purpose |

| C++ | Backend development and application logic |
| C++ STL | Data structures and algorithms |
| HTML | Frontend structure |
| CSS | Frontend styling |
| JavaScript | Frontend functionality and API communication |
| Custom C++ HTTP API | Frontend-backend communication |
| SQLite | Database management |
| GCC / G++ | Compilation |
| Winsock | Network communication on Windows |
| Visual Studio Code | Development environment |
| Git and GitHub | Version control and collaboration |

## 5. Setup and Installation Instructions

### Prerequisites

Install the following software before running the project:

- Visual Studio Code
- MinGW-w64 (GCC/G++ compiler)
- Git
- Windows Operating System

Ensure that GCC and G++ are added to the system PATH.

Verify the installation:

```bash
gcc --version
g++ --version
git --version
```

### Step 1: Clone the Repository

```bash
git clone <https://github.com/joshihardika91-droid/PBL-T045>
```

Navigate to the project directory:

```bash
cd PBL-T045
```

### Step 2: Prepare Frontend and Database

Copy the frontend file to the project root:

```cmd
copy frontend\hostelmate_app.html hostelmate_app.html
```

Copy the existing SQLite database:

```cmd
copy database\hostelmate.db hostelmate.db
```

### Step 3: Compile SQLite

```bash
gcc -c database/sqlite3.c -o database/sqlite3.o
```

### Step 4: Build the Application

Compile the C++ server and link it with SQLite and the Windows socket library:

```bash
g++ -std=c++11 -Idatabase server.cpp database/db.cpp database/sqlite3.o -o HostelMateServer.exe -lws2_32
```

### Step 5: Run the Application

Execute the generated server:

```cmd
HostelMateServer.exe
```

### Step 6: Access the Application

Open your browser and visit:

```text
http://localhost:8080
```

The Hostel-Mate application will be accessible through the browser.

To stop the server, press `Ctrl + C` in the terminal.

**Note:** The frontend HTML file and database must be available in the project root directory when running the server. If starting with a fresh database, skip copying the existing database.

## 6. Major Features and Modules

### 1. User Authentication Module
- Student registration and login.
- Admin login.
- Admin approval and rejection of student registrations.
- Session-based authentication.

### 2. Student Management Module
- Student profile management.
- Student information retrieval.
- Password management.

### 3. Attendance Management Module
- Mark and manage student attendance.
- View attendance records.
- Undo attendance entries.

### 4. Fee Management Module
- Maintain student fee records.
- Track fee payments.
- Manage outstanding fees.

### 5. Leave Management Module
- Submit leave applications.
- View leave requests.
- Approve or reject leave applications.

### 6. Complaint Management Module
- Submit student complaints.
- View complaints.
- Resolve pending complaints.

### 7. Room Allocation Module
- Manage hostel room records.
- Submit room allocation requests.
- Process room requests.

### 8. Mess Management Module
- View and manage mess-related information.
- Maintain mess menu and related details.

## 7. Current Project Status / Progress

**Project Status: Core Implementation and Integration**

The major modules of Hostel-Mate have been implemented and integrated into the application.

- C++ backend development completed for the implemented modules.
- Web-based frontend developed using HTML, CSS, and JavaScript.
- Custom C++ HTTP API implemented for frontend-backend communication.
- SQLite database integration implemented for persistent data storage.
- Student and administrator functionalities implemented.
- Application setup and local execution supported through the provided server build process.

Further testing, debugging, and improvements can be performed to enhance application reliability and user experience.

