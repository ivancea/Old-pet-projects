#include <iostream>
#include <vector>
#include <conio.h>
using namespace std;

class options{
    vector<string> _options;
    string _title;

    void coutTitle()const{
        if(_title.size()==0) return;
        vector<string> v;
        string t = _title;
        while(t.size()>80){
            v.push_back(t.substr(0,80));
            t.erase(0,80);
        }
        v.push_back(t);
        while(v.size()>1){
            cout << v[0];
            v.erase(v.begin());
        }
        cout << string((80-v[0].size())/2,' ') << v[0] << endl << endl;
    }

public:
    options(){}
    options(string title):_title(title){}
    options(const options& o):_options(o.getOptions()),_title(o.getTitle()){}
    void setTitle(string title){
        _title = title;
    }
    string getTitle()const{
        return _title;
    }
    vector<string> getOptions()const{
        return _options;
    }
    string getOption(size_t i)const{
        if(i<_options.size()) return _options[i];
        return "";
    }
    void addOption(string option){
        _options.push_back(option);
    }
    void addOptions(const vector<string>& v){
        for(int i=0; i<v.size(); i++)
            _options.push_back(v[i]);
    }
    void setOptions(const vector<string>& v){
        _options = v;
    }
    int show()const{
        int index = 0;
        char c = 0;
        while(c!='\r' && c!='\n'){
            system("cls");
            coutTitle();
            for(int i=0; i<_options.size(); i++){
                cout << '[' << (i==index?'X':' ') << "] " << _options[i] << endl;
            }
            c = getch();
            if((c=='w' || c=='W') && index>0)
                --index;
            if((c=='s' || c=='S') && index<_options.size()-1)
                ++index;
        }
        return index;
    }
};

int main(){
    options op;
    op.setTitle("Que querrias ser en otra vida?");
    op.addOption("Un Dios.");
    op.addOption("Un gobernador.");
    op.addOption("Un pensador.");
    op.addOption("Un trabajador.");
    op.addOption("Un esclavo.");
    cout << endl << endl << op.show();
    system("pause");
}

/*
char asd='w';
int h=1;

void pantalla (char a)
{
char b;

 if (a=='w')
   {
   a='X';
   b=' ';
   h=1;
 }
 if (a=='s')
   {
   a=' ';
   b='X';
   h=2;
   }
 if (a!='w' || a!='s')
 {
 if (h==1)
 {
 a='X';
 b=' ';
 }else{
 a=' ';
 b='X';
 }
 }


cout << endl << "Elige una opcion con w, a, s, d, y pulsa ENTER" << endl;
cout << "     ["<< a << "] Salir" << endl;
cout << "     ["<< b << "] Ver procesos en ejecución"<< endl<<endl;
}


int main ()
{
do{
pantalla (asd);
asd=getch();
system("cls");
}while (asd!='\r');
if (h==1)
{
return 0;
}else{
system("cls");
system("tasklist");
system("pause");
return 0;
}
}*/
