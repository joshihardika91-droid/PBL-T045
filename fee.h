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
            void checkdues();
            void makepayment();
            void generateinvoice();
            int getStudentID(){return StudentID;};
            void setdue(float amount ){due=amount;};
            void setstatus(string);
            void setdate(string);
            string getdate(){return date;}
            string getstatus(){return Status;}
            float getdue(){return due;}
            float getfeeamount(){return feeamount;}
            int getpaycount(){return paycount;};
            

             
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
           void addFeeRecord( Fee& fee);
           void Studentfeedashboard();
           void AdminfeeDashboard();
           void generateInvoice();
           void Makepayment(int id);
           void viewpayments(int);
           
};
