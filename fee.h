#include"student.h"
#include"user.h"
#define MAX 400
class Feehandler{
    public:Fee *feerecords;
            int *hashtable;
           void sortbyduefees(int);
           void findrecords();
           void displayfee(); 
           void createhashfunction(int);
};
