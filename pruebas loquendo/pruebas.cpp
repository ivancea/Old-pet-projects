#include <iostream>
#include <ctime>
#include <chrono>
#include <thread>
#include <fstream>

#include "loqtts.h"

using namespace std;

string toWords(uint64_t n){__is_integer<float> a; a.__value = 4;
    static const string num[] = {"cero", "una" /* "uno" para masculino */, "dos", "tres", "cuatro", "cinco",
                                 "seis", "siete", "ocho", "nueve", "diez", "once",
                                 "doce", "trece", "catorce", "quince", "dieciseis",
                                 "diecisiete", "dieciocho", "diecinueve", "veinte"};
    static const string ten[] = {"cero", "dieci", "veinti", "treinta", "cuarenta",
                                 "cincuenta", "sesenta", "setenta", "ochenta", "noventa"};
    string t;
    if(n==0)
        return num[0];
    if(n<=20 && n>0){
        return t + num[n];
    }
    if(n>20 && n<30){
        return ten[n/10]+num[n%10];
    }
    if(n/10>0){
        t += ten[n/10];
        n%=10;
        if(n>0)
            t += " y ";
    }
    if(n>0){
        t += num[n];
    }
    return t;
}

int main(int argc, char *argv[]){
    ttsHandleType hReader;
    ttsResultType r;
    const char* p = "Jorge";

    /* Initializes a LoquendoTTS Reader using the implicit session */
    r = ttsNewReader(&hReader, NULL);
    if (r != tts_OK){
        cerr << ttsGetErrorMessage(r) << endl;
        ttsDeleteSession(NULL);
        return r;
    }
    r = ttsLoadPersona(hReader, p, NULL, NULL);
    if(r != tts_OK){
        cerr << ttsGetErrorMessage(r) << endl;
        ttsDeleteSession(NULL);
        return r;
    }

    r = ttsSetAudio(hReader, "LTTS7AudioBoard", NULL, 32000, tts_LINEAR, tts_STEREO, 0);

    if(r != tts_OK){
        cerr << ttsGetErrorMessage(r) << endl;
        ttsDeleteSession(NULL);
        return r;
    }
    int lastMinute=60;
    while(true){
        time_t tim;
        tm *tstr;
        time(&tim);
        tstr = localtime(&tim);

        if(tstr->tm_min==lastMinute || tstr->tm_min%5 != 0){
            this_thread::sleep_for(chrono::seconds(1));
            continue;
        }
        lastMinute = tstr->tm_min;

        cout << '<' << tstr->tm_hour << ':' << (tstr->tm_min/10>0?"":"0") << tstr->tm_min << '>' << endl;

        string toRead = "Son las " + toWords(tstr->tm_hour+(tstr->tm_min>30?1:0)-(tstr->tm_hour>12?12:0));
        if(tstr->tm_min == 0)
            toRead += " en punto";
        else if(tstr->tm_min == 30)
            toRead += " y media";
        else if(tstr->tm_min > 30)
            toRead += " menos " + toWords(60-tstr->tm_min);
        else
            toRead += " y " + toWords(tstr->tm_min);

        /*system("Oraciones.exe > temp.tmp");
        ifstream file("temp.tmp");
        string toRead;
        getline(file, toRead);
        file.close();
        for(int i=0; i<toRead.size(); i++)
            if(toRead[i]==','){
                toRead.replace(i,1,",,");
                ++i;
            }
        system("del /F /Q temp.tmp");*/

        r = ttsRead(
                hReader,	/* Reader handle */
                toRead.c_str(), /* Input */
                ttsFALSE, /* bAsync */
                ttsFALSE, /* bFromFile */
                NULL);	/* ttsRead keeps control until the end */
    }
    ttsDeleteSession(NULL);
}
