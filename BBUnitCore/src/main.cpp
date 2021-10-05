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
TaskHandle_t CommsTask;
TaskHandle_t RobotTask;
const int baudrate = 115200;
const int rs_config = SERIAL_8N1;
  
const char* ssid     = "BR-95";
const char* password = "StormBlessed";


unsigned int targetTime = 0;

uint8_t motorPower = 0;

WiFiServer wifiServer(80);

static SemaphoreHandle_t commsMutex;
#define BUFFER_SIZE 10
uint8_t commsFlag = 0; //0 if buffer can be overwriten from remote, 1 if buffer needs to be read, 2 if buffer needs to be sent
char buffer[BUFFER_SIZE];
/**
 * In order to take the mutex 
 * xSemaphoreTake(commsMutex, portMAX_DELAY);
 * In order to release the mutex
 * xSemaphoreGive(commsMutex);
 */

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

	commsMutex = xSemaphoreCreateMutex();
	Serial.begin(115200);

	Serial.println("booting up");
	targetTime = millis();
	// WiFi.mode(WIFI_STA);
	WiFi.softAP(ssid, password);

	IPAddress IP = WiFi.softAPIP();


	
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
	Serial.println(IP);

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


	wifiServer.begin();

	// create tasks

	  //create a task that will be executed in the Task1code() function, with priority 1 and executed on core 0
	xTaskCreatePinnedToCore(
						CommsTaskLoop,   /* Task function. */
						"CommsTask",     /* name of task. */
						10000,       /* Stack size of task */
						NULL,        /* parameter of the task */
						1,           /* priority of the task */
						&CommsTask,      /* Task handle to keep track of created task */
						0);          /* pin task to core 0 */                  
	delay(500); 

	//create a task that will be executed in the Task2code() function, with priority 1 and executed on core 1
	xTaskCreatePinnedToCore(
						RobotTaskLoop,   /* Task function. */
						"RobotTask",     /* name of task. */
						10000,       /* Stack size of task */
						NULL,        /* parameter of the task */
						1,           /* priority of the task */
						&RobotTask,      /* Task handle to keep track of created task */
						1);          /* pin task to core 1 */
	delay(500); 

	esp_task_wdt_init(200, 0);
	esp_task_wdt_add(CommsTask);
	esp_task_wdt_add(RobotTask);


}

void RobotTaskLoop(void* paramaters) {
	while(true) {
		ArduinoOTA.handle();
		// comms test
		xSemaphoreTake(commsMutex, portMAX_DELAY);
		if (commsFlag == 1) {

			Serial.write(buffer, BUFFER_SIZE);

			buffer[0] = 'A';
			buffer[1] = 'l';
			buffer[2] = 'i';
			buffer[3] = 'v';
			buffer[4] = 'e';
			buffer[5] = '\n';

			Serial.write(buffer, BUFFER_SIZE);
			commsFlag = 2;
			
		}
		xSemaphoreGive(commsMutex);
		esp_task_wdt_reset();
	}
}


void CommsTaskLoop(void* paramaters) {
	while(true) {
		WiFiClient client = wifiServer.available();
	
			if (client){
				// this while loop is nessary, have it run on the second core so that it does not harm everything
				while (client.connected()) {
					// Serial.println("connected to client");
					// read data from wifi client and send to serial
					uint8_t bufferIndex = 0;
					xSemaphoreTake(commsMutex, portMAX_DELAY);
					if (commsFlag == 0) {
						for (int i = 0; i < BUFFER_SIZE; i++) {
							buffer[i] = 0;
						}
						while (client.available() > 0) {
							
							buffer[bufferIndex] = client.read();
							bufferIndex++;
							// Serial.println(buffer[bufferIndex]);
							commsFlag = 1;
						}
					} else if (commsFlag == 2) {
						// for (int i = 0; i < BUFFER_SIZE; i++) {
						// 	Serial.print(buffer);
						// 	client.write(buffer);
						// }
						Serial.write(buffer, BUFFER_SIZE);
						client.write(buffer, BUFFER_SIZE);
						client.flush();
						commsFlag = 0;
					}
					xSemaphoreGive(commsMutex);
					

					
				
					
				}
			}
			
		client.stop();
		esp_task_wdt_reset();
	}
}


void loop() {
	// char* readPointer = comms->read();
	// if (readPointer != NULL) {
	// 	Serial.println("packet detected, writing to motors");
	// 	Serial.println(readPointer);
	// 	// rightMotor->write(LOW, *(readPointer + 3));
	// 	// leftMotor->write(HIGH, *(readPointer + 7));
	// 	// targetTime = millis() + 10 * (*(readPointer + 2));

	// }
	


	// wait for client
	// Serial.println("waiting for client");
  	
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