#include <iostream>

using namespace std;

enum type{
    /**    TYPES    **/
    type_bool     = 0b000000000001,
    type_char     = 0b000000000010,
    type_uchar    = 0b000000000100,
    type_short    = 0b000000001000,
    type_ushort   = 0b000000010000,
    type_int      = 0b000000100000,
    type_uint     = 0b000001000000,
    type_llong    = 0b000010000000,
    type_ullong   = 0b000100000000,
    type_float    = 0b001000000000,
    type_double   = 0b010000000000,
    type_ldouble  = 0b100000000000,

    /**    OTHER    **/
    type_signed   = 0b111010101011,
    type_unsigned = 0b000101010100,
    type_all      = 0b111111111111
};

bool allNumeric(string s){
    for(int i=0; i<s.size(); i++)
        if(s[i]<48 || s[i]>57)
        return false;
    return true;
}

bool minorEqual(string a, string b){
    if(a.size()<b.size()) return true;
    if(a.size()>b.size()) return false;
    if(a==b) return true;
    for(int i=0; i<a.size(); i++)
        if(a[i]<b[i]) return true;
        else if(a[i]>b[i]) return false;
    return true;
}

int typeOf(string s){
    if(s=="") return 0;
    int type = 0;
    bool sign = false;
    /** BOOL **/
    if((s.size()==1 && (s[0]=='0'||s[0]=='1'))||(s.size()==4 &&s.substr(0, 4)=="true")||(s.size()==5 &&s.substr(0, 5)=="false"))
        type+=type_bool;

    /** CHAR && UCHAR **/
    if(s.size()==1)
        type+=type_char+type_uchar;
    else{
        if(s[0]=='-')
            sign=true;
        if(s.size()<4+sign && allNumeric(s.substr(sign, s.size()))){
            if(sign){
                if(minorEqual(s.substr(1, s.size()), "128"))
                    type+=type_char;
            }else{
                if(minorEqual(s.substr(0, s.size()), "127"))
                    type+=type_char;
                if(minorEqual(s.substr(0, s.size()), "255"))
                    type+=type_uchar;
            }
        }
    }

    /** SHORT && USHORT **/
    if(s[0]=='-')
        sign=true;
    if(allNumeric(s.substr(sign, s.size()))){
        if(sign){
            if(minorEqual(s.substr(1, s.size()), "32768"))
                type+=type_short;
        }else{
            if(minorEqual(s.substr(0, s.size()), "32767"))
                type+=type_short;
            if(minorEqual(s.substr(0, s.size()), "65535"))
                type+=type_ushort;
        }
    }

    /** INT && UINT **/
    if(s[0]=='-')
        sign=true;
    if(allNumeric(s.substr(sign, s.size()))){
        if(sign){
            if(minorEqual(s.substr(1, s.size()), "2147483648"))
                type+=type_int;
        }else{
            if(minorEqual(s.substr(0, s.size()), "2147483647"))
                type+=type_int;
            if(minorEqual(s.substr(0, s.size()), "4294967295"))
                type+=type_uint;
        }
    }

    /** LONG && ULONG **/
    if(s[0]=='-')
        sign=true;
    if(allNumeric(s.substr(sign, s.size()))){
        if(sign){
            if(minorEqual(s.substr(1, s.size()), "9223372036854775808"))
                type+=type_llong;
        }else{
            if(minorEqual(s.substr(0, s.size()), "9223372036854775807"))
                type+=type_llong;
            if(minorEqual(s.substr(0, s.size()), "18446744073709551615"))
                type+=type_ullong;
        }
    }
    double b=atof(s.c_str());
    float a=(float)b;
    bool is=false;
    if(b==0.0 && (s=="0"||s=="0.0")){
        type+=type_float+type_double;
    }else if(b!=0.0){
        type+=type_double;
        if(a==b)
            type+=type_float;
    }
    return type;
}

int main(){
    string s = "-48965";
    if(typeOf(s)&type_int)
        cout << "Se puede transformar en un entero (signed)." << endl;
    cin.get();
    return 1;
}
