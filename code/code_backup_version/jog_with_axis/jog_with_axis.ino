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
int Speed_stepper1 = 0;
int Speed_stepper2 = 0;
int Speed_stepper3 = 0;
int Speed_stepper4 = 0;
int Speed_stepper5 = 0;
int Speed_stepper6 = 0;

int Speed_max_stepper1 = 0;
int Speed_max_stepper2 = 0;
int Speed_max_stepper3 = 0;
int Speed_max_stepper4 = 0;
int Speed_max_stepper5 = 0;
int Speed_max_stepper6 = 0;

int move_stepper1 = 0;
int move_stepper2 = 0;
int move_stepper3 = 0;
int move_stepper4 = 0;
int move_stepper5 = 0;
int move_stepper6 = 0;

int move_to_stepper1 = 0;
int move_to_stepper2 = 0;
int move_to_stepper3 = 0;
int move_to_stepper4 = 0;
int move_to_stepper5 = 0;
int move_to_stepper6 = 0;

bool stop_stepper1 = 0;
bool stop_stepper2 = 0;
bool stop_stepper3 = 0;
bool stop_stepper4 = 0;
bool stop_stepper5 = 0;
bool stop_stepper6 = 0;

// set /reset variables
bool is_moving = 0;
bool use_movespeed = 1;
bool ready_for_functions_call = 1;
bool stop_moving = 1;
bool joging = 0;
bool homing = 0;

bool homed_axis1 = 0;
bool homed_axis2 = 0;
bool homed_axis3 = 0;
bool homed_axis4 = 0;
bool homed_axis5 = 0;
bool homed_axis6 = 0;

bool homed = 0;

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
    Serial.begin(250000);
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

void joggen() {
    joging = 1;
    use_movespeed = 1;
    //axis 1
    if (status_knop_axis_1_cw && status_knop_axis_1_ccw) {
        stop_stepper1 = 1;
    }
    else if (status_knop_axis_1_cw == HIGH) {
        move_stepper1 = 1;
        Speed_stepper1 = 1000;
        stop_stepper1 = 0;
    }
    else if (status_knop_axis_1_ccw == HIGH) {
        move_stepper1 = -1;
        Speed_stepper1 = -1000;
        stop_stepper1 = 0;
    }
    else {
        stop_stepper1 = 1;
    }
    //axis 2
    if (status_knop_axis_2_cw && status_knop_axis_2_ccw) {
        stop_stepper2 = 1;
    }
    else if (status_knop_axis_2_cw == HIGH) {
        move_stepper2 = 1;
        Speed_stepper2 = 1000;
        stop_stepper2 = 0;
    }
    else if (status_knop_axis_2_ccw == HIGH) {
        move_stepper2 = -1;
        Speed_stepper2 = -1000;
        stop_stepper2 = 0;
    }
    else {
        stop_stepper2 = 1;
    }
    //axis 3
    if (status_knop_axis_3_cw && status_knop_axis_3_ccw) {
        stop_stepper3 = 1;
    }
    else if (status_knop_axis_3_cw == HIGH) {
        move_stepper3 = 1;
        Speed_stepper3 = 1000;
        stop_stepper3 = 0;
    }
    else if (status_knop_axis_3_ccw == HIGH) {
        move_stepper3 = -1;
        Speed_stepper3 = -1000;
        stop_stepper3 = 0;
    }
    else {
        stop_stepper3 = 1;
    }
    //axis 4
    if (status_knop_axis_4_cw && status_knop_axis_4_ccw) {
        stop_stepper4 = 1;
    }
    else if (status_knop_axis_4_cw == HIGH) {
        move_stepper4 = 1;
        Speed_stepper4 = 1000;
        stop_stepper4 = 0;
    }
    else if (status_knop_axis_4_ccw == HIGH) {
        move_stepper4 = -1;
        Speed_stepper4 = -1000;
        stop_stepper4 = 0;
    }
    else {
        stop_stepper4 = 1;
    }
    //axis 5
    if (status_knop_axis_5_cw && status_knop_axis_5_ccw) {
        stop_stepper5 = 1;
    }
    else if (status_knop_axis_5_cw == HIGH) {
        move_stepper5 = 1;
        move_stepper6 = -1;
        Speed_stepper5 = 1000;
        Speed_stepper6 = -1000;
        stop_stepper5 = 0;
        stop_stepper6 = 0;
    }
    else if (status_knop_axis_5_ccw == HIGH) {
        move_stepper5 = -1;
        move_stepper6 = 1;
        Speed_stepper5 = -1000;
        Speed_stepper6 = 1000;
        stop_stepper5 = 0;
        stop_stepper6 = 0;
    }
    //axis 6
    if (status_knop_axis_6_cw && status_knop_axis_6_ccw) {
        stop_stepper6 = 1;
    }
    else if (status_knop_axis_6_cw == HIGH) {
        move_stepper5 = 1;
        move_stepper6 = 1;
        Speed_stepper5 = 1000;
        Speed_stepper6 = 1000;
        stop_stepper5 = 0;
        stop_stepper6 = 0;
    }
    else if (status_knop_axis_6_ccw == HIGH) {
        move_stepper5 = -1;
        move_stepper6 = -1;
        Speed_stepper5 = -1000;
        Speed_stepper6 = -1000;
        stop_stepper5 = 0;
        stop_stepper6 = 0;
    }
    //stop stepper 5 en 6 als knop niet is ingedrukt
    if (!status_knop_axis_5_ccw && !status_knop_axis_5_cw && !status_knop_axis_6_ccw && !status_knop_axis_6_cw) {
        stop_stepper5 = 1;
        stop_stepper6 = 1;
    }

    if (status_knop_tool_cw == HIGH) {
        servo_tool.writeMicroseconds(servo_tool_pos + 1);
    }
    else if (status_knop_tool_ccw == HIGH) {
        servo_tool.writeMicroseconds(servo_tool_pos - 1);
    }

    //when nothing pressed set ready for function call
    if (status_knop_axis_1_ccw || status_knop_axis_1_cw || status_knop_axis_2_ccw || status_knop_axis_2_cw || status_knop_axis_3_ccw || status_knop_axis_3_cw || status_knop_axis_4_ccw || status_knop_axis_4_cw || status_knop_axis_5_ccw || status_knop_axis_5_cw || status_knop_axis_6_ccw || status_knop_axis_6_cw || status_knop_tool_ccw || status_knop_tool_cw) {
        ready_for_functions_call = 0;
    }
    else {
        ready_for_functions_call = 1;
    }
}

int home_all_axis() {
    if (!homed_axis1) {
        //home
    }

    return 1;
}

void control_stepper() {
    if (stop_moving == HIGH) {
        stepper1.stop();
        stepper2.stop();
        stepper3.stop();
        stepper4.stop();
        stepper5.stop();
        stepper6.stop();
        Serial.println(" all motors are stopped ");
    }
    else {
        if (stop_stepper1 == HIGH) {
            stepper1.stop();
        }
        else if (use_movespeed == HIGH) {
            stepper1.setSpeed(Speed_stepper1);
            stepper1.move(move_stepper1);
            stepper1.runSpeed();
            Serial.println(" stepper 1 use move  ");
        }
        else {
            stepper1.setMaxSpeed(Speed_max_stepper1);
            stepper1.moveTo(move_to_stepper1);
            stepper1.run();
            Serial.println(" stepper 1 use move to ");
        }
        if (stop_stepper2 == HIGH) {
            stepper2.stop();
        }
        else if (use_movespeed == HIGH) {
            stepper2.setSpeed(Speed_stepper2);
            stepper2.move(move_stepper2);
            stepper2.runSpeed();            
        }
        else {
            stepper2.setMaxSpeed(Speed_max_stepper2);
            stepper2.moveTo(move_to_stepper2);
            stepper2.run();
        }
        if (stop_stepper3 == HIGH) {
            stepper3.stop();
        }
        else if (use_movespeed == HIGH) {
            stepper3.setSpeed(Speed_stepper3);
            stepper3.move(move_stepper3);
            stepper3.runSpeed();
        }
        else {
            stepper3.setMaxSpeed(Speed_max_stepper3);
            stepper3.moveTo(move_to_stepper3);
            stepper3.run();
        }
        if (stop_stepper4 == HIGH) {
            stepper4.stop();
        }
        else if (use_movespeed == HIGH) {
            stepper4.setSpeed(Speed_stepper4);
            stepper4.move(move_stepper4);
            stepper4.runSpeed();
        }
        else {
            stepper4.setMaxSpeed(Speed_max_stepper4);
            stepper4.moveTo(move_to_stepper4);
            stepper4.run();
        }
        if (stop_stepper5 == HIGH) {
            stepper5.stop();
        }
        else if (use_movespeed == HIGH) {
            stepper5.setSpeed(Speed_stepper5);
            stepper5.move(move_stepper5);
            stepper5.runSpeed();
        }
        else {
            stepper5.setMaxSpeed(Speed_max_stepper5);
            stepper5.moveTo(move_to_stepper5);
            stepper5.run();
        }
        if (stop_stepper6 == HIGH) {
            stepper6.stop();
        }
        else if (use_movespeed == HIGH) {
            stepper6.setSpeed(Speed_stepper6);
            stepper6.move(move_stepper6);
            stepper6.runSpeed();
        }
        else {
            stepper6.setMaxSpeed(Speed_max_stepper6);
            stepper6.moveTo(move_to_stepper6);
            stepper6.run();
        }
        //endeffector

        move_stepper1 = 0;
        move_stepper2 = 0;
        move_stepper3 = 0;
        move_stepper4 = 0;
        move_stepper5 = 0;
        move_stepper6 = 0;

        Speed_stepper1 = 0;
        Speed_stepper2 = 0;
        Speed_stepper3 = 0;
        Speed_stepper4 = 0;
        Speed_stepper5 = 0;
        Speed_stepper6 = 0;
        
    }
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
        control_stepper();

        //homed = home_all_axis();
        //print();
        

        if (!status_knop_noodstop_kast) {
            if (status_knop_reset) {
                digitalWrite(enable_all_steppers, LOW); // set all motors on
                digitalWrite(power_steppers_off, LOW); //schakel relais weer in
                stop_moving = 0;
                Serial.println("reset");
            }
            if (!status_knop_enable_all_steppers) {
                Serial.println(" motors are disabled ");
                digitalWrite(enable_all_steppers, HIGH); // set all motors of
            }
            if (status_knop_stop_controller || stop_moving) {
                stop_moving = 1;
                Serial.println(" stopped ");
            }

            if ((softwarejoggen == 1) && (stop_moving == 0) && (ready_for_functions_call == 1 || joging == 1) ) {
                Serial.println(" jog function called ");
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
