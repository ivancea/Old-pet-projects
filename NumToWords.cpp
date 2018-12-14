#include <iostream>
#include <cstring>

using namespace std;

string toWords(uint64_t n){
    static const string num[] = {"zero", "one", "two", "three", "four", "five",
                                 "six", "seven", "eight", "nine", "ten", "eleven",
                                 "twelve", "thirteen", "fourteen", "fifteen", "sixteen",
                                 "seventeen", "eighteen", "nineteen", "twenty"};
    static const string ten[] = {"zero", "teen", "twenty", "thirty", "forty",
                                 "fifty", "sixty", "seventy", "eighty", "ninety"};
    string t;
    if(n==0)
        return num[0];
    if(n/1000000000000>0){
        t += toWords(n/1000000000000) + " trillion";
        n%=1000000000000;
        if(n>0){
            if(n>=100)
                t += ", ";
            else
                t += " and ";
        }
    }
    if(n/1000000000>0){
        t += toWords(n/1000000000) + " billion";
        n%=1000000000;
        if(n>0){
            if(n>=100)
                t += ", ";
            else
                t += " and ";
        }
    }
    if(n/1000000>0){
        t += toWords(n/1000000) + " million";
        n%=1000000;
        if(n>0){
            if(n>=100)
                t += ", ";
            else
                t += " and ";
        }
    }
    if(n/1000>0){
        t += toWords(n/1000) + " thousand";
        n%=1000;
        if(n>0){
            if(n>=100)
                t += ", ";
            else
                t += " and ";
        }
    }
    if(n/100>0){
        t += toWords(n/100) + " hundred";
        n%=100;
        if(n>0)
            t += " and ";
    }
    if(n<=20 && n>0){
        return t + num[n];
    }
    if(n/10>0){
        t += ten[n/10];
        n%=10;
        if(n>0)
            t += '-';
    }
    if(n>0){
        t += num[n];
    }
    return t;
}

int main(){
    while(true){
        string t;
        getline(cin,t);
        cout << toWords(stoull(t)) << endl;
    }
}
