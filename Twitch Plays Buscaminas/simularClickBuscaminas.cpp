#include "ivancea.h"

using namespace std;

const string nick="ivancea96";
const string pass="";
const string channel="ivancea96";
const int X=9, Y=9;

void connectIRC(irc& ir);
void getWindow(HWND& window);
void leftClick(HWND& window, int x, int y);
inline void clickOnCell(HWND& window, int x, int y){
    leftClick(window, 47+18*(x-1), 90+18*(y-1));
}

int main(){
    irc ir;
    HWND window;
    string s;
    connectIRC(ir);
    getWindow(window);
    thSleep(20);
   // clickOnCell(window, 1,1);

    cout << "Conectado." << endl;
    while(true){
        s=ir.getClient()->recvChars();
        vector<string> v = split(s, ' ', 2);
        if(v.size()==2 && contains(v[1], "PRIVMSG", 0)){
            v = split(v[1], ':');
            if(contains(v[1],"\r\n",v[1].size()-2))
                v[1].erase(v[1].size()-2,2);
            else if(v[1][v[1].size()-1]=='\r' || v[1][v[1].size()-1]=='\n')
                v[1].erase(v[1].size()-1,1);
            string coords = trim(v[1]);
            if(coords.size()<=5 && coords.size()>=3){
                v = split(coords, ',');
                if(v.size()==2){
                    int x=parseString(v[0]), y=parseString(v[1]);
                    cout << x << ',' << y << endl;
                    if(!(x<1||x>X || y<1||y>Y))
                        clickOnCell(window,x,y);
                }
            }
        }

        thSleep(20);
    }
}


void connectIRC(irc& ir){
    while(!ir.start("", pass, nick, "irc.twitch.tv", 6667)) thSleep(100);
    ir.connect(channel);
}

void getWindow(HWND& window){
    window = FindWindow(NULL, "Buscaminas");
    if(window==0){
        cout << "Error finding Window";
        system("pause>nul");
    }else{
        SetForegroundWindow(window);
    }
}

BOOL CALLBACK EnumChildProc(HWND hwnd,LPARAM lParam){
    if(!IsWindow(hwnd)) return true;
    *(bool*)lParam=true;
    int bufsize = GetWindowTextLength(hwnd) + 1;
    LPWSTR title = new WCHAR[bufsize];
    GetWindowTextW(hwnd, title, bufsize);
    wcout << title;
    delete[] title;
}

void leftClick(HWND& window, int x, int y){
    if(IsWindow(window)){
        /*{
            bool a = false;
            EnumChildWindows(window, EnumChildProc, (LPARAM)&a);
            if(a){
                x=300;
                y=280;
            }
        }*/
        SetForegroundWindow(window);
        thSleep(20);
        RECT rect;
        GetWindowRect(window, &rect);

        double nx = (x + rect.left) * (65535 / (GetSystemMetrics(SM_CXSCREEN) - 1));
        double ny = (y + rect.top) * (65535 / (GetSystemMetrics(SM_CYSCREEN) - 1));

        INPUT input = {0};
        input.type=INPUT_MOUSE;
        input.mi.dx = nx;
        input.mi.dy = ny;
        input.mi.dwFlags = MOUSEEVENTF_MOVE | MOUSEEVENTF_ABSOLUTE | MOUSEEVENTF_LEFTDOWN | MOUSEEVENTF_LEFTUP;
        SendInput(1,&input, sizeof(INPUT));
    }
}
