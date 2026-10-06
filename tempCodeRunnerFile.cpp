#include<iostream>
using namespace std;

int main()
{
    string str, vowels = "", consonants = "";
    cout << "Enter string" << endl;
    cin >> str;

    for(int i = 0; i < str.length(); i++)
    {
        char ch = str[i];
        if(ch == 'a' || ch == 'e' || ch == 'i' || ch == 'o' || ch == 'u' || ch == 'A' || ch == 'E' || ch == 'I' || ch == 'O' || ch == 'U')
        {
            vowels = vowels + ch;
        }
        else
        {
            consonants = consonants + ch;
        }
    }

    cout << "vowels: " << vowels << endl;
    cout << "consonants: " << consonants << endl;
    return 0;
}