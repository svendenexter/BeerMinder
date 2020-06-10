/*
 Name:		jog_steppers_2.ino
 Created:	6/4/2020 11:43:51 AM
 Author:	stefan
*/

/* Example sketch to control a stepper motor with TB6600 stepper motor driver, AccelStepper library and Arduino: acceleration and deceleration. More info: https://www.makerguides.com */

// Include the AccelStepper library:
#include <Servo.h>
#include <MultiStepper.h>
#include <AccelStepper.h>

// define pins
#pragma region 

#define dirPin1 2
#define stepPin1 3
#define dirPin2 4
#define stepPin2 5
#define dirPin3 6
#define stepPin3 7
#define dirPin4 8
#define stepPin4 9
#define dirPin5 10
#define stepPin5 11
#define dirPin6 12
#define stepPin6 13

#define enable_all_steppers 52

// data poorten

//knoppen
#define knop_axis_1_cw 22
#define knop_axis_1_ccw 23
#define knop_axis_2_cw 24
#define knop_axis_2_ccw 25
#define knop_axis_3_cw 26
#define knop_axis_3_ccw 27
#define knop_axis_4_cw 28
#define knop_axis_4_ccw 29
#define knop_axis_5_cw 30
#define knop_axis_5_ccw 31
#define knop_axis_6_cw 32
#define knop_axis_6_ccw 33
#define knop_tool_cw 34
#define knop_tool_ccw 35

#define knop_joggen 36
#define knop_noodstop 37
#define knop_reset 38
#define knop_dodemans 39

#define knop_enable_all_steppers 51

//switches
#define switch_axis_1 40
#define switch_axis_2 41
#define switch_axis_3_cw 42
#define switch_axis_3_ccw 43
#define switch_axis_4 44
#define switch_axis_5_cw 45
#define switch_axis_5_ccw 46
#define switch_axis_6 47 
#define switch_axis_2_ccw 48 //als nodig is

#define tool_servo_pin 49	//servo
#define tool_servo2_pin 50

//#define knop_enable_all_steppers 51
// 52 is used by enable all steppers

#define reserve 53 // is nog vrij

//12 analog ports
//a0 - a11
#pragma endregion 

//accelstepper declaration and servo
#pragma region 
// Define stepper motor connections and motor interface type. Motor interface type must be set to 1 when using a driver:
#define motorInterfaceType 1
// Create a new instance of the AccelStepper class:
AccelStepper stepper1 = AccelStepper(motorInterfaceType, stepPin1, dirPin6);
AccelStepper stepper2 = AccelStepper(motorInterfaceType, stepPin2, dirPin5);
AccelStepper stepper3 = AccelStepper(motorInterfaceType, stepPin3, dirPin4);
AccelStepper stepper4 = AccelStepper(motorInterfaceType, stepPin4, dirPin3);
AccelStepper stepper5 = AccelStepper(motorInterfaceType, stepPin5, dirPin2);
AccelStepper stepper6 = AccelStepper(motorInterfaceType, stepPin6, dirPin1);

MultiStepper steppers;

Servo servo_tool;

#pragma endregion 

//define limits of steppers
#pragma region  

#define limit_axis_1_cw 2000
#define limit_axis_1_ccw -2000
#define limit_axis_2_cw 2000
#define limit_axis_2_ccw -2000
#define limit_axis_3_cw 2000
#define limit_axis_3_ccw -2000
#define limit_axis_4_cw 2000
#define limit_axis_4_ccw -2000
#define limit_axis_5_cw 2000
#define limit_axis_5_ccw -2000
#define limit_axis_6_cw 2000
#define limit_axis_6_ccw -2000

#pragma endregion 

//variabelen
#pragma region
//variabelen voor snelle software verandering/instelling
int softwarejoggen = 1;

// start variabels of parameters
int servo_tool_pos = 1500; //set servo to midpoint 1500 (servo between 700 and 2300 or 1000 and 2000)
#pragma endregion


void setup() {
	// Set the maximum speed and acceleration:

	pinMode(knop_axis_1_cw, OUTPUT);
	pinMode(knop_axis_1_ccw, OUTPUT);
	pinMode(knop_axis_2_cw, OUTPUT);
	pinMode(knop_axis_2_ccw, OUTPUT);
	pinMode(knop_axis_3_cw, OUTPUT);
	pinMode(knop_axis_3_ccw, OUTPUT);
	pinMode(knop_axis_4_cw, OUTPUT);
	pinMode(knop_axis_4_ccw, OUTPUT);
	pinMode(knop_axis_5_cw, OUTPUT);
	pinMode(knop_axis_5_ccw, OUTPUT);
	pinMode(knop_axis_6_cw, OUTPUT);
	pinMode(knop_axis_6_ccw, OUTPUT);
	pinMode(knop_axis_6_cw, OUTPUT);
	pinMode(knop_tool_cw, OUTPUT);
	pinMode(knop_tool_ccw, OUTPUT);
	pinMode(knop_joggen, OUTPUT);

	stepper1.setMaxSpeed(1000);
	stepper1.setAcceleration(500);
	stepper2.setMaxSpeed(1000);
	stepper2.setAcceleration(500);
	stepper3.setMaxSpeed(1000);
	stepper3.setAcceleration(500);
	stepper4.setMaxSpeed(1000);
	stepper4.setAcceleration(500);
	stepper5.setMaxSpeed(1000);
	stepper5.setAcceleration(500);
	stepper6.setMaxSpeed(1000);
	stepper6.setAcceleration(500);

	//om 2 steppers tegelijk aan te sturen gebruik steppers
	steppers.addStepper(stepper1);
	steppers.addStepper(stepper2);

	servo_tool.attach(tool_servo_pin);
}


void joggen() {	
	/*
	stepper1.setMaxSpeed(1000);
	stepper1.setAcceleration(99999);
	stepper2.setMaxSpeed(1000);
	stepper2.setAcceleration(99999);
	stepper3.setMaxSpeed(1000);
	stepper3.setAcceleration(99999);
	stepper4.setMaxSpeed(1000);
	stepper4.setAcceleration(99999);
	stepper5.setMaxSpeed(1000);
	stepper5.setAcceleration(99999);
	stepper6.setMaxSpeed(1000);
	stepper6.setAcceleration(99999);
	*/
	
	//axis 1
	if (knop_axis_1_cw == HIGH) {
		stepper1.moveTo(stepper1.currentPosition() + 10);
		stepper1.setSpeed(1000);
		stepper1.runSpeed();
		//stepper1.run();
	}
	else if (knop_axis_1_ccw == HIGH) {
		stepper1.moveTo(stepper1.currentPosition() - 10);
		stepper1.setSpeed(1000);
		stepper1.runSpeed();
	}
	else {
		stepper1.stop();
	}
	//axis 2
	if (knop_axis_2_cw == HIGH) {
		stepper2.moveTo(stepper2.currentPosition() + 10);
		stepper2.setSpeed(1000);
		stepper2.runSpeed();
	}
	else if (knop_axis_2_ccw == HIGH) {
		stepper2.moveTo(stepper2.currentPosition() - 10);
		stepper2.setSpeed(1000);
		stepper2.runSpeed();
	}
	else {
		stepper2.stop();
	}
	//axis 3
	if (knop_axis_3_cw == HIGH) {
		stepper3.moveTo(stepper1.currentPosition() + 10);
		stepper3.setSpeed(1000);
		stepper3.runSpeed();
	}
	else if (knop_axis_3_ccw == HIGH) {
		stepper3.moveTo(stepper3.currentPosition() - 10);
		stepper3.setSpeed(1000);
		stepper3.runSpeed();
	}
	else {
		stepper3.stop();
	}
	//axis 4
	if (knop_axis_4_cw == HIGH) {
		stepper4.moveTo(stepper4.currentPosition() + 10);
		stepper4.setSpeed(1000);
		stepper4.runSpeed();
	}
	else if (knop_axis_4_ccw == HIGH) {
		stepper4.moveTo(stepper4.currentPosition() - 10);
		stepper4.setSpeed(1000);
		stepper4.runSpeed();
	}
	else {
		stepper4.stop();
	}
	//axis 5
	if (knop_axis_5_cw == HIGH) {
		stepper5.moveTo(stepper5.currentPosition() + 10);
		stepper5.setSpeed(1000);
		stepper5.runSpeed();
	}
	else if (knop_axis_5_ccw == HIGH) {
		stepper5.moveTo(stepper5.currentPosition() - 10);
		stepper5.setSpeed(1000);
		stepper5.runSpeed();
	}
	else {
		stepper5.stop();
	}
	//axis 6
	if (knop_axis_6_cw == HIGH) {
		stepper6.moveTo(stepper6.currentPosition() + 1);
		stepper6.setSpeed(1000);
		stepper6.runSpeed();
	}
	else if (knop_axis_6_ccw == HIGH) {
		stepper6.moveTo(stepper6.currentPosition() - 1);
		stepper6.setSpeed(1000);
		stepper6.runSpeed();
	}
	else {
		stepper6.stop();
	}	
	if (knop_tool_cw == HIGH) {
		servo_tool.writeMicroseconds(servo_tool_pos + 1);
	}
	else if (knop_tool_ccw == HIGH) {
		servo_tool.writeMicroseconds(servo_tool_pos - 1);
	}
}

void loop() {

	if (knop_joggen == HIGH || softwarejoggen == 1) {
		joggen();
	}
	else {
		stepper1.setMaxSpeed(1000);
		stepper1.setAcceleration(500);
		stepper2.setMaxSpeed(1000);
		stepper2.setAcceleration(500);
		stepper3.setMaxSpeed(1000);
		stepper3.setAcceleration(500);
		stepper4.setMaxSpeed(1000);
		stepper4.setAcceleration(500);
		stepper5.setMaxSpeed(1000);
		stepper5.setAcceleration(500);
		stepper6.setMaxSpeed(1000);
		stepper6.setAcceleration(500);
	}


}
