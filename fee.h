#include"student.h"
#include"user.h"
#define MAX 400
class Fee{
    private :int StudentID;
             float feeamount;
             string date;
             float due;
             string Status;
              double payments[100];
            int paycount; 
    public:Fee(){
            StudentID=0;
             feeamount=0;
             date=="";
             due=0;
             Status="Pending";
             paycount=0;
             }
           
            int getStudentID(){return StudentID;};
            void setdue(float amount ){due=amount;};
            void setstatus(string s){Status=s;};
            void setdate(string d){date=d;};
            void setstudentid(int ID){StudentID=ID;}
            void setfeeamount(float f){feeamount=f;}
            string getdate(){return date;}
            string getstatus(){return Status;}
            float getdue(){return due;}
            float getfeeamount(){return feeamount;}
            int getpaycount(){return paycount;};
            int addpayment(float amt){
               int  Paycount=getpaycount();
                if (Paycount>100){
                        return 0;
                }
                payments[Paycount]=amt;
                Paycount++;
                return 1;
           }
           float getpayment(int i){
                return payments[i];
           }
            

             
};
class Feehandler{
    private:Fee *feerecords;
            int *hashtable;
            int recordcount;
            
    public:Feehandler();
           ~Feehandler(){delete[] feerecords;
                          delete[] hashtable;};
            int getrecordcount(){return recordcount;}
           void sortbyduefees(int);
           int findrecords(int );
           void displayfee(); 
           void createhashfunction(int);
           void addFeeRecord();
           void Studentfeedashboard(int id);
           void AdminfeeDashboard();
           void Makepayment(int id);
           void viewpayments(int);
         
           
};
