#include <WiFi.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <WebSerial.h>
#include "config.h"


class Comms
{
private:
    uint8_t commsFlag; //0 if buffer can be overwriten from remote, 1 if buffer needs to be read, 2 if buffer needs to be sent
    char buffer[BUFFER_SIZE];
    char localReadBuffer[BUFFER_SIZE];
    char localWriteBuffer[BUFFER_SIZE];
public:
    int write(const char*); // null terminated
    char* read();
    void relay(WiFiClient* client);
    void init();
    int newData;
    Comms();
    ~Comms();
    
};
