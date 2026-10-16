#include"student.h"
#define MAX 400
class Feehandler{
    public:Fee *feerecords;
            int hashtable[recordcount];
           void sortbyduefees(int);
           void findrecords();
           void displayfee(); 
           void createhashfunction(int);
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
void Feehandler::createhashfunction(int recordcount){
    int i=0,s_id,index;
    for(i=0;i<recordcount;i++){
        hashtable[i]=-1;
    }
    for (i=0;i<recordcount;i++){
         s_id=feerecords[i].getStudentID();
         index=s_id%recordcount;
         while(hashtable[index]!=-1){
            index=(index+1)%recordcount;
         }
    hashtable[index]=i;
    }

}
void Feehandler::findrecords(){
    int id,index,ridx;
    cout<<"Enter student ID";
    cin>>id;
    index=id%recordcount;
    while(hashtable[index]!=-1){
        ridx=hashtable[index];
        if(feerecords[ridx].getStudentID()==id){
        
        }
        index=(index+1)%recordcount;

    }
    
    cout<<"No Record found"<<endl;
}