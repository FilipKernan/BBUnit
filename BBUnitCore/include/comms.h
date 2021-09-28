#include <WiFi.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <WebSerial.h>
#include <string.h>


class Comms
{
private:
    int tx;
    int rx;
    char* msg;
    int baudRate;
    static void recvMsg(uint8_t *data, size_t len);
public:
    // BluetoothSerial* serial;
    int write(const char*); // null terminated
    char* read();
    void init();
    Comms(int tx, int rx, int baud);
    ~Comms();
    
};
