/*
 Name:    jog_steppers_2.ino
 Created: 6/4/2020 11:43:51 AM
 Author:  stefan
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
/*
//knoppen //verkeerd gespiegeld
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
*/
//knoppen
#define knop_axis_1_cw 35
#define knop_axis_1_ccw 34
#define knop_axis_2_cw 33
#define knop_axis_2_ccw 32
#define knop_axis_3_cw 31
#define knop_axis_3_ccw 30
#define knop_axis_4_cw 29
#define knop_axis_4_ccw 28
#define knop_axis_5_cw 27
#define knop_axis_5_ccw 26
#define knop_axis_6_cw 25
#define knop_axis_6_ccw 24
#define knop_tool_cw 23
#define knop_tool_ccw 22


#define knop_noodstop_kast 36 
#define knop_stop_controller 37
#define knop_reset 38
#define knop_dodemans 39

#define knop_enable_all_steppers 51

//switches
#define switch_axis_1 40
#define switch_axis_2_cw 41
#define switch_axis_2_ccw 42 //als nodig is
#define switch_axis_3_cw 43
#define switch_axis_3_ccw 44
#define switch_axis_4 45
#define switch_axis_5_cw 46
#define switch_axis_5_ccw 47
#define switch_axis_6 48  //let op werkt andersom


#define tool_servo_pin 49 //servo
#define tool_servo2_pin 50

//#define knop_enable_all_steppers 51
// 52 is used by enable all steppers

#define power_steppers_off 53 // is nog vrij

//12 analog ports
//a0 - a11
#pragma endregion 

//accelstepper declaration and servo
#pragma region 
// Define stepper motor connections and motor interface type. Motor interface type must be set to 1 when using a driver:
#define motorInterfaceType 1
// Create a new instance of the AccelStepper class:
AccelStepper stepper1 = AccelStepper(motorInterfaceType, stepPin1, dirPin1);
AccelStepper stepper2 = AccelStepper(motorInterfaceType, stepPin2, dirPin2);
AccelStepper stepper3 = AccelStepper(motorInterfaceType, stepPin3, dirPin3);
AccelStepper stepper4 = AccelStepper(motorInterfaceType, stepPin4, dirPin4);
AccelStepper stepper5 = AccelStepper(motorInterfaceType, stepPin5, dirPin5);
AccelStepper stepper6 = AccelStepper(motorInterfaceType, stepPin6, dirPin6);

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
  Serial.begin(9600);
  Serial.println("setup started");
  pinMode(knop_axis_1_cw, INPUT);
  pinMode(knop_axis_1_ccw, INPUT);
  pinMode(knop_axis_2_cw, INPUT);
  pinMode(knop_axis_2_ccw, INPUT);
  pinMode(knop_axis_3_cw, INPUT);
  pinMode(knop_axis_3_ccw, INPUT);
  pinMode(knop_axis_4_cw, INPUT);
  pinMode(knop_axis_4_ccw, INPUT);
  pinMode(knop_axis_5_cw, INPUT);
  pinMode(knop_axis_5_ccw, INPUT);
  pinMode(knop_axis_6_cw, INPUT);
  pinMode(knop_axis_6_ccw, INPUT);
  pinMode(knop_axis_6_cw, INPUT);
  pinMode(knop_tool_cw, INPUT);
  pinMode(knop_tool_ccw, INPUT);

  pinMode(knop_stop_controller, INPUT);
  pinMode(knop_reset, INPUT);
  pinMode(knop_dodemans, INPUT);

  pinMode(knop_noodstop_kast, INPUT);
  pinMode(knop_enable_all_steppers, INPUT);

  pinMode(switch_axis_1, INPUT);
  pinMode(switch_axis_2_cw, INPUT);
  pinMode(switch_axis_2_ccw, INPUT);
  pinMode(switch_axis_3_cw, INPUT);
  pinMode(switch_axis_3_ccw, INPUT);
  pinMode(switch_axis_4, INPUT);
  pinMode(switch_axis_5_cw, INPUT);
  pinMode(switch_axis_5_ccw, INPUT);
  pinMode(switch_axis_6, INPUT);  //werkt met nc

  pinMode(power_steppers_off, OUTPUT);
  pinMode(enable_all_steppers, OUTPUT);

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
  Serial.println("setup compleet");
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
  if (digitalRead(knop_axis_1_cw) == HIGH) {
    stepper1.move(100);
    stepper1.setSpeed(1000);
    stepper1.run();
    Serial.println("knop 1 cw");
    Serial.println(String(stepper1.currentPosition()));
    Serial.println(String(stepper1.distanceToGo()));
    Serial.println(digitalRead(dirPin1));
    //stepper1.run();
  }
  else if (digitalRead(knop_axis_1_ccw) == HIGH){
    stepper1.move(-100);
    stepper1.setSpeed(1000);
    stepper1.run();
    Serial.println(" knop 1 ccw ");
    Serial.println(String(stepper1.currentPosition()));
    Serial.println(String(stepper1.distanceToGo()));
    Serial.println(digitalRead(dirPin1));
  }
  else {
    stepper1.stop();
  }
  //axis 2
  if (digitalRead(knop_axis_2_cw )== HIGH) {
    stepper2.moveTo(stepper2.currentPosition() + 1);
    stepper2.setSpeed(1000);
    stepper2.runSpeed();
  }
  else if (digitalRead(knop_axis_2_ccw )== HIGH) {
    stepper2.moveTo(stepper2.currentPosition() - 1);
    stepper2.setSpeed(1000);
    stepper2.runSpeed();
  }
  else {
    stepper2.stop();
  }
  //axis 3
  if (digitalRead(knop_axis_3_cw )== HIGH) {
    stepper3.moveTo(stepper3.currentPosition() + 1);
    stepper3.setSpeed(1000);
    stepper3.runSpeed();
  }
  else if (digitalRead(knop_axis_3_ccw )== HIGH) {
    stepper3.moveTo(stepper3.currentPosition() - 1);
    stepper3.setSpeed(1000);
    stepper3.runSpeed();
  }
  else {
    stepper3.stop();
  }
  //axis 4
  if (digitalRead(knop_axis_4_cw )== HIGH) {
    stepper4.moveTo(stepper4.currentPosition() + 1);
    stepper4.setSpeed(1000);
    stepper4.runSpeed();
  }
  else if (digitalRead(knop_axis_4_ccw )== HIGH) {
    stepper4.moveTo(stepper4.currentPosition() - 1);
    stepper4.setSpeed(1000);
    stepper4.runSpeed();
  }
  else {
    stepper4.stop();
  }
  //axis 5
  if (digitalRead(knop_axis_5_cw )== HIGH) {
    stepper5.moveTo(stepper5.currentPosition() + 1);
    stepper5.setSpeed(1000);
    stepper5.runSpeed();
  }
  else if (digitalRead(knop_axis_5_ccw )== HIGH) {
    stepper5.moveTo(stepper5.currentPosition() - 1);
    stepper5.setSpeed(1000);
    stepper5.runSpeed();
  }
  else {
    stepper5.stop();
  }
  //axis 6
  if (digitalRead(knop_axis_6_cw )== HIGH) {
    stepper6.moveTo(stepper6.currentPosition() + 1);
    stepper6.setSpeed(1000);
    stepper6.runSpeed();
  }
  else if (digitalRead(knop_axis_6_ccw )== HIGH) {
    stepper6.moveTo(stepper6.currentPosition() - 1);
    stepper6.setSpeed(1000);
    stepper6.runSpeed();
  }
  else {
    stepper6.stop();
  } 
  if (digitalRead(knop_tool_cw )== HIGH) {
    servo_tool.writeMicroseconds(servo_tool_pos + 1);
  }
  else if (digitalRead(knop_tool_ccw )== HIGH) {
    servo_tool.writeMicroseconds(servo_tool_pos - 1);
  }
}

int check_switches() {
  return 0;
}

void loop() {

  if (!digitalRead(knop_noodstop_kast)) {
    if (digitalRead(knop_reset)) {
      digitalWrite(enable_all_steppers, LOW); // set all motors of
      digitalWrite(power_steppers_off, LOW); //schakel relais weer in
    }
    if (digitalRead(knop_enable_all_steppers)) {
      digitalWrite(enable_all_steppers, HIGH); // set all motors of
    }
    if (softwarejoggen == 1) {
      joggen();
      //Serial.print("in joggen");
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
  else {
    digitalWrite(enable_all_steppers, HIGH); // set all motors of
    digitalWrite(power_steppers_off, HIGH); //schakel relais
  }
}
