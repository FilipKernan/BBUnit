#include "comms.h"
#include <Arduino.h>


// AsyncWebServer server(80);


Comms::Comms(int tx, int rx, int baud)
{
    baudRate = baud;

}

Comms::~Comms()
{
    // free(serial);
}

void Comms::init() {
    // WebSerial.begin(&server);
    /* Attach Message Callback */
    // WebSerial.msgCallback(Comms::recvMsg);
    // server.begin();
}

// TODO: change to use write instead of print, also define packet
int Comms::write(const char* buff) {
    // int msgSize = 10;
    // // if (serial->availableForWrite() <= msgSize) {
    // //     // serial->write(buff, msgSize);
    // //     return 0;
    // // }
    // serial->println(buff);
    return 0;
}


char* Comms::read() {
    // int size = serial->available();
    // if (size) {
    //     msg = new char[size + 1];
    //     for (int i = 0; i<10; i++) {
    //         msg[i] = serial->read();
    //     }
    //     return msg;
    // }
    return NULL;
    
}


void Comms::recvMsg(uint8_t *data, size_t len){
  WebSerial.println("Received Data...");
  String d = "";
  for(int i=0; i < len; i++){
    d += char(data[i]);
  }
  WebSerial.println(d);
}