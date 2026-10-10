#include"student.h"
void makepayement(Fee &obj){
        float amount,due,fee;
        cin>>amount;
        due=obj.getdue();
        obj.setdue(due);
        

   }