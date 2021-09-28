#include "motorController.h"
#include <Arduino.h>


MotorController::MotorController(int dir, int pwm, uint8_t num, volatile long* encoderCount) {
    this->dirPin = dir;
    this->pwmPin = pwm;
    this->num = num;
    this->encoderCount = encoderCount;
    pinMode(pwm, OUTPUT);
    pinMode(dir, OUTPUT);
}

MotorController::~MotorController() {
}


void MotorController::write(int direction, int pwmSpeed){
    digitalWrite(this->dirPin, direction);
    // analogWrite(this->pwmPin, pwmSpeed); need to use ledcWrite()
}

/***
 * TODO: Write flip function to flip a motor's polarity
 * TODO: Implement position control with encoders & pid
 * TODO: Implement velocity control
 * NOTE: ensure controlers are controled by timers & only enable one type
 *          of control at a time
 * 
 */

long MotorController::getEncoderCount() {
    return *(this->encoderCount);
}

char* MotorController::encoderToComms() {
    char header = 0x00;
    header = header | (0b10 << 6);
    header = header | (this->num << 3);
    char buff[10];
    buff[0] = header;
    for (int i = 1; i < 9; i++) {
        buff[i] = ((*this->encoderCount) >> ((i - 1) * 8)) & 0xFF;
    }
    buff[9] = 0x00;
}