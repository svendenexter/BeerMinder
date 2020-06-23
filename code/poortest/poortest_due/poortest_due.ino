/*
  Blink

  Turns an LED on for one second, then off for one second, repeatedly.

  Most Arduinos have an on-board LED you can control. On the UNO, MEGA and ZERO
  it is attached to digital pin 13, on MKR1000 on pin 6. LED_BUILTIN is set to
  the correct LED pin independent of which board is used.
  If you want to know what pin the on-board LED is connected to on your Arduino
  model, check the Technical Specs of your board at:
  https://www.arduino.cc/en/Main/Products

  modified 8 May 2014
  by Scott Fitzgerald
  modified 2 Sep 2016
  by Arturo Guadalupi
  modified 8 Sep 2016
  by Colby Newman

  This example code is in the public domain.

  http://www.arduino.cc/en/Tutorial/Blink
*/
#include <Servo.h>
#include <MultiStepper.h>
#include <AccelStepper.h> 

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

// the setup function runs once when you press reset or power the board
void setup() {
  // initialize digital pin LED_BUILTIN as an output.
  Serial.begin(9600);
  int i = 2;
  Serial.print("while in");
  while (i < 54) {
    i++;
    Serial.print("while loop");
    pinMode( i, INPUT);

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
  Serial.print("setup compleet");
}

// the loop function runs over and over again forever
void loop() {
  //digitalWrite(LED_BUILTIN, HIGH);   // turn the LED on (HIGH is the voltage level)
  //delay(100);                       // wait for a second
  //digitalWrite(LED_BUILTIN, LOW);    // turn the LED off by making the voltage LOW
  //delay(100);          // wait for a second
  int x = 22;
  while ( x < 49) {
    Serial.print(String(x) + " = " + digitalRead(x)+ "  ");
    x ++;
    //digitalRead(53);
  }
  Serial.print("51 = " + String(digitalRead(51))+ "  ");
  Serial.println(" nieuwe regel ");

}
