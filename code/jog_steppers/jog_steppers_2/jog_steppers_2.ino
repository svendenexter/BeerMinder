/*
 Name:		jog_steppers_2.ino
 Created:	6/4/2020 11:43:51 AM
 Author:	stefa
*/

/* Example sketch to control a stepper motor with TB6600 stepper motor driver, AccelStepper library and Arduino: acceleration and deceleration. More info: https://www.makerguides.com */

// Include the AccelStepper library:
#include <MultiStepper.h>
#include <AccelStepper.h>

#pragma region define pins	

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

//knoppen
#define knop_axis_1_cw 22
#define knop_axis_1_ccw 23
#define knop_axis_2_cw 24
#define knop_axis_2_ccw 25
#define knop_axis_3_cw 26
#define knop_axis_3_ccw 27
#define knop_axis_4_cw 28
#define knop_axis_5_ccw 29
#define knop_axis_5_cw 30
#define knop_axis_5_ccw 31
#define knop_axis_6_cw 32
#define knop_axis_6_ccw 33
//leds
#define led_axis_1_cw 22
#define led_axis_1_ccw 23
#define led_axis_2_cw 24
#define led_axis_2_ccw 25
#define led_axis_3_cw 26
#define led_axis_3_ccw 27
#define led_axis_4_cw 28
#define led_axis_5_ccw 29
#define led_axis_5_cw 30
#define led_axis_5_ccw 31
#define led_axis_6_cw 32
#define led_axis_6_ccw 33

#pragma endregion 

#pragma region define pins	

#pragma endregion 

#pragma region accelstepper declaration

#define motorInterfaceType 1
// Create a new instance of the AccelStepper class:
AccelStepper stepper1 = AccelStepper(motorInterfaceType, stepPin1, dirPin6);
AccelStepper stepper2 = AccelStepper(motorInterfaceType, stepPin2, dirPin5);
AccelStepper stepper3 = AccelStepper(motorInterfaceType, stepPin3, dirPin4);
AccelStepper stepper4 = AccelStepper(motorInterfaceType, stepPin4, dirPin3);
AccelStepper stepper5 = AccelStepper(motorInterfaceType, stepPin5, dirPin2);
AccelStepper stepper6 = AccelStepper(motorInterfaceType, stepPin6, dirPin1);

#pragma endregion 

// Define stepper motor connections and motor interface type. Motor interface type must be set to 1 when using a driver:

void setup() {
	// Set the maximum speed and acceleration:
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

void loop() {
	// Set the target position:
	stepper.moveTo(8000);
	// Run to target position with set speed and acceleration/deceleration:
	stepper.runToPosition();

	delay(1000);

	// Move back to zero:
	stepper.moveTo(0);
	stepper.runToPosition();

	delay(1000);
}