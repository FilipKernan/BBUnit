#include <Arduino.h>
#include "config.h"
#include "comms.h"
#include <WiFi.h>
#include <ros.h>
#include <std_msgs/String.h>
#include <std_msgs/Int32.h>

#include <ArduinoOTA.h>
// #include "motorController.h"


// MotorController* leftMotor;
// MotorController* rightMotor;
Comms* comms;
TaskHandle_t CommsTask;
TaskHandle_t RobotTask;
const int baudrate = 115200;
  
unsigned int targetTime = 0;

uint8_t motorPower = 0;




volatile long leftEncoderCount = 0;
volatile bool lastLeftA = 0;
volatile bool lastLeftB = 0;

volatile long rightEncoderCount = 0;
volatile bool lastRightA = 0;
volatile bool lastRightB = 0;

volatile long frontEncoderCount = 0;
volatile bool lastFrontA = 0;
volatile bool lastFrontB = 0;

volatile long backEncoderCount = 0;
volatile bool lastBackA = 0;
volatile bool lastBackB = 0;

const char* ssid     = "Nighthawk-2.4G";
const char* password = "9PoundHammer";

IPAddress server(192,168,1,1); // need to change this and set up a static server

const uint16_t serverPort = 11411; // change based on server config

ros::NodeHandle nh;

// make callback prototypes
void subCB(const std_msgs::Int32 &msg);

// system constants for pubs
std_msgs::String str_msg;


// publishers
ros::Publisher chatter("chatter", &str_msg);


// subscribers
ros::Subscriber<std_msgs::Int32> sub("incoming", &subCB);

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

	comms = new Comms();

	Serial.begin(9600);

	Serial.println("booting up");
	targetTime = millis();
	// WiFi.begin();
	WiFi.mode(WIFI_STA);


	WiFi.begin(ssid, password);
	

	// TODO: change this
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


	Serial.println("IP address: ");
  	Serial.println(WiFi.localIP());

	nh.getHardware()->setConnection(server, serverPort);
	nh.initNode();


	// Another way to get IP
	Serial.print("IP = ");
	Serial.println(nh.getHardware()->getLocalIP());


	// setup publishers
  	nh.advertise(chatter);


	// setup subscribers
	nh.subscribe(sub);

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


	





}



// change this to handle robot loop
void processCommand() {
	// unsigned char messageIndex = currentCommand[0];
	int currentCommandType = 0;

	// unsigned char isMultiPart = currentCommand[2];
	switch(currentCommandType){
		case 0: // something went wront
			break;
		case 1: // reboot
			break;
		case 2: // report alive
			break;
		case 3: // OTA
			break;
		case 4: // report motor
		case 5: // report gyro
			// currentCommandData[0] = currentCommand[13];
			break;
		case 8: // report goal

			break;
		case 9: // set motor power
			// currentCommandData[0] = currentCommand[12];
			// currentCommandData[1] = currentCommand[13];
			break;
		case 10: // set encoder goal
			
		case 11: // set gyro goal
			// for(int i = 3; i < BUFFER_SIZE - 1; i++){
			// 	if(currentCommand[i] != 0){
			// 		currentCommandData[0] = currentCommand[i];
			// 		long goal = 0;
			// 		for(int j = i + 1; j < BUFFER_SIZE - 1; i++){
			// 			goal += int(currentCommand[j]);
			// 		}
			// 		currentCommandData[1] = goal;
			// 		i = BUFFER_SIZE;
			// 	}
			// }
			break;
		case 12: // set pid
			// for(int i = 3; i < BUFFER_SIZE - 1; i++){
			// 	if(currentCommand[i] != 0){
			// 		currentCommandData[0] = currentCommand[i]; // pid index
			// 		currentCommandData[1] = currentCommand[i + 1];
			// 		String value = "";
			// 		for(int j = i + 1; j < BUFFER_SIZE - 1; i++){
			// 			value += currentCommand[j];
			// 		}
			// 		currentCommandData[1] = value.toFloat();
			// 		i = BUFFER_SIZE;
			// 	}
			// }
			break;
		case 13: // report pid
			break;
		case 15: // plain text
			// special parsing
			break;
		case 17: // play sound
			// currentCommandData[0] = currentCommand[13];
			break;
			
	}
	// if(!(isMultiPart | 0xA)){ // change this to properly handle long messages
	// 	// finishedProcessingCommand = true;
	// }
}




void loop() {
	ArduinoOTA.handle();
		

		

	processCommand(); // change this to do basic robot things

	while (WiFi.status() != WL_CONNECTED) {
		Serial.print(".");
  	}

	nh.spinOnce()
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