#include "comms.h"
#include <Arduino.h>


// AsyncWebServer server(80);


Comms::Comms()
{
    commsFlag = 0;
    newData = 0;
    for (int i = 0; i < BUFFER_SIZE; i++) {
        localWriteBuffer[i] = 0;
        buffer[i] = 0;
        localReadBuffer[i] = 0;
    }

}

Comms::~Comms()
{
    // free(serial);
}

void Comms::init() {
    
}
// Are You Alive??
// TODO: change to use write instead of print, also define packet
int Comms::write(const unsigned char* buff) {
    // xSemaphoreTake(commsMutex, portMAX_DELAY);

    for (int i = 0; i < BUFFER_SIZE; i++){
        localWriteBuffer[i] = *(buff + i);
    }
    newData = 1;
    // xSemaphoreGive(commsMutex);


    return 0;
}


unsigned char* Comms::read() {
    newData = 0;
    return this->localReadBuffer;
}



void Comms::relay(WiFiClient* client) {
    uint8_t bufferIndex = 0;
    // if the comms flag is 0, read from client
    if (commsFlag == 0) {
        // Serial.write("Read flag is 0");
        for (int i = 0; i < BUFFER_SIZE; i++) {
            buffer[i] = 0;
        }
        while (client->available() > 0) {
            buffer[bufferIndex] = client->read();
            bufferIndex++;
            if (buffer[0] != 0 && buffer[0] != 13){
                // Serial.printf("input is %0X", buffer);
                commsFlag = 1;
            }
            esp_task_wdt_reset();
        }
    } else if (commsFlag == 1 && newData == 1) {
        // Serial.write("Read flag is 1");

        // Serial.write(buffer, BUFFER_SIZE);
        newData = 1;
        for (int i = 0; i < BUFFER_SIZE; i++){
            localReadBuffer[i] = buffer[i];
        }
        Serial.write(buffer, BUFFER_SIZE);
        commsFlag = 2;
        
    } else if (commsFlag == 2) { // if comms flag is 2, read from buffer and sent to source
        // Serial.write("Read flag is 2");

        for (int i = 0; i < BUFFER_SIZE; i++){
            buffer[i] = localWriteBuffer[i];
        }
        Serial.println("what is about to be sent");
        Serial.write(buffer, BUFFER_SIZE);
        client->write(buffer, BUFFER_SIZE);
        Serial.println("sending done");
        // client->flush();
        commsFlag = 0;
    }
    esp_task_wdt_reset();

    
}

