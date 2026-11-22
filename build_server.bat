@echo off
if not exist database\sqlite3.o (
  echo Building sqlite3.o...
  gcc -c database\sqlite3.c -o database\sqlite3.o
)
g++ -std=c++11 -Idatabase server.cpp database\db.cpp database\sqlite3.o -o HostelMateServer.exe -lws2_32
if errorlevel 1 (
  echo.
  echo Build failed.
) else (
  echo Built HostelMateServer.exe
)
