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
//14-21

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

#define knop_enable_all_steppers 51
#define knop_noodstop_kast 36 
#define knop_stop_controller 37
#define knop_reset 38
#define knop_dodemans 39



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

#define power_steppers_off 53 

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

// global variabelen
#pragma region
//variabelen voor snelle software verandering/instelling
int softwarejoggen = 1;

// start variabels of parameters
int servo_tool_pos = 1500; //set servo to midpoint 1500 (servo between 700 and 2300 or 1000 and 2000)

// set /reset variables
bool stop_moving = 1;
bool joging = 0;
bool homed = 0;
bool homing = 0;
bool homed_axis1 = 0;
bool homed_axis2 = 0;
bool homed_axis3 = 0;
bool homed_axis4 = 0;
bool homed_axis5 = 0;
bool homed_axis6 = 0;

//status of buttons
#pragma region
bool status_stepPin1;  
bool status_dirPin1;
bool status_stepPin2;
bool status_dirPin2;
bool status_stepPin3;
bool status_dirPin3;
bool status_stepPin4;
bool status_dirPin4;
bool status_stepPin5;
bool status_dirPin5;
bool status_stepPin6;
bool status_dirPin6;

bool status_knop_axis_1_cw;
bool status_knop_axis_1_ccw;
bool status_knop_axis_2_cw;
bool status_knop_axis_2_ccw;
bool status_knop_axis_3_cw;
bool status_knop_axis_3_ccw;
bool status_knop_axis_4_cw;
bool status_knop_axis_4_ccw;
bool status_knop_axis_5_cw;
bool status_knop_axis_5_ccw;
bool status_knop_axis_6_cw;
bool status_knop_axis_6_ccw;
bool status_knop_tool_cw;
bool status_knop_tool_ccw;

bool status_knop_enable_all_steppers;
bool status_knop_noodstop_kast;
bool status_knop_stop_controller;
bool status_knop_reset;
bool status_knop_dodemans;

//status of switches
bool status_switch_axis_1;
bool status_switch_axis_2_cw;
bool status_switch_axis_2_ccw;
bool status_switch_axis_3_cw;
bool status_switch_axis_3_ccw;
bool status_switch_axis_4;
bool status_switch_axis_5_cw;
bool status_switch_axis_5_ccw;
bool status_switch_axis_6;
#pragma endregion

#pragma endregion


void setup() {
    // Set the maximum speed and acceleration:
    Serial.begin(115200);
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
    joging = 1;
    //axis 1
    if (status_knop_axis_1_cw == HIGH) {
        stepper1.move(1);
        stepper1.setSpeed(1000);
        stepper1.runSpeed();
    }
    else if (status_knop_axis_1_ccw == HIGH) {
        stepper1.move(-1);
        stepper1.setSpeed(-1000);
        stepper1.runSpeed();
    }
    else {
        stepper1.stop();
    }
    //axis 2
    if (status_knop_axis_2_cw == HIGH) {
        stepper2.move(1);
        stepper2.setSpeed(1000);
        stepper2.runSpeed();
    }
    else if (status_knop_axis_2_ccw == HIGH) {
        stepper2.move(-1);
        stepper2.setSpeed(-1000);
        stepper2.runSpeed();
    }
    else {
        stepper2.stop();
    }
    //axis 3
    if (status_knop_axis_3_cw == HIGH) {
        stepper3.move(1);
        stepper3.setSpeed(3000);
        stepper3.runSpeed();
    }
    else if (status_knop_axis_3_ccw == HIGH) {
        stepper3.move(-1);
        stepper3.setSpeed(-1000);
        stepper3.runSpeed();
    }
    else {
        stepper3.stop();
    }
    //axis 4
    if (status_knop_axis_4_cw == HIGH) {
        stepper4.move(1);
        stepper4.setSpeed(1000);
        stepper4.runSpeed();
    }
    else if (status_knop_axis_4_ccw == HIGH) {
        stepper4.move(-1);
        stepper4.setSpeed(-1000);
        stepper4.runSpeed();
    }
    else {
        stepper4.stop();
    }
    //axis 5
    if (status_knop_axis_5_cw == HIGH) {
        stepper5.move(1);
        stepper5.setSpeed(1000);
        stepper5.runSpeed();
    }
    else if (status_knop_axis_5_ccw == HIGH) {
        stepper5.move(-1);
        stepper5.setSpeed(-1000);
        stepper5.runSpeed();
    }
    else {
        stepper5.stop();
    }
    //axis 6
    if (status_knop_axis_6_cw == HIGH) {
        stepper6.move(1);
        stepper6.setSpeed(1000);
        stepper6.runSpeed();
    }
    else if (status_knop_axis_6_ccw == HIGH) {
        stepper6.move(-1);
        stepper6.setSpeed(-1000);
        stepper6.runSpeed();
    }
    else {
        stepper6.stop();
    }
    if (status_knop_tool_cw == HIGH) {
        servo_tool.writeMicroseconds(servo_tool_pos + 1);
    }
    else if (status_knop_tool_ccw == HIGH) {
        servo_tool.writeMicroseconds(servo_tool_pos - 1);
    }
}


void check_switches() {
    //Serial.print("  CHECK_SWITCHES CALLED ");
    //Serial.print(status_switch_axis_3_cw);

    if (status_switch_axis_1 && !joging) {
        stop_moving = 1;
        //Serial.print("  1  = ");
    }
    if (status_switch_axis_2_cw) {
        if (status_dirPin2 == 1) {
            stop_moving = 1;
          //  Serial.print(" 2 cw = ");
        }
    }
    if (status_switch_axis_2_ccw) {
        if (status_dirPin2 == 0) {
            stop_moving = 1;
        //    Serial.print(" 2 ccw = ");
       }
    }
    if (status_switch_axis_3_cw) {
        if (status_dirPin3 == 1) {
            stop_moving = 1;
        //    Serial.print(" 3 cw = ");
        }
    }
    if (status_switch_axis_3_ccw) {
        if (status_dirPin3 == 0) {
            stop_moving = 1;
         //   Serial.print(" 3 cww = ");
          }
    }
    if (status_switch_axis_4 && !joging) {
            stop_moving = 1;
         //   Serial.print(" 4  = ");
    }
    if (status_switch_axis_5_cw) {
        if (status_dirPin5 == 1) {
            stop_moving = 1;
         //   Serial.print(" 5 cw = ");
        }
    }
    if (status_switch_axis_5_ccw) {
        if (status_dirPin5 == 0) {
            stop_moving = 1;
       //     Serial.print("   5 cww   = ");
      }
    }
    if (status_switch_axis_6 && !joging) {
        stop_moving = 1;
       // Serial.print("   6     ");
    }
}

void read_inputs() {
    //status dirpin

    status_dirPin1 = digitalRead(dirPin1);
    status_dirPin2 = digitalRead(dirPin2);
    status_dirPin3 = digitalRead(dirPin3);
    status_dirPin4 = digitalRead(dirPin4);
    status_dirPin5 = digitalRead(dirPin5);
    status_dirPin6 = digitalRead(dirPin6);

    //status of buttons
    status_knop_axis_1_cw = digitalRead(knop_axis_1_cw);
    status_knop_axis_1_ccw = digitalRead(knop_axis_1_ccw);
    status_knop_axis_2_cw = digitalRead(knop_axis_2_cw);
    status_knop_axis_2_ccw = digitalRead(knop_axis_2_ccw);
    status_knop_axis_3_cw = digitalRead(knop_axis_3_cw);
    status_knop_axis_3_ccw = digitalRead(knop_axis_3_ccw);
    status_knop_axis_4_cw = digitalRead(knop_axis_4_cw);
    status_knop_axis_4_ccw = digitalRead(knop_axis_4_ccw);
    status_knop_axis_5_cw = digitalRead(knop_axis_5_cw);
    status_knop_axis_5_ccw = digitalRead(knop_axis_5_ccw);
    status_knop_axis_6_cw = digitalRead(knop_axis_6_cw);
    status_knop_axis_6_ccw = digitalRead(knop_axis_6_ccw);
    status_knop_tool_cw = digitalRead(knop_tool_cw);
    status_knop_tool_ccw = digitalRead(knop_tool_ccw);

    status_knop_enable_all_steppers = digitalRead(knop_enable_all_steppers);
    status_knop_noodstop_kast = digitalRead(knop_noodstop_kast);
    status_knop_stop_controller = digitalRead(knop_stop_controller);
    status_knop_reset = digitalRead(knop_reset);
    status_knop_dodemans = digitalRead(knop_dodemans);

    //status of switches
    status_switch_axis_1 = digitalRead(switch_axis_1);
    status_switch_axis_2_cw = digitalRead(switch_axis_2_cw);
    status_switch_axis_2_ccw = digitalRead(switch_axis_2_ccw);
    status_switch_axis_3_cw = digitalRead(switch_axis_3_cw);
    status_switch_axis_3_ccw = digitalRead(switch_axis_3_ccw);
    status_switch_axis_4 = digitalRead(switch_axis_4);
    status_switch_axis_5_cw = digitalRead(switch_axis_5_cw);
    status_switch_axis_5_ccw = digitalRead(switch_axis_5_ccw);
    status_switch_axis_6 = digitalRead(switch_axis_6);
}

int home_all_axis() {
    if (!homed_axis1) {
        //home
    }

    return 1;
}

void print() {
    Serial.print(" axis_1 = ");
    Serial.print(status_switch_axis_1);
    Serial.print(" axis_2_cw = ");
    Serial.print(status_switch_axis_2_cw);
    Serial.print(" axis_2_ccw = ");
    Serial.print(status_switch_axis_2_ccw);
    Serial.print(" axis_3_cw = ");
    Serial.print(status_switch_axis_3_cw);
    Serial.print(" axis_3_ccw = ");
    Serial.print(status_switch_axis_3_ccw);
    Serial.print(" axis_4 = ");
    Serial.print(status_switch_axis_4);
    Serial.print(" axis_5_cw = ");
    Serial.print(status_switch_axis_5_cw);
    Serial.print(" axis_5_ccw = ");
    Serial.print(status_switch_axis_5_ccw);
    Serial.print(" axis_6 = ");
    Serial.print(status_switch_axis_6);

    //Serial.println(stop_moving);
    Serial.println("  ");
}

void loop() {

    while (1) {
        read_inputs();
        check_switches();
        //homed = home_all_axis();
        //print();

        if (!status_knop_noodstop_kast) {
            if (status_knop_reset) {
                digitalWrite(enable_all_steppers, LOW); // set all motors of
                digitalWrite(power_steppers_off, LOW); //schakel relais weer in
                stop_moving = 0;
                Serial.println("reset");
            }
            if (status_knop_enable_all_steppers) {
                digitalWrite(enable_all_steppers, HIGH); // set all motors of
            }
            if (status_knop_stop_controller || stop_moving) {
                stepper1.stop();
                stepper2.stop();
                stepper3.stop();
                stepper4.stop();
                stepper5.stop();
                stepper6.stop();
                stop_moving = 1;
                Serial.println("knop ingedrukt");
            }

            if ((softwarejoggen == 1) && (stop_moving == 0)) {
                joggen();
            }
            else {
                joging = 0;
            }
            
            if (homed) {
                //run program
            }
        }
        else {
            digitalWrite(enable_all_steppers, HIGH); // set all motors off
            digitalWrite(power_steppers_off, HIGH); //schakel relais
        }
    }
}
