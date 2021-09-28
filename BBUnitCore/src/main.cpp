#include <Arduino.h>
#include "comms.h"
#include <WiFi.h>
#include <ESPmDNS.h>
#include <WiFiUdp.h>
#include <ArduinoOTA.h>
// #include "motorController.h"
#include "config.h"

// MotorController* leftMotor;
// MotorController* rightMotor;
Comms* comms;

const int serverPort = 1234;
const int baudrate = 115200;
const int rs_config = SERIAL_8N1;
  
// reading buffor config
#define BUFFER_SIZE 1024

WiFiServer server;
byte buff[BUFFER_SIZE];

int commsTx = 11;
int commsRx = 10;

unsigned int targetTime = 0;

uint8_t motorPower = 0;

/***
 * header:			01	10		11, 			00
 * 		sensor : gyro, encoder, plain text 2 bit, command ||| command and plain text have the rest of the header filled with 0s
 * 		sensor #: 3 bits
 * 		axis for gyro: 2 bits
 * 		extra 0 bit
 * 	Command messages
 * 		Reboot message with all 1s
 * 		enter ota update - message with alternating 10s
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
 *  motor power 1: 1 byte, 	
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
// DigitalPin<FRONT_MOTOR_A> frontA;
// DigitalPin<FRONT_MOTOR_B> frontB;

// DigitalPin<RIGHT_MOTOR_A> rightA;
// DigitalPin<RIGHT_MOTOR_B> rightB;

// DigitalPin<BACK_MOTOR_A> backA;
// DigitalPin<BACK_MOTOR_B> backB;

// DigitalPin<LEFT_MOTOR_A> leftA;
// DigitalPin<LEFT_MOTOR_B> leftB;

void setup() {


	// put your setup code here, to run once:
	// leftMotor = new MotorController(LEFT_MOTOR_DIR, LEFT_MOTOR_PWM, 3, &leftEncoderCount);
	// rightMotor = new MotorController(RIGHT_MOTOR_DIR, RIGHT_MOTOR_PWM, 1, &rightEncoderCount);

	// comms = new Comms(commsRx, commsTx, 9600);
	comms->init();
	comms->write("I am alive");
	// comms->init();
	Serial.begin(9600);

	Serial.println("booting up");
	targetTime = millis();
	WiFi.mode(WIFI_STA);
	WiFi.begin(ssid, password);

	while (WiFi.waitForConnectResult() != WL_CONNECTED) {
		Serial.println("Connection Failed! Rebooting...");
		delay(5000);
		ESP.restart();
	}


	
	ArduinoOTA.setHostname("br95");
	ArduinoOTA.setPassword("StormBlessed");



	ArduinoOTA
    	.onStart([]() {
			String type;
			if (ArduinoOTA.getCommand() == U_FLASH)
				type = "sketch";
			else // U_SPIFFS
				type = "filesystem";

			// NOTE: if updating SPIFFS this would be the place to unmount SPIFFS using SPIFFS.end()
			Serial.println("Start updating " + type);
		})
		.onEnd([]() {
			Serial.println("\nEnd");
		})
		.onProgress([](unsigned int progress, unsigned int total) {
			Serial.printf("Progress: %u%%\r", (progress / (total / 100)));
		})
		.onError([](ota_error_t error) {
			Serial.printf("Error[%u]: ", error);
			if (error == OTA_AUTH_ERROR) Serial.println("Auth Failed");
			else if (error == OTA_BEGIN_ERROR) Serial.println("Begin Failed");
			else if (error == OTA_CONNECT_ERROR) Serial.println("Connect Failed");
			else if (error == OTA_RECEIVE_ERROR) Serial.println("Receive Failed");
			else if (error == OTA_END_ERROR) Serial.println("End Failed");
		});

  	ArduinoOTA.begin();

	Serial.println("Ready");
	Serial.print("IP address: ");
	Serial.println(WiFi.localIP());

	// attachInterrupt(digitalPinToInterrupt(FRONT_MOTOR_INT), frontEncoderChange, CHANGE);
	// frontA.mode(INPUT);
	// frontB.mode(INPUT);

	// attachInterrupt(digitalPinToInterrupt(RIGHT_MOTOR_INT), rightEncoderChange, CHANGE);
	// rightA.mode(INPUT);
	// rightB.mode(INPUT);

	// attachInterrupt(digitalPinToInterrupt(BACK_MOTOR_INT), backEncoderChange, CHANGE);
	// backA.mode(INPUT);
	// backB.mode(INPUT);

	// attachInterrupt(digitalPinToInterrupt(LEFT_MOTOR_INT), leftEncoderChange, CHANGE);
	// leftA.mode(INPUT);
	// leftB.mode(INPUT);

	server = WiFiServer(serverPort);
	server.begin();
	delay(1000);

}

void loop() {
	char* readPointer = comms->read();
	if (readPointer != NULL) {
		Serial.println("packet detected, writing to motors");
		Serial.println(readPointer);
		// rightMotor->write(LOW, *(readPointer + 3));
		// leftMotor->write(HIGH, *(readPointer + 7));
		// targetTime = millis() + 10 * (*(readPointer + 2));

	}
	
	ArduinoOTA.handle();

	// wait for client
  	WiFiClient client = server.available();
	if (!client)
	return;
	
	while (client.connected()) {
	int size = 0;
	
	// read data from wifi client and send to serial
	while ((size = client.available())) {
			size = (size >= BUFFER_SIZE ? BUFFER_SIZE : size);
			client.read(buff, size);
			Serial.write(buff, size);
			Serial.flush();
	}
	
	// read data from serial and send to wifi client
	while ((size = Serial.available())) {
			size = (size >= BUFFER_SIZE ? BUFFER_SIZE : size);
			Serial.readBytes(buff, size);
			client.write(buff, size);
			client.flush();
	}
	}
   client.stop();
	// while (comms->serial->available())	{
	// 	String power = comms->serial->readString();
	// 	Serial.println(power);
	// 	int powerInt = power.toInt();
	// 	Serial.println(powerInt);
	// 	// rightMotor->write(LOW, powerInt);
	// 	// leftMotor->write(HIGH, powerInt);
	// 	targetTime = millis() + 1000;
	// }
	// comms->write("I am stormblessed");
	// Serial.println(rightMotor->getEncoderCount());
	// Serial.println(leftMotor->getEncoderCount());


}



// void leftEncoderChange() {
// 	bool a = fastDigitalRead(LEFT_MOTOR_A);
// 	bool b = fastDigitalRead(LEFT_MOTOR_B);

// 	leftEncoderCount += (a ^ lastLeftB) - (lastLeftA ^ b);

// 	lastLeftA = a;
// 	lastLeftB = b;
	
// }

// void rightEncoderChange() {
// 	bool a = fastDigitalRead(RIGHT_MOTOR_A);
// 	bool b = fastDigitalRead(RIGHT_MOTOR_B);

// 	rightEncoderCount += (a ^ lastRightB) - (lastRightA ^ b);

// 	lastRightA = a;
// 	lastRightB = b;
	
// }

// void frontEncoderChange() {
// 	bool a = fastDigitalRead(FRONT_MOTOR_A);
// 	bool b = fastDigitalRead(FRONT_MOTOR_B);

// 	frontEncoderCount += (a ^ lastFrontB) - (lastFrontA ^ b);

// 	lastFrontA = a;
// 	lastFrontB = b;
	
// }

// void backEncoderChange() {
// 	bool a = fastDigitalRead(BACK_MOTOR_A);
// 	bool b = fastDigitalRead(BACK_MOTOR_B);

// 	backEncoderCount += (a ^ lastBackB) - (lastBackA ^ b);

// 	lastBackA = a;
// 	lastBackB = b;
	
// }