#ifndef MESS_H
#define MESS_H

#include <iostream>
#include <string>
using namespace std;

class Mess
{
private:
    string days[7];

    string menu[7][4];

    string timings[7][4];
    

public:
    Mess();

    void showMenu();
    void showTimings();

    void changeMenu();
    void changeTimings();

    void adminMenu();
};
#endif