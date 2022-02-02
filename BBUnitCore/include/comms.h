#include <WiFi.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <WebSerial.h>
#include "config.h"


class Comms
{
private:
    uint8_t commsFlag; //0 if buffer can be overwriten from remote, 1 if buffer needs to be read, 2 if buffer needs to be sent
    unsigned char buffer[BUFFER_SIZE];
    unsigned char localReadBuffer[BUFFER_SIZE];
    unsigned char localWriteBuffer[BUFFER_SIZE];
public:
    int write(const unsigned char*); // null terminated
    unsigned char* read();
    void relay(WiFiClient* client);
    void init();
    int newData;
    Comms();
    ~Comms();
    
};
