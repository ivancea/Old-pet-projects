#include "timers.h"

#include "Serial.h"

Serial::Serial(const std::string portName)
{
    //We're not yet connected
    connected = false;

    //Try to connect to the given port throuh CreateFile
    hSerial = CreateFile(portName.c_str(),
            GENERIC_READ | GENERIC_WRITE,
            0,
            NULL,
            OPEN_EXISTING,
            FILE_ATTRIBUTE_NORMAL,
            NULL);

    //Check if the connection was successfull
    if(hSerial==INVALID_HANDLE_VALUE)
    {
        //If not success full display an Error
        if(GetLastError()==ERROR_FILE_NOT_FOUND){

            //Print Error if neccessary
            ///std::cerr << "ERROR: Handle was not attached. Reason: " << portName << " not available." << std::endl;

        }
        else
        {
            ///std::cerr << "ERROR!!!";
        }
    }
    else
    {
        //If connected we try to set the comm parameters
        DCB dcbSerialParams = {0};

        //Try to get the current
        if (!GetCommState(hSerial, &dcbSerialParams))
        {
            //If impossible, show an error
            printf("failed to get current serial parameters!");
        }
        else
        {
            //Define serial connection parameters for the arduino board
            dcbSerialParams.BaudRate=CBR_9600;
            dcbSerialParams.ByteSize=8;
            dcbSerialParams.StopBits=ONESTOPBIT;
            dcbSerialParams.Parity=NOPARITY;

             //Set the parameters and check for their proper application
             if(!SetCommState(hSerial, &dcbSerialParams))
             {
                ///std::cerr << "ALERT: Could not set Serial Port parameters" << std::endl;
             }
             else
             {
                 //If everything went fine we're connected
                 connected = true;
                 //We wait 2s as the arduino board will be reseting
                 Sleep(ARDUINO_WAIT_TIME);
             }
        }
    }

}

Serial::~Serial()
{
    //Check if we are connected before trying to disconnect
    if(connected)
    {
        //We're no longer connected
        connected = false;
        //Close the serial handler
        CloseHandle(hSerial);
    }
}

int Serial::ReadData(char *buffer, unsigned int nbChar)
{
    //Number of bytes we'll have read
    DWORD bytesRead;
    //Number of bytes we'll really ask to read
    unsigned int toRead;

    //Use the ClearCommError function to get status info on the Serial port
    ClearCommError(hSerial, &errors, &status);

    //Check if there is something to read
    if(status.cbInQue>0)
    {
        //If there is we check if there is enough data to read the required number
        //of characters, if not we'll read only the available characters to prevent
        //locking of the application.
        if(status.cbInQue>nbChar)
        {
            toRead = nbChar;
        }
        else
        {
            toRead = status.cbInQue;
        }

        //Try to read the require number of chars, and return the number of read bytes on success
        if(ReadFile(hSerial, buffer, toRead, &bytesRead, NULL) && bytesRead != 0)
        {
            return bytesRead;
        }

    }

    //If nothing has been read, or that an error was detected return -1
    return -1;

}


bool Serial::WriteData(char *buffer, unsigned int nbChar)
{
    DWORD bytesSend;

    //Try to write the buffer on the Serial port
    if(!WriteFile(hSerial, (void *)buffer, nbChar, &bytesSend, 0))
    {
        //In case it don't work get comm error and return false
        ClearCommError(hSerial, &errors, &status);

        return false;
    }
    else
        return true;
}

std::string Serial::ReadData(unsigned int nChars)
{
    //Number of bytes we'll have read
    DWORD bytesRead;
    //Number of bytes we'll really ask to read
    unsigned int toRead;

    //Use the ClearCommError function to get status info on the Serial port
    ClearCommError(hSerial, &errors, &status);

    //Check if there is something to read
    if(status.cbInQue>0)
    {
        //If there is we check if there is enough data to read the required number
        //of characters, if not we'll read only the available characters to prevent
        //locking of the application.
        if(status.cbInQue>nChars)
        {
            toRead = nChars;
        }
        else
        {
            toRead = status.cbInQue;
        }

        //Try to read the require number of chars, and return the number of read bytes on success

        char buffer[nChars];

        if(ReadFile(hSerial, buffer, toRead, &bytesRead, NULL) && bytesRead != 0)
        {
            return std::string(buffer,nChars);
        }

    }

    //If nothing has been read, or that an error was detected return ""
    return "";

}


bool Serial::WriteData(std::string data)
{
    DWORD bytesSend;

    //Try to write the buffer on the Serial port
    if(!WriteFile(hSerial, data.c_str(), data.size(), &bytesSend, 0))
    {
        //In case it don't work get comm error and return false
        ClearCommError(hSerial, &errors, &status);

        return false;
    }
    else
        return true;
}

std::string Serial::ReadDataLength(unsigned int length, uint8_t sleep){
    if(length==0) return "";
    char t[length];
    unsigned int readed = 0;
    int error;
    while(readed<length && connected){
        error = ReadData(t+readed, length-readed);
        if(error!=-1)
            readed += error;
        else if(sleep>0) thSleep(sleep)
;   }
    return std::string(t,readed);
}

bool Serial::IsConnected()
{
    //Simply return the connection status
    return connected;
}
