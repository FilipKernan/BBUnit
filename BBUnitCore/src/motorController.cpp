#include "motorController.h"
#include <Arduino.h>


MotorController::MotorController(int dir, int pwm) {
    this->dirPin = dir;
    this->pwmPin = pwm;
    pinMode(pwm, OUTPUT);
    pinMode(dir, OUTPUT);
}

MotorController::~MotorController() {
}


void MotorController::write(int direction, int pwmSpeed){
    digitalWrite(this->dirPin, direction);
    analogWrite(this->pwmPin, pwmSpeed);
}

/***
 * TODO: Write flip function to flip a motor's polarity
 * TODO: Implement position control with encoders & pid
 * TODO: Implement velocity control
 * NOTE: ensure controlers are controled by timers & only enable one type
 *          of control at a time
 * 
 * 
 * /