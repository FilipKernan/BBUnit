#include <WiFi.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <WebSerial.h>
#include <string.h>
#include "config.h"


class Comms
{
private:
    int tx;
    int rx;
    char buffer[BUFFER_SIZE];
    static void recvMsg(uint8_t *data, size_t len);
public:
    int write(const char*); // null terminated
    char* read();
    void init();
    Comms(int tx, int rx, int baud);
    ~Comms();
    
};
