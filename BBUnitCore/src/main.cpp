#include <Arduino.h>
#include "comms.h"
#include "motorController.h"

MotorController* leftMotor;
MotorController* rightMotor;
Comms* comms;

int leftMotorDir = 0;
int leftMotorPwm = 0;

int rightMotorDir = 0;
int rightMotorPwm = 0;

int commsTx = 0;
int commsRx = 0;

unsigned int targetTime = 0;

uint8_t motorPower = 0;


void setup() {
  // put your setup code here, to run once:
  leftMotor = new MotorController(leftMotorDir, leftMotorPwm);
  rightMotor = new MotorController(rightMotorDir, rightMotorPwm);

  comms = new Comms(commsTx, commsRx, 38400);
  comms->init();
}

void loop() {
  // put your main code here, to run repeatedly:

    String msg = comms->read();
  // TODO: move this logic into the comms file, and standerdize packet
  if (msg.length()) {
    Serial.println(msg);
    int commaCount = 0;
    for (int i = 0; i < msg.length(); ++i ){ 
      if (msg.charAt(i) == ',') {
        commaCount++;
      }
    }
    String commands[commaCount + 1];
    for (int i = 0; i < commaCount; ++i) {
      int commaIndex = msg.indexOf(',');
      if (commaIndex != -1) {
        commands[i] = msg.substring(0, commaIndex);
        msg.remove(0, commaIndex + 1);
      } else {
        commands[i] = msg;
      }
      
    }
    int msSec = commands[0].toInt();
    targetTime = millis() + msSec;
    motorPower = commands[1].toInt();
    
  }

  if (millis() < targetTime) {
    leftMotor->write(LOW, motorPower);
    rightMotor->write(HIGH, motorPower);
  } else {
    leftMotor->write(LOW, 0);
    rightMotor->write(LOW, 0);
  }

}