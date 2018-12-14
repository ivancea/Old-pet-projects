#include "Serial.h"

#include "timers.h"

Serial::Serial():connected(false){}

Serial::Serial(const std::string portName):connected(false){
    start(portName);
}

bool Serial::start(const std::string portName){
    stop();
    connected = false;

    hSerial = CreateFile(portName.c_str(),
            GENERIC_READ | GENERIC_WRITE,
            0,
            NULL,
            OPEN_EXISTING,
            FILE_ATTRIBUTE_NORMAL,
            NULL);

    if(hSerial==INVALID_HANDLE_VALUE){
        if(GetLastError()==ERROR_FILE_NOT_FOUND){
            lastError = "File \"" + portName + "\" not found";
        }else{
            lastError = "Unknown error when opening a handle to \"" + portName + "\"";
        }
    }else{
        DCB dcbSerialParams = {0};

        if (!GetCommState(hSerial, &dcbSerialParams)){
            lastError = "Failed to get serial parameters";
        }else{

            dcbSerialParams.BaudRate=CBR_9600;
            dcbSerialParams.ByteSize=8;
            dcbSerialParams.StopBits=ONESTOPBIT;
            dcbSerialParams.Parity=NOPARITY;

             if(!SetCommState(hSerial, &dcbSerialParams)){
                lastError = "Could not set Serial Port parameters";
             }else{
                 connected = true;
                 Sleep(ARDUINO_WAIT_TIME);
             }
        }
    }
    return connected;
}

void Serial::stop(){
    if(connected){
        connected = false;
        CloseHandle(hSerial);
    }
}

Serial::~Serial(){
    stop();
}

int Serial::readData(char *buffer, unsigned int nbChar){
    DWORD bytesRead;
    unsigned int toRead;

    ClearCommError(hSerial, &errors, &status);

    if(status.cbInQue>0)
    {
        if(status.cbInQue>nbChar){
            toRead = nbChar;
        }else{
            toRead = status.cbInQue;
        }

        if(ReadFile(hSerial, buffer, toRead, &bytesRead, NULL) && bytesRead > 0){
            return bytesRead;
        }
    }

    return -1;

}


bool Serial::writeData(char *buffer, unsigned int nbChar){
    DWORD bytesSend;

    if(!WriteFile(hSerial, (void *)buffer, nbChar, &bytesSend, 0)){
        ClearCommError(hSerial, &errors, &status);
        return false;
    }

    return true;
}

std::string Serial::readData(unsigned int nChars){
    DWORD bytesRead;
    unsigned int toRead;

    ClearCommError(hSerial, &errors, &status);

    if(status.cbInQue>0){
        if(status.cbInQue>nChars){
            toRead = nChars;
        }else{
            toRead = status.cbInQue;
        }

        char buffer[nChars];

        if(ReadFile(hSerial, buffer, toRead, &bytesRead, NULL) && bytesRead != 0){
            return std::string(buffer,nChars);
        }

    }

    return "";

}


bool Serial::writeData(std::string data){
    DWORD bytesSend;

    if(!WriteFile(hSerial, data.c_str(), data.size(), &bytesSend, 0)){
        ClearCommError(hSerial, &errors, &status);
        return false;
    }
    return true;
}

std::string Serial::readDataLength(unsigned int length, unsigned int timeout, unsigned int sleep){
    if(length==0) return "";

    char t[length];
    unsigned int readed = 0;
    int error;
    unsigned int totalTime=0, whileTime = 0;

    clock_t started = clock();

    while(readed<length && connected && (timeout==0 || clock()-started<(timeout*1000)/CLOCKS_PER_SEC)){
        totalTime += whileTime;

        error = readData(t+readed, length-readed);

        if(error!=-1)
            readed += error;
        else if(sleep>0) thSleep(sleep);

    }
    return std::string(t,readed);
}

bool Serial::isConnected(){
    return connected;
}

std::string Serial::getLastError(){
    return lastError;
}
