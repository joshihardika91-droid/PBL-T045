
#include"fee.h"
#include<iostream>
using namespace std;

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
        if(min!=i){
        temp=feerecords[i];
        feerecords[i]=feerecords[min];
        feerecords[min]=temp;}

    }
        createhashfunction(recordcount);

}
void Feehandler::createhashfunction(int recordcount){
    int i=0,s_id,index;
    for(i=0;i<recordcount;i++){
        hashtable[i]=-1;
    }
    for (i=0;i<recordcount;i++){
         s_id=feerecords[i].getStudentID();
         index=s_id%MAX;
         while(hashtable[index]!=-1){
            index=(index+1)%MAX;
         }
    hashtable[index]=i;
    }

}
int Feehandler::findrecords(int id){
    int index,ridx;
    if (recordcount == 0)
    {
        cout << "No fee records available.\n";
        return -1;
    }
    
    index=id%MAX;
    while(hashtable[index]!=-1){
        ridx=hashtable[index];
        if(feerecords[ridx].getStudentID()==id){
            return ridx;
        
        }
        index=(index+1)%MAX;

    }
    
    cout<<"No Record found"<<endl;
    return-1;
}

void Feehandler::addFeeRecord()

{
    if (recordcount >= MAX)
    {
        cout << "Fee record limit reached.\n";
        return;
    }
    Fee fee;
    int sid;
    float fmt;
    string dt;
    
    cout<<"Enter student id"<<endl;
    cin>>sid;
    fee.setstudentid(sid);
    cout<<"Enter Fee Amount "<<endl;
    cin>>fmt;
    fee.setfeeamount(fmt);
    fee.setdue(fmt);
    cout<<"Enter date"<<endl;
    getline(cin>>ws,dt);
    fee.setdate(dt);


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

void Feehandler::Makepayment(int id){
   int  idx=findrecords(id);
   if (idx==-1){
    cout<<"No records "<<endl;

   }
   float amount;

    cout << "Outstanding fee: Rs. "
         << feerecords[idx].getdue() << '\n';

    if (feerecords[idx].getdue() <= 0)
    {
        cout << "All fees have been paid.\n";
        return;
    }

    cout << "Enter payment amount: Rs. ";
    cin >> amount;

   

    if (amount <= 0 ||
        amount > feerecords[idx].getdue())
    {
        cout << "Invalid payment amount.\n";
        return;
    }

    if (!feerecords[idx].addpayment(amount))
    {
        cout << "Payment history is full.\n";
        return;
    }

    float remaining =
        feerecords[idx].getdue() - amount;

    feerecords[idx].setdue(remaining);

    if (remaining == 0)
        feerecords[idx].setstatus("Paid");
    else
        feerecords[idx].setstatus("Partial");

    cout << "Payment successful!\n";
    cout << "Remaining due: Rs. " << remaining << '\n';


}
void Feehandler::viewpayments(int id){
    int  idx=findrecords(id);
   if (idx==-1){
    cout<<"No records "<<endl;

   }
   int c=feerecords[idx].getpaycount();
   cout<<"\n-----------TRANSACTION HISTORY-----------\n";
   if (c == 0)
    {
        cout << "No payment history available.\n";
        return;
    }


    for (int i = 0; i < c; i++)
    {
        cout << "Payment " << i + 1 << ": Rs. "
             << feerecords[idx].getpayment(i) << '\n';
    }

    cout << "Total paid: Rs. "
         << feerecords[idx].getfeeamount()
            - feerecords[idx].getdue() << '\n';

    cout << "Due fees: Rs. "
         << feerecords[idx].getdue() << '\n';
}


void Feehandler::Studentfeedashboard(int id){
    int ch;
    do{
            cout<<"----Student Fee Portal-----"<<endl;
            cout<<"1.Make Payment"<<endl;
            cout<<"2.View previous payments"<<endl;
            cout<<"3.Return to Student menu"<<endl;
            cout<<"Enter Choice"<<endl;
            cin>>ch;
            switch(ch){
                case 1:
                       Makepayment( id);
                        break;
                case 2:viewpayments(id);
                        break;
                case 3:cout<<"Back to Main Menu";
                       return ;
                default:cout<<"Invalid Choice ";
                
            }
    }while(ch!=3);

}
void Feehandler::AdminfeeDashboard(){
        int ch;
    do{
            cout<<"----Student Fee Portal-----"<<endl;
            cout<<"1.Sort Record by dues"<<endl;
            cout<<"2.View all records"<<endl;
            cout<<"3.Add Record"<<endl;
            cout<<"4.Return to Admin menu"<<endl;
            cout<<"Enter Choice"<<endl;
            cin>>ch;
            switch(ch){
                case 1:
                        {int rcount=getrecordcount();
                        sortbyduefees(rcount);
                        break;}
                case 2:displayfee();
                        break;
                case 3:
                        addFeeRecord();
                         break;
                case 4:cout<<"Back to Main Menu";
                       return ;
                default:cout<<"Invalid Choice";
                
            }
    }while(ch!=4);
}