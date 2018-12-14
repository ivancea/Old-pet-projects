#include <iostream>
#include <vector>
#include <map>

#include "serial.h"

#include "strings.h"

using namespace std;


/// OPTIONS
const uint32_t maxDigitalPin = 13;
const uint32_t maxPin = 19;
const string port = "\\\\.\\COM3";

Serial se;

enum id{
    PINMODE=0,

    DIGITALWRITE,
    ANALOGWRITE,

    DIGITALREAD,
    ANALOGREAD,

    TONE,
    NOTONE
};

enum pinModes{
    PM_OUTPUT=0,
    PM_INPUT=1
};

enum digitalModes{
    DM_LOW=0,
    DM_HIGH=1
};

const string message = "-Arduino Interpret-";

map<string,string> help = {
    {"clear",       "Usage: clear   (Clear the screen"},
    {"help",        "Usage: help <command>"},
    {"pinmode",     "Usage: pinmode <pinnumber> <INPUT|OUTPUT>"},
    {"digitalwrite","Usage: digitalwrite <pinnumber> <HIGH|LOW>"},
    {"analogwrite", "Usage: analogwrite <pinnumber> <value>"},
    {"digitalread", "Usage: digitalread <pinnumber>"},
    {"analogread",  "Usage: analogread <pinnumber>"},
    {"tone",        "Usage: tone <pinnumber> <frequency> [duration]"},
    {"notone",      "Usage: notone <pinnumber>"}
};

void interpret(string cmd, vector<string>& args){

    if(equalIgnoreCase(cmd,"help")){
        if(args.size()==0){
            cout << "Commands:" << endl;
            for(auto it:help)
                cout << "    " << it.first << endl;
        }else
        if(args.size()==1){
            if(help.find(args[0])!=help.end()){
                cout << help[args[0]] << endl;
            }else
                cout << "That command doesn't exists" << endl;
        }else
            cout << help["help"] << endl;
    }else

    if(equalIgnoreCase(cmd,"clear")){
        if(args.size()!=0){
            cout << help["clear"] << endl;
            return;
        }
        system("cls");
        cout << message << endl;
    }else

    if(equalIgnoreCase(cmd,"pinmode")){
        if(args.size()!=2 || (!equalIgnoreCase(args[1],"input") && !equalIgnoreCase(args[1],"output"))){
            cout << help["pinmode"] << endl;
            return;
        }
        int pinNumber = atoi(args[0].c_str());

        if(args[0].size()==2 && args[0][0]=='A' && args[0][1]>='0' && args[0][1]<='9')
            pinNumber = maxDigitalPin+1 + args[0][1]-'0';

        if(pinNumber<0 || pinNumber>maxPin){
            cout << "Error: Range: 19 >= pinnumber >= 0 OR {A0,A1,A2,...}" << endl;
            return;
        }

        char data[] = {(char)PINMODE, (char)pinNumber, (char)(equalIgnoreCase(args[1],"input")?PM_INPUT:PM_OUTPUT)};

        se.writeData(data,sizeof(data));
    }else

    if(equalIgnoreCase(cmd,"digitalwrite")){
        if(args.size()!=2 || (equalIgnoreCase(args[1],"high") && equalIgnoreCase(args[1],"low"))){
            cout << help["digitalwrite"] << endl;
            return;
        }
        int pinNumber = atoi(args[0].c_str());

        if(args[0].size()==2 && args[0][0]=='A' && args[0][1]>='0' && args[0][1]<='9')
            pinNumber = maxDigitalPin+1 + args[0][1]-'0';

        if(pinNumber<0 || pinNumber>maxPin){
            cout << "Error: Range: " << maxPin << " >= pinnumber >= 0 OR {A0,A1,A2,...}" << endl;
            return;
        }

        char data[] = {(char)DIGITALWRITE, (char)pinNumber, (char)(equalIgnoreCase(args[1],"high")?DM_HIGH:DM_LOW)};

        se.writeData(data,sizeof(data));
    }else

    if(equalIgnoreCase(cmd,"analogwrite")){
        if(args.size()!=2 || !isDec(args[1])){
            cout << help["analogwrite"] << endl;
            return;
        }
        int pinNumber = atoi(args[0].c_str());

        if(args[0].size()==2 && args[0][0]=='A' && args[0][1]>='0' && args[0][1]<='9')
            pinNumber = maxDigitalPin+1 + args[0][1]-'0';

        if(pinNumber<0 || pinNumber>maxPin){
            cout << "Error: Range: " << maxPin << " >= pinnumber >= 0 OR {A0,A1,A2,...}" << endl;
            return;
        }

        int write = atoi(args[1].c_str());

        if(write<0 || write>255){
            cout << "Error: Range: 255 >= value >= 0" << endl;
            return;
        }

        char data[] = {(char)ANALOGWRITE, (char)pinNumber, (char)write};

        se.writeData(data,sizeof(data));
    }else

    if(equalIgnoreCase(cmd,"digitalread")){
        if(args.size()!=1){
            cout << help["digitalread"] << endl;
            return;
        }
        int pinNumber = atoi(args[0].c_str());

        if(args[0].size()==2 && args[0][0]=='A' && args[0][1]>='0' && args[0][1]<='9')
            pinNumber = maxDigitalPin+1 + args[0][1]-'0';

        if(pinNumber<0 || pinNumber>maxPin){
            cout << "Error: Range: " << maxPin << " >= pinnumber >= 0 OR {A0,A1,A2,...}" << endl;
            return;
        }

        char data[] = {(char)DIGITALREAD, (char)pinNumber};

        se.writeData(data,sizeof(data));
        string response = se.readDataLength(1);
        if(response.size()!=1){
            cout << "Error reading data from Arduino..." << endl;
            return;
        }
        cout << (response[0]==DM_LOW?"LOW":"HIGH") << endl;
    }else

    if(equalIgnoreCase(cmd,"analogread")){
        if(args.size()!=1){
            cout << help["analogread"] << endl;
            return;
        }
        int pinNumber = atoi(args[0].c_str());

        if(args[0].size()==2 && args[0][0]=='A' && args[0][1]>='0' && args[0][1]<='9')
            pinNumber = maxDigitalPin+1 + args[0][1]-'0';

        if(pinNumber<0 || pinNumber>maxPin){
            cout << "Error: Range: " << maxPin << " >= pinnumber >= 0 OR {A0,A1,A2,...}" << endl;
            return;
        }

        char data[] = {(char)ANALOGREAD, (char)pinNumber};

        se.writeData(data,sizeof(data));
        string response = se.readDataLength(2);
        if(response.size()!=2){
            cout << "Error reading data from Arduino..." << endl;
            return;
        }
        uint16_t value = *(uint16_t*)response.c_str();
        cout << value << " / 1024. " << endl;
        cout << ((double)value*100)/1024.0 << " %" << endl;
    }else

    if(equalIgnoreCase(cmd,"tone")){
        if(args.size()<2 || args.size()>3 || !isDec(args[1]) || args[1][0]=='-'){
            cout << help["tone"] << endl;
            return;
        }
        int pinNumber = atoi(args[0].c_str());

        if(args[0].size()==2 && args[0][0]=='A' && args[0][1]>='0' && args[0][1]<='9')
            pinNumber = maxDigitalPin+1 + args[0][1]-'0';

        if(pinNumber<0 || pinNumber>maxPin){
            cout << "Error: Range: " << maxPin << " >= pinnumber >= 0 OR {A0,A1,A2,...}" << endl;
            return;
        }

        uint32_t freq = atoi(args[1].c_str());
        uint32_t time = 0;

        if(freq<31){
            cout << "Error: Range: frequency >= 31" << endl;
            return;
        }

        if(args.size()==3){
            if(!isDec(args[2]) || args[2][0]=='-'){
                cout << "Error: Range: duration > 0" << endl;
                return;
            }
            time = atoi(args[2].c_str());
            if(time<=0) return;
        }

        char data[] = {(char)TONE, (char)pinNumber};

        se.writeData(data,sizeof(data));
        se.writeData((char*)&freq,sizeof(freq));
        se.writeData((char*)&time,sizeof(time));
    }else

    if(equalIgnoreCase(cmd,"notone")){
        if(args.size()!=1){
            cout << help["notone"] << endl;
            return;
        }
        int pinNumber = atoi(args[0].c_str());

        if(args[0].size()==2 && args[0][0]=='A' && args[0][1]>='0' && args[0][1]<='9')
            pinNumber = maxDigitalPin+1 + args[0][1]-'0';

        if(pinNumber<0 || pinNumber>maxPin){
            cout << "Error: Range: " << maxPin << " >= pinnumber >= 0 OR {A0,A1,A2,...}" << endl;
            return;
        }

        char data[] = {(char)NOTONE, (char)pinNumber};

        se.writeData(data,sizeof(data));
    }else
        cout << "Unkonwn command. Type \"help\" for commands." << endl;

}

int main(){
    string t;
    cout << message << endl;
    while(!se.start(port)){
        cout << "Press a key for retry...";
        system("pause>nul");
        cout << endl;
    }
    cout << "Arduino on port " << port << endl;
    while(true){
        cout << ">> ";
        getline(cin,t);

        if(t.size()==0) continue;
        if(t=="exit") break;

        vector<string> v = split(t);

        if(v.size()==0) continue;

        string cmd = v[0];
        v.erase(v.begin());
        interpret(cmd, v);

    }
}
