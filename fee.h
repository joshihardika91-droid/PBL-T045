#include"student.h"
class Feehandler{
    public:Fee *feerecords;
           void sortbyduefees(int);
           void findrecords();
           void displayfee(); 
};
void Feehandler::sortbyduefees(int recordcount){
    int i=0,j=0,min;
    Fee temp;
    for(i=0;i<recordcount-1;i++){
        min=i;
        for(j=i+1;j<recordcount;j++){
            if(feerecords[j].getdue()<feerecords[min].getdue()){
                min=j;
            }
        }
        temp=feerecords[i];
        feerecords[i]=feerecords[min];
        feerecords[min]=temp;

    }
}