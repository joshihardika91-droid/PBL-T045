#ifndef MESS_H
#define MESS_H

#include <iostream>
#include <string>
using namespace std;

class Mess
{
private:
    string breakfast;
    string lunch;
    string dinner;

    string breakfastTime;
    string lunchTime;
    string dinnerTime;

public:
    Mess();

    void showMenu();
    void showTimings();

    void changeMenu();
    void changeTimings();

    void adminMenu();
};
#endif