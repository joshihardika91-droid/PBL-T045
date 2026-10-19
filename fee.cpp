#include"student.h"
#include"fee.h"
#include"user.h"

void Feehandler::sortbyduefees(int count){
    int i=0,j=0,min;
    Fee temp;
    if (count < 0 || count > recordcount)
    {
        cout << "Invalid record count.\n";
        return;
    }

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
int Feehandler::findrecords(){
    int id,index,ridx;
    cout<<"Enter student ID";
    cin>>id;
    index=id%recordcount;
    while(hashtable[index]!=-1){
        ridx=hashtable[index];
        if(feerecords[ridx].getStudentID()==id){
            return ridx;
        
        }
        index=(index+1)%recordcount;

    }
    
    cout<<"No Record found"<<endl;
    return-1;
}

void Feehandler::addFeeRecord( Fee& fee)
{
    if (recordcount >= MAX)
    {
        cout << "Fee record limit reached.\n";
        return;
    }

    if (fee.getStudentID() <= 0 || fee.getdue() < 0 ||
        fee.getfeeamount() < 0)
    {
        cout << "Invalid fee record.\n";
        return;
    }

    for (int i = 0; i < recordcount; i++)
    {
        if (feerecords[i].getStudentID() == fee.getStudentID())
        {
            cout << "A fee record already exists for this student.\n";
            return;
        }
    }

    feerecords[recordcount] = fee;
    recordcount++;


    createhashfunction(recordcount);
    cout << "Fee record added successfully.\n";
}
void Feehandler::displayfee() 
{
    if (recordcount == 0)
    {
        cout << "No fee records available.\n";
        return;
    }

    for (int i = 0; i < recordcount; i++)
    {
        cout << "\nStudent ID: " << feerecords[i].getStudentID();
        cout   << "\nTotal fee: " << feerecords[i].getfeeamount();
        cout  << "\nDue: " << feerecords[i].getdue();
        cout << "\nStatus: " << feerecords[i].getstatus();
        cout << "\nDate: " << feerecords[i].getdate() << '\n';
    }
}