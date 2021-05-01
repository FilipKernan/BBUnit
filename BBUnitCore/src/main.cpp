#include <Arduino.h>
#include "comms.h"
#include "motorController.h"
#include "config.h"
#include <DigitalIO.h>

MotorController* leftMotor;
MotorController* rightMotor;
SoftwareSerial* comms;

int leftMotorDir = 53;
int leftMotorPwm = 10;

int rightMotorDir = 50;
int rightMotorPwm = 9;

int commsTx = 15;
int commsRx = 14;

unsigned int targetTime = 0;

uint8_t motorPower = 0;


/***
 * TODO: 
 * Today
 * 		Attach interupts
 * 		Set up ISRs
 * 		output data from encoders
 * Tommorow
 * 		Read from gyro
 * 		
 * 
 */

void setup() {
	// put your setup code here, to run once:
	// leftMotor = new MotorController(leftMotorDir, leftMotorPwm);
	// rightMotor = new MotorController(rightMotorDir, rightMotorPwm);

	// comms = new SoftwareSerial(commsTx, commsRx);
	// comms->begin(9600);
	// comms->init();
	Serial.begin(9600);
	Serial.println("booting up");
	targetTime = millis();


	// pinMode(leftMotorPwm, OUTPUT);
	// pinMode(leftMotorDir, OUTPUT);
	// pinMode(rightMotorPwm, OUTPUT);
	// pinMode(rightMotorDir, OUTPUT);



	attachInterrupt(digitalPinToInterrupt(LEFT_MOTOR_INT), leftEncoderChange, CHANGE);
	fastPinMode(LEFT_MOTOR_A, INPUT);
	fastPinMode(LEFT_MOTOR_B, INPUT);

}

void loop() {
	// put your main code here, to run repeatedly:
	// Serial.println(comms->available());
	// if (comms->available()){
	// 	Serial.println(comms->read());
	// }
	// TODO: move this logic into the comms file, and standerdize packet
	// if (msg.length()) {
	// 	Serial.println(msg);
	// 	int commaCount = 0;
	// 	for (unsigned int i = 0; i < msg.length(); ++i ){ 
	// 		if (msg.charAt(i) == ',') {
	// 			commaCount++;
	// 		}
	// 	}
	// 	String commands[commaCount + 1];
	// 	for (int i = 0; i < commaCount; ++i) {
	// 		int commaIndex = msg.indexOf(',');
	// 		if (commaIndex != -1) {
	// 			commands[i] = msg.substring(0, commaIndex);
	// 			msg.remove(0, commaIndex + 1);
	// 		} else {
	// 			commands[i] = msg;
	// 		}
			
	// 	}
	// 	int msSec = commands[0].toInt();
	// 	// targetTime = millis() + msSec;
	// 	motorPower = commands[1].toInt();
		
	// }

	// if (millis() < targetTime) {
	// 	leftMotor->write(LOW, 150);
	// 	rightMotor->write(HIGH, 150);
	// 	// digitalWrite(leftMotorDir, LOW);
    // 	// analogWrite(leftMotorPwm, 200);
	// 	// digitalWrite(rightMotorDir, LOW);
    // 	// analogWrite(rightMotorPwm, 200);
	// 	Serial.println("writing to motors");
	// } else {
	// 	leftMotor->write(LOW, 0);
	// 	rightMotor->write(LOW, 0);
		
	// }
	// int status = comms->println("alive");

	// Serial.print("I am alive, ");
	// Serial.print(status);
	// Serial.print("\n");

}



void leftEncoderChange() {
	bool a = fastDigitalRead(LEFT_MOTOR_A);
	bool b = fastDigitalRead(LEFT_MOTOR_B);

	leftEncoderCount += (a ^ lastLeftB) - (lastLeftA ^ b);

	lastLeftA = a;
	lastLeftB = b;
	
}

void rightEncoderChange() {
	bool a = fastDigitalRead(RIGHT_MOTOR_A);
	bool b = fastDigitalRead(RIGHT_MOTOR_B);

	rightEncoderCount += (a ^ lastRightB) - (lastRightA ^ b);

	lastRightA = a;
	lastRightB = b;
	
}

void frontEncoderChange() {
	bool a = fastDigitalRead(FRONT_MOTOR_A);
	bool b = fastDigitalRead(FRONT_MOTOR_B);

	frontEncoderCount += (a ^ lastFrontB) - (lastFrontA ^ b);

	lastFrontA = a;
	lastFrontB = b;
	
}

void backEncoderChange() {
	bool a = fastDigitalRead(BACK_MOTOR_A);
	bool b = fastDigitalRead(BACK_MOTOR_B);

	backEncoderCount += (a ^ lastBackB) - (lastBackA ^ b);

	lastBackA = a;
	lastBackB = b;
	
}