#include <Arduino.h>
#include "comms.h"
#include "motorController.h"
#include "config.h"
#include <DigitalIO.h>

MotorController* leftMotor;
MotorController* rightMotor;
Comms* comms;

int leftMotorDir = 53;
int leftMotorPwm = 10;

int rightMotorDir = 50;
int rightMotorPwm = 9;

int commsTx = 11;
int commsRx = 10;

unsigned int targetTime = 0;

uint8_t motorPower = 0;

/***
 * header:			01	10		11
 * 		sensor : gyro, encoder, plain text 2 bit
 * 		sensor #: 3 bits
 * 		axis for gyro: 2 bits
 * 		extra 0 bit
 * 	8 byte message
 *  1 byte of 0 if done, 1 if contunued in next message
 *  
 * 
 */
/***
 * header: 
 * 		message type: 1 byte
 * 		3 bits:
 * 			direct chassis power
 * 			direct head power
 * 			chassis position control
 * 			chassis velocity control
 * 			head position control
 * 			other message
 * 		
 * 
 *  motor power 1: 1 byte	
 *  time to run 1: 1 byte
 * 	motor power 2: 1 byte	
 *  time to run 2: 1 byte
 *  motor power 3: 1 byte	
 *  time to run 3: 1 byte
 *  motor power 4: 1 byte	
 *  time to run 4: 1 byte
 * 	1 byte of 0 if done, 1 if contunued in next message
 */

/***
 * TODO: 
 * Today
 * 		Test encoder output
 * 		test the bluetooth
 * Tommorow
 * 		set up packets
 * 		Read from gyro
 * 		
 * 
 */

void setup() {
	// put your setup code here, to run once:
	// leftMotor = new MotorController(leftMotorDir, leftMotorPwm);
	// rightMotor = new MotorController(rightMotorDir, rightMotorPwm);

	comms = new Comms(commsTx, commsRx, 9600);
	comms->init();
	// comms->init();
	Serial.begin(9600);

	Serial.println("booting up");
	targetTime = millis();


	// pinMode(leftMotorPwm, OUTPUT);
	// pinMode(leftMotorDir, OUTPUT);
	// pinMode(rightMotorPwm, OUTPUT);
	// pinMode(rightMotorDir, OUTPUT);



	// attachInterrupt(digitalPinToInterrupt(LEFT_MOTOR_INT), leftEncoderChange, CHANGE);
	// fastPinMode(LEFT_MOTOR_A, INPUT);
	// fastPinMode(LEFT_MOTOR_B, INPUT);

}

void loop() {
	// put your main code here, to run repeatedly:
	String value = comms->read();
	if (value != "") {
		Serial.println(value );	
	}
	
	
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
	int status = comms->write("alive");



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