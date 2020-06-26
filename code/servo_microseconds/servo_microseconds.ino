#include <Servo.h> 

Servo myservo;

void setup() 
{ 
  myservo.attach(49);
  myservo.writeMicroseconds(700);  // set servo to mid-point
} 

void loop() {} 
