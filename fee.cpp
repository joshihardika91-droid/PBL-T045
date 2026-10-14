#include"student.h"
void makepayement(Fee &obj){
        float amount,due,fee;
        cin>>amount;
        due=obj.getdue();
        if(due==amount){
                due=0;
        }
        obj.setdue(due);
        
        

   }