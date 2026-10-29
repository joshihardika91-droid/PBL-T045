#include "mess.h"

Mess::Mess()
{
    days[0] = "Monday";
    days[1] = "Tuesday";
    days[2] = "Wednesday";
    days[3] = "Thursday";
    days[4] = "Friday";
    days[5] = "Saturday";
    days[6] = "Sunday";

    // Monday
    menu[0][0] = "Paratha, Curd";
    menu[0][1] = "Rice, Dal, Roti, Sabzi";
    menu[0][2] = "Samosa, Tea";
    menu[0][3] = "Rice, Dal, Roti, Paneer";

    // Tuesday
    menu[1][0] = "Poha, Milk";
    menu[1][1] = "Rice, Rajma, Roti, Salad";
    menu[1][2] = "Biscuits, Tea";
    menu[1][3] = "Roti, Dal, Mix Veg";

    // Wednesday
    menu[2][0] = "Aloo Paratha, Curd";
    menu[2][1] = "Rice, Dal, Chole, Roti";
    menu[2][2] = "Pakora, Tea";
    menu[2][3] = "Rice, Dal, Roti, Sabzi";

    // Thursday
    menu[3][0] = "Bread, Butter, Milk";
    menu[3][1] = "Rice, Dal, Roti, Paneer";
    menu[3][2] = "Sandwich, Tea";
    menu[3][3] = "Roti, Dal, Aloo Sabzi";

    // Friday
    menu[4][0] = "Poha, Tea";
    menu[4][1] = "Rice, Dal, Roti, Chole";
    menu[4][2] = "Samosa, Tea";
    menu[4][3] = "Rice, Dal, Roti, Mix Veg";

    // Saturday
    menu[5][0] = "Paratha, Curd";
    menu[5][1] = "Rice, Dal, Roti, Sabzi";
    menu[5][2] = "Bread Pakora, Tea";
    menu[5][3] = "Pulao, Raita";

    // Sunday
    menu[6][0] = "Puri, Sabzi";
    menu[6][1] = "Rice, Dal, Paneer, Roti";
    menu[6][2] = "Cake, Tea";
    menu[6][3] = "Special Dinner";

    // Timings for all days
    for(int i = 0; i < 7; i++)
    {
        timings[i][0] = "8:00 AM - 9:00 AM";    // Breakfast
        timings[i][1] = "1:00 PM - 2:00 PM";    // Lunch
        timings[i][2] = "5:00 PM - 6:00 PM";    // Snacks
        timings[i][3] = "8:00 PM - 9:00 PM";    // Dinner
    }
}

void Mess::showMenu()
{
    cout << "\n========== WEEKLY MESS MENU ==========\n";

    for(int i = 0; i < 7; i++)
    {
        cout << "\n" << days[i] << ":\n";

        cout << "Breakfast : " << menu[i][0] << endl;
        cout << "Lunch     : " << menu[i][1] << endl;
        cout << "Snacks    : " << menu[i][2] << endl;
        cout << "Dinner    : " << menu[i][3] << endl;
    }
}

void Mess::showTimings()
{
    cout << "\n========== MESS TIMINGS ==========\n";

    for(int i = 0; i < 7; i++)
    {
        cout << "\n" << days[i] << ":\n";

        cout << "Breakfast : " << timings[i][0] << endl;
        cout << "Lunch     : " << timings[i][1] << endl;
        cout << "Snacks    : " << timings[i][2] << endl;
        cout << "Dinner    : " << timings[i][3] << endl;
    }
}
