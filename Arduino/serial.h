#ifndef SERIAL_H
#define SERIAL_H

#define ARDUINO_WAIT_TIME 2000

#include <windows.h>
#include <string>

class Serial
{
    private:
        HANDLE hSerial;
        bool connected;
        COMSTAT status;
        DWORD errors;

        std::string lastError;

    public:
        Serial();
        Serial(const std::string portName);
        bool start(const std::string portName);
        void stop();
        ~Serial();

        int readData(char *buffer, unsigned int nbChar);
        std::string readData(unsigned int nChars);
        std::string readDataLength(unsigned int length, unsigned int timeout=0, unsigned int sleep=1);

        bool writeData(char *buffer, unsigned int nbChar);
        bool writeData(std::string data);

        bool isConnected();
        std::string getLastError();


};

#endif // SERIAL_H
