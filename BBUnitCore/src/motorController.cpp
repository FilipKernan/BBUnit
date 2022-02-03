#include "motorController.h"
#include <Arduino.h>


MotorController::MotorController(int dir, int pwm, uint8_t num, volatile long* encoderCount, float kp, float ki, float kd, bool usePid, int forward) {
    this->dirPin = dir;
    this->pwmPin = pwm;
    this->num = num;
    this->encoderCount = encoderCount;
    this->kp = kp;
    this->ki = ki;
    this->kd = kd;
    this->goal = *encoderCount
    pinMode(pwm, OUTPUT);
    pinMode(dir, OUTPUT);
    this.effort = 0;
    for (int i = 0; i < 10; i++) {
        this->integral[i] = 0;
    }
    this.integralIndex = 0;
    this->usePid = usePid;
    ledcAttachPin(this->pwmPin, this->num);
    this->forward = forward;
}

MotorController::~MotorController() {
}


void MotorController::write(int direction, int pwmSpeed){
    digitalWrite(this->dirPin, direction);
    ledcWrite(this->num, pwmSpeed)
}

void MotorController::updatePID(float kp, float ki, float kd){
    this->kp = kp;
    this->ki = ki;
    this->kd = kd;
}

void MotorController::updateMotor() {
    if (usePid) {
        int error = (int)(goal - *encoderCount);
        integral[integralIndex] = error;
        unsigned long currentDelta = millis() - lastTime;
        timeDeltas[integralIndex] = currentDelta;
        long sum = 0;
        for (int i = 0; i < 10; i++) {
            sum += integral[i] * timeDeltas[i];
        }
        uint_8 lastIndex = (integralIndex + 9) % 10 
        effort = kp * error + ki * sum + kd * (error - integral[lastIndex])/currentDelta;
        lastTime = millis();
        if (effort > 0) {
            digitalWrite(this->dirPin, forward);
        } else {
            digitalWrite(this->dirPin, (forward +1)%2));
        }
        ledcWrite(this->num, abs(effort))
    }
    
}


void MotorController::setGoal(long goal){
    this->goal = goal;
}

long MotorController::setGoal() {
    return goal;
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