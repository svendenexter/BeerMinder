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

// data poorten 14 - 21
#define rx_pin 0
#define tx_pin 1

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

Servo servo_tool;  // pulse tussen 500 en 2500

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

//LCD scherm
#pragma region

#pragma endregion

// global variabeles
#pragma region
//variabelen voor snelle software verandering/instelling for testing
int softwarejoggen = 0;
int softwarehomen = 1;
int softwarerunprogram = 0;

#pragma endregion

//variables
#pragma region

//LCD touchscreen variabels
int currentpage = 0;

//timer zooi
int period1000 = 1000;
int period1000_case10home = 1000;
int period2000 = 2000;
int period3000 = 3000;
unsigned long time_now = 0;

// set /reset variables

int homeorder = 1;
int program_order = 1;
bool is_moving = 0;
bool use_movespeed = 1;
bool use_moveto_withspeed = 0;
bool ready_for_functions_call = 1;
bool stop_moving = 1;
bool joging = 0;
bool homing = 0;
bool printing_positions = 0;
bool running_program = 0;

bool homed_axis1 = 0;
bool homed_axis2 = 0;
bool homed_axis3 = 0;
bool homed_axis4 = 0;
bool homed_axis5 = 0;
bool homed_axis6 = 0;

bool homed = 0;

// start variabels of parameters
bool move_all_to = 0;
long move_steppers[] = { 0, 0, 0, 0, 0, 0 };

int servo_tool_pos = 1500; //// pulse tussen 500 en 2500
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

int move_to_stepper1_countdown = 0;
int move_to_stepper2_countdown = 0;
int move_to_stepper3_countdown = 0;
int move_to_stepper4_countdown = 0;
int move_to_stepper5_countdown = 0;
int move_to_stepper6_countdown = 0;

bool stop_stepper1 = 0;
bool stop_stepper2 = 0;
bool stop_stepper3 = 0;
bool stop_stepper4 = 0;
bool stop_stepper5 = 0;
bool stop_stepper6 = 0;



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
//counter to fix EMC from steppers for the switches

int counter_switch_axis_1 = 0;
int counter_switch_axis_2_cw = 0;
int counter_switch_axis_2_ccw = 0;
int counter_switch_axis_3_cw = 0;
int counter_switch_axis_3_ccw = 0;
int counter_switch_axis_4 = 0;
int counter_switch_axis_5_cw = 0;
int counter_switch_axis_5_ccw = 0;
int counter_switch_axis_6 = 0;

#pragma endregion

#pragma endregion


void setup() {
    // Set the maximum speed and acceleration:
    //Serial.begin(250000);
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
    steppers.addStepper(stepper3);
    steppers.addStepper(stepper4);
    steppers.addStepper(stepper5);
    steppers.addStepper(stepper6);

    //servo_tool.attach(tool_servo_pin);
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
   /*
    status_switch_axis_1 = digitalRead(switch_axis_1);
    status_switch_axis_2_cw = digitalRead(switch_axis_2_cw);
    status_switch_axis_2_ccw = digitalRead(switch_axis_2_ccw);
    status_switch_axis_3_cw = digitalRead(switch_axis_3_cw);
    status_switch_axis_3_ccw = digitalRead(switch_axis_3_ccw);
    status_switch_axis_4 = digitalRead(switch_axis_4);
    status_switch_axis_5_cw = digitalRead(switch_axis_5_cw);
    status_switch_axis_5_ccw = digitalRead(switch_axis_5_ccw);
    status_switch_axis_6 = digitalRead(switch_axis_6);
    */

    //axis 1
    if (digitalRead(switch_axis_1)) {
        counter_switch_axis_1++;
    }
    else {
        counter_switch_axis_1 = 0;
        status_switch_axis_1 = 0;
    }
    if (counter_switch_axis_1 >= 3) {
        status_switch_axis_1 = 1;
    }
    //axis_2_cw
    if (digitalRead(switch_axis_2_cw)) {
        counter_switch_axis_2_cw++;
    }
    else {
        counter_switch_axis_2_cw = 0;
        status_switch_axis_2_cw = 0;
    }
    if (counter_switch_axis_2_cw >= 3) {
        status_switch_axis_2_cw = 1;
    }
    //axis_2_ccw
    if (digitalRead(switch_axis_2_ccw)) {
        counter_switch_axis_2_ccw++;
    }
    else {
        counter_switch_axis_2_ccw = 0;
        status_switch_axis_2_ccw = 0;
    }
    if (counter_switch_axis_2_ccw >= 3) {
        status_switch_axis_2_ccw = 1;
    }
    //axis_3_cw
    if (digitalRead(switch_axis_3_cw)) {
        counter_switch_axis_3_cw++;
    }
    else {
        counter_switch_axis_3_cw = 0;
        status_switch_axis_3_cw = 0;
    }
    if (counter_switch_axis_3_cw >= 3) {
        status_switch_axis_3_cw = 1;
    }
    //axis_3_ccw
    if (digitalRead(switch_axis_3_ccw)) {
        counter_switch_axis_3_ccw++;
    }
    else {
        counter_switch_axis_3_ccw = 0;
        status_switch_axis_3_ccw = 0;
    }
    if (counter_switch_axis_3_ccw >= 3) {
        status_switch_axis_3_ccw = 1;
    }
    //axis 4
    if (digitalRead(switch_axis_4)) {
        counter_switch_axis_4++;
    }
    else {
        counter_switch_axis_4 = 0;
        status_switch_axis_4 = 0;
    }
    if (counter_switch_axis_4 >= 3) {
        status_switch_axis_4 = 1;
    }
    //axis_5_cw
    if (digitalRead(switch_axis_5_cw)) {
        counter_switch_axis_5_cw++;
    }
    else {
        counter_switch_axis_5_cw = 0;
        status_switch_axis_5_cw = 0;
    }
    if (counter_switch_axis_5_cw >= 3) {
        status_switch_axis_5_cw = 1;
    }
    //axis_5_ccw
    if (digitalRead(switch_axis_5_ccw)) {
        counter_switch_axis_5_ccw++;
    }
    else {
        counter_switch_axis_5_ccw = 0;
        status_switch_axis_5_ccw = 0;
    }
    if (counter_switch_axis_5_ccw >= 3) {
        status_switch_axis_5_ccw = 1;
    }
    //axis 6
    if (digitalRead(switch_axis_6)) {
        counter_switch_axis_6++;
    }
    else {
        counter_switch_axis_6 = 0;
        status_switch_axis_6 = 0;
    }
    if (counter_switch_axis_6 >= 3) {
        status_switch_axis_6 = 1;
    }

}

void check_switches_for_move() {
    if (use_movespeed) {
        //axis2
        if (status_switch_axis_2_cw) {
            if (move_stepper2 > 0) {
                move_stepper2 = -1;
            }
            if (Speed_stepper2 > 0) {
                Speed_stepper2 = -1000;
            }
        }
        if (status_switch_axis_2_ccw) {
            if (move_stepper2 < 0) {
                move_stepper2 = 1;
            }
            if (Speed_stepper2 < 0) {
                Speed_stepper2 = 1000;
            }
        }
        //axis3
        if (status_switch_axis_3_cw) {
            if (move_stepper3 > 0) {
                move_stepper3 = -1;
            }
            if (Speed_stepper3 > 0) {
                Speed_stepper3 = -1000;
            }
        }
        if (status_switch_axis_3_ccw) {
            if (move_stepper3 < 0) {
                move_stepper3 = 1;
            }
            if (Speed_stepper3 < 0) {
                Speed_stepper3 = 1000;
            }
        }
        //axis5
        if (status_switch_axis_5_cw) {
            if (move_stepper5 > 0 && move_stepper6 < 0) {
                move_stepper5 = -1;
                move_stepper6 = 1;
            }
            if (Speed_stepper5 > 0 && Speed_stepper6 < 0) {
                Speed_stepper5 = -1000;
                Speed_stepper6 = 1000;
            }
        }
        if (status_switch_axis_5_ccw) {
            if (move_stepper5 < 0 && move_stepper6 > 0) {
                move_stepper5 = 1;
                move_stepper6 = -1;
            }
            if (Speed_stepper5 < 0 && Speed_stepper6 > 0) {
                Speed_stepper5 = 1000;
                Speed_stepper6 = -1000;
            }
        }
    }
    else if (!homing) {
        if (status_switch_axis_2_cw || status_switch_axis_2_ccw) {
            stop_stepper2;
        }
        if (status_switch_axis_3_cw || status_switch_axis_3_ccw) {
            stop_stepper3;
        }

        if (status_switch_axis_5_cw || status_switch_axis_5_ccw) {
            stop_stepper6;
            stop_stepper5;
        }
    }
}

void joggen() {
    joging = 1;
    use_movespeed = 1;
    use_moveto_withspeed = 0;
    //axis 1
    if (status_knop_axis_1_cw && status_knop_axis_1_ccw) {
        stop_stepper1 = 1;
    }
    else if (status_knop_axis_1_cw == HIGH) {
        move_stepper1 = 1;
        Speed_stepper1 = 10000;
        stop_stepper1 = 0;
    }
    else if (status_knop_axis_1_ccw == HIGH) {
        move_stepper1 = -1;
        Speed_stepper1 = -10000;
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
        Speed_stepper3 = 2000;
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
        servo_tool.attach(tool_servo_pin);
        servo_tool.writeMicroseconds(550);
        //servo_tool_pos++;
    }
    else if (status_knop_tool_ccw == HIGH) {
        servo_tool.attach(tool_servo_pin);
        servo_tool.writeMicroseconds(1900);
        //servo_tool_pos--;        
    }
    else if (status_knop_dodemans == 1){
        servo_tool.detach();
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
    printPosition();
    homing = 1;
    ready_for_functions_call = 0;

    switch (homeorder) {
    case 0:
        if (!homed) {
            homeorder = 1;

        }
        break;
    case 1:
       // servo_tool.attach(tool_servo_pin);
      //  servo_tool.writeMicroseconds(1500);
        static unsigned long time2 = millis();
        if (millis() >= time2 + 2000) {
            homeorder = 5;
         //   servo_tool.detach();
        }
        break;
    case 5: //axis 6
        
        if (status_switch_axis_6 == HIGH) {
            homed_axis6 = 1;
            stepper5.setCurrentPosition(0); // een nulpunt om beter daarna beter 0 nulpunt te setten
            stepper6.setCurrentPosition(0);
            homeorder = 7;
        }
        else {
            use_movespeed = 1;
            use_moveto_withspeed = 0;
            move_stepper5 = 1;
            move_stepper6 = 1;
            Speed_stepper5 = 1000;
            Speed_stepper6 = 1000;
            stop_stepper5 = 0;
            stop_stepper6 = 0;
            homed_axis6 = 0;
        }
        break;
    case 7: //axis  ga even iets terug
        use_movespeed = 1;
        use_moveto_withspeed = 1;
        move_to_stepper5 = -1000; // iets terugdraaien na het nullen om opnieuw te homen
        move_to_stepper6 = -1000;
        Speed_stepper5 = -1000;
        Speed_stepper6 = -1000;
        stop_stepper5 = 0;
        stop_stepper6 = 0;
        homed_axis6 = 0;
        if ( stepper5.currentPosition() == move_to_stepper5 && 
            stepper6.currentPosition() == move_to_stepper6) {

            Serial.println(" position away from switch 6 reached ");
            homeorder = 8;
        }
        break;
    case 8: //axis 6
        if (status_switch_axis_6 == HIGH) {
            homed_axis6 = 1;
            stepper5.setCurrentPosition(0); // HET ECHTE NULPUNT AXIS 6 (later wel reset door axis 5
            stepper6.setCurrentPosition(0);
            homeorder = 10;  
        }
        else {
            use_movespeed = 1;
            use_moveto_withspeed = 0;
            move_stepper5 = 1;
            move_stepper6 = 1;
            Speed_stepper5 = 100;
            Speed_stepper6 = 100;
            stop_stepper5 = 0;
            stop_stepper6 = 0;
            homed_axis6 = 0;
        }
        break;
    case 10: //axis 5
        if (status_switch_axis_5_cw == HIGH) {
            homed_axis5 = 1;
            stepper5.setCurrentPosition(0);
            stepper6.setCurrentPosition(0);
            homeorder = 12;
        }
        else {
            use_movespeed = 1;
            use_moveto_withspeed = 0;
            move_stepper5 = 1;
            move_stepper6 = -1;
            Speed_stepper5 = 1000;
            Speed_stepper6 = -1000;
            stop_stepper5 = 0;
            stop_stepper6 = 0;
            homed_axis5 = 0;
        }
        break;


    case 12: //axis  5  ga even iets terug
        use_movespeed = 1;
        use_moveto_withspeed = 1;
        move_to_stepper5 = -750; // iets terugdraaien na het nullen om opnieuw te homen
        move_to_stepper6 = 750;
        Speed_stepper5 = -1000;
        Speed_stepper6 = 1000;
        stop_stepper5 = 0;
        stop_stepper6 = 0;
        homed_axis5 = 0;
        if (stepper5.currentPosition() == move_to_stepper5 &&
            stepper6.currentPosition() == move_to_stepper6) {

            Serial.println(" position away from switch 5 reached ");
            homeorder = 13;
        }
        break;
    case 13: //axis 5   SET ZERO point stepper 5 and 6
        if (status_switch_axis_5_cw == HIGH) {
            homed_axis5 = 1;
            stepper5.setCurrentPosition(0); // HET ECHTE NULPUNT stepper 5 
            stepper6.setCurrentPosition(0); // HET ECHTE NULPUNT stepper 5
            homeorder = 15;
        }
        else {
            use_movespeed = 1;
            use_moveto_withspeed = 0;
            move_stepper5 = 1;
            move_stepper6 = -1;
            Speed_stepper5 = 100;
            Speed_stepper6 = -100;
            stop_stepper5 = 0;
            stop_stepper6 = 0;
            homed_axis5 = 0;
        }
        break;

    case 15: //axis 4 first zero point
        if (status_switch_axis_4 == HIGH) {
            homed_axis4 = 1;
            stepper4.setCurrentPosition(0);
            homeorder = 17;
        }
        else {
            //Serial.println("in else");
            use_movespeed = 1;
            use_moveto_withspeed = 0;
            move_stepper4 = 0;
            Speed_stepper4 = 1000;
            stop_stepper4 = 0;
            homed_axis4 = 0;
        }
        break;
    case 17: //axis 4 ga even iets terug
        use_movespeed = 1;
        use_moveto_withspeed = 1;
        move_to_stepper4 = -750; // iets terugdraaien na het nullen om opnieuw te homen
        Speed_stepper4 = -1000;
        stop_stepper4 = 0;
        homed_axis4 = 0;
        if (stepper4.currentPosition() == move_to_stepper4 ) {
            Serial.println(" position away from switch 4 reached ");
            homeorder = 18;
        }
        break;

    case 18: //axis 4 zeropoint
        if (status_switch_axis_4 == HIGH) {
            homed_axis4 = 1;
            stepper4.setCurrentPosition(0); // HET ECHTE NULPUNT AXIS 6 (later wel reset door axis 5
            homeorder = 20;
        }
        else {
            use_movespeed = 1;
            use_moveto_withspeed = 0;
            move_stepper4 = 1;
            Speed_stepper4 = 100;
            stop_stepper4 = 0;
            homed_axis4 = 0;
        }
        break;

    case 20: //axis 3  move CW 
        if (!homed_axis3) {
            if (status_switch_axis_3_cw == HIGH) {
                homed_axis3 = 1;
                stepper3.setCurrentPosition(0);
                homeorder = 22;
            }
            else {
                use_movespeed = 1;
                use_moveto_withspeed = 0;
                move_stepper3 = 1;
                Speed_stepper3 = 4000;
                stop_stepper3 = 0;
                homed_axis3 = 0;
            }
        }
        break;

    case 22: //axis 3 ga even iets terug
        use_movespeed = 1;
        use_moveto_withspeed = 1;
        move_to_stepper3 = -2000; // iets terugdraaien na het nullen om opnieuw te homen
        Speed_stepper3 = -1500;
        stop_stepper3 = 0;
        homed_axis3 = 0;
        if (stepper3.currentPosition() == move_to_stepper3) {
            Serial.println(" position away from switch 3 reached ");
            homeorder = 23;
        }
        break;
    case 23: //axis 3 zeropoint
        if (status_switch_axis_3_cw == HIGH) {
            homed_axis3 = 1;
            stepper3.setCurrentPosition(0); // echt nulpunt axis 3
            homeorder = 25;
        }
        else {
            use_movespeed = 1;
            use_moveto_withspeed = 0;
            move_stepper3 = 1;
            Speed_stepper3 = 100;
            stop_stepper3 = 0;
            homed_axis3 = 0;
        }
        break;

    case 25: //axis 2 first homepoint
        if (!homed_axis2) {
            if (status_switch_axis_2_ccw == HIGH) {
                homed_axis2 = 1;
                stepper2.setCurrentPosition(0);
                homeorder = 27;
            }
            else {
                use_movespeed = 1;
                use_moveto_withspeed = 0;
                move_stepper2 = -1;
                Speed_stepper2 = -500;
                stop_stepper2 = 0;
                homed_axis2 = 0;
            }
        }
        break;

    case 27: //axis 2 ga even iets terug
        use_movespeed = 1;
        use_moveto_withspeed = 1;
        move_to_stepper2 = 750; // iets terugdraaien na het nullen om opnieuw te homen
        Speed_stepper2 = 1000;
        stop_stepper2 = 0;
        homed_axis2 = 0;
        if (stepper2.currentPosition() == move_to_stepper2) {
            Serial.println(" position away from switch 2 reached ");
            homeorder = 28;
        }
        break;

    case 28: //axis 2 zeropoint
        if (status_switch_axis_2_ccw == HIGH) {
            homed_axis2 = 1;
            stepper2.setCurrentPosition(0); //echt nulpunt axis 2
            homeorder = 30;
        }
        else {
            use_movespeed = 1;
            use_moveto_withspeed = 0;
            move_stepper2 = -1;
            Speed_stepper2 = -100;
            stop_stepper2 = 0;
            homed_axis2 = 0;
        }
        break;

    case 30: //axis 1
        if (!homed_axis1) {
            if (status_switch_axis_1 == HIGH) {
                homed_axis1 = 0;
                stepper1.setCurrentPosition(0);
                homeorder = 32;
            }
            else {
                use_movespeed = 1;
                use_moveto_withspeed = 0;
                move_stepper1 = 1;
                Speed_stepper1 = 1000;
                stop_stepper1 = 0;
                homed_axis1 = 0;
            }
        }
        break;
    case 32: //axis 1 ga even iets terug
        use_movespeed = 1;
        use_moveto_withspeed = 1;
        move_to_stepper1 = -2000; // iets terugdraaien na het nullen om opnieuw te homen
        Speed_stepper1 = -1500;
        stop_stepper1 = 0;
        homed_axis1 = 0;
        if (stepper1.currentPosition() == move_to_stepper1) {
            Serial.println(" position away from switch 1 reached ");
            homeorder = 33;
        }
        break;

    case 33: //axis 1 zeropoint
        if (status_switch_axis_1 == HIGH) {
            homed_axis1 = 1;
            stepper1.setCurrentPosition(0); //echt nulpunt axis 1
            homeorder = 40;
        }
        else {
            use_movespeed = 1;
            use_moveto_withspeed = 0;
            move_stepper1 = 1;
            Speed_stepper1 = 100;
            stop_stepper1 = 0;
            homed_axis1 = 0;
        }
        break;


    case 40: //if not all switches are pressed run home all axis again
        if (!(homed_axis1 && homed_axis2 && homed_axis3 && homed_axis4 && homed_axis5 && homed_axis6)) {
            homeorder = 1;
        }  // end of switchs
    }
    if (homed_axis1 && homed_axis2 && homed_axis3 && homed_axis4 && homed_axis5 && homed_axis6) {
        ready_for_functions_call = 1;
        homeorder = 0;
        return 1;
    }
    else {
        return 0;
    }
}

void control_stepper() {
    check_switches_for_move();
    if (stop_moving == HIGH) {
        stepper1.stop();
        stepper2.stop();
        stepper3.stop();
        stepper4.stop();
        stepper5.stop();
        stepper6.stop();
    }
    else if (move_all_to == 0) {
        if (stop_stepper1 == HIGH) {
            stepper1.stop();
        }
        else if (use_movespeed == HIGH && use_moveto_withspeed == HIGH) {
            stepper1.moveTo(move_to_stepper1);
            stepper1.setSpeed(Speed_stepper1);
            stepper1.runSpeed();
        }
        else if (use_movespeed == HIGH) {
            stepper1.move(move_stepper1);
            stepper1.setSpeed(Speed_stepper1);
            stepper1.runSpeed();
        }
        else {
            stepper1.moveTo(move_to_stepper1);
            stepper1.setMaxSpeed(Speed_max_stepper1);
            stepper1.run();
        }
        if (stop_stepper2 == HIGH) {
            stepper2.stop();
        }
        else if (use_movespeed == HIGH && use_moveto_withspeed == HIGH) {
            stepper2.moveTo(move_to_stepper2);
            stepper2.setSpeed(Speed_stepper2);
            stepper2.runSpeed();
        }
        else if (use_movespeed == HIGH) {
            stepper2.move(move_stepper2);
            stepper2.setSpeed(Speed_stepper2);
            stepper2.runSpeed();
        }
        else {
            stepper2.moveTo(move_to_stepper2);
            stepper2.setMaxSpeed(Speed_max_stepper2);
            stepper2.run();
        }
        if (stop_stepper3 == HIGH) {
            stepper3.stop();
        }
        else if (use_movespeed == HIGH && use_moveto_withspeed == HIGH) {
            stepper3.moveTo(move_to_stepper3);
            stepper3.setSpeed(Speed_stepper3);
            stepper3.runSpeed();
        }
        else if (use_movespeed == HIGH) {
            stepper3.move(move_stepper3);
            stepper3.setSpeed(Speed_stepper3);
            stepper3.runSpeed();
        }
        else {
            stepper3.moveTo(move_to_stepper3);
            stepper3.setMaxSpeed(Speed_max_stepper3);
            stepper3.run();
        }
        if (stop_stepper4 == HIGH) {
            stepper4.stop();
        }
        else if (use_movespeed == HIGH && use_moveto_withspeed == HIGH) {
            stepper4.moveTo(move_to_stepper4);
            stepper4.setSpeed(Speed_stepper4);
            stepper4.runSpeed();
        }
        else if (use_movespeed == HIGH) {
            stepper4.move(move_stepper4);
            stepper4.setSpeed(Speed_stepper4);
            stepper4.runSpeed();
        }
        else {
            stepper4.moveTo(move_to_stepper4);
            stepper4.setMaxSpeed(Speed_max_stepper4);
            stepper4.run();
        }
        if (stop_stepper5 == HIGH) {
            stepper5.stop();
        }
        else if (use_movespeed == HIGH && use_moveto_withspeed == HIGH) {
            stepper5.moveTo(move_to_stepper5);
            stepper5.setSpeed(Speed_stepper5);
            stepper5.runSpeed();
        }
        else if (use_movespeed == HIGH) {
            stepper5.move(move_stepper5);
            stepper5.setSpeed(Speed_stepper5);
            stepper5.runSpeed();
        }
        else {
            stepper5.moveTo(move_to_stepper5);
            stepper5.setMaxSpeed(Speed_max_stepper5);
            stepper5.run();
        }
        if (stop_stepper6 == HIGH) {
            stepper6.stop();
        }
        else if (use_movespeed == HIGH && use_moveto_withspeed == HIGH) {
            stepper6.moveTo(move_to_stepper6);
            stepper6.setSpeed(Speed_stepper6);
            stepper6.runSpeed();
        }
        else if (use_movespeed == HIGH) {
            stepper6.move(move_stepper6);
            stepper6.setSpeed(Speed_stepper6);
            stepper6.runSpeed();
        }
        else {
            stepper6.moveTo(move_to_stepper6);
            stepper6.setMaxSpeed(Speed_max_stepper6);
            stepper6.run();
        }
    }
    /*
    else if (move_all_to == 1) {
        //beetje dubbel in code voor multistepper
        if (stop_stepper1 == HIGH || stop_stepper2 == HIGH || stop_stepper3 == HIGH || stop_stepper4 == HIGH || stop_stepper5 == HIGH || stop_stepper6 == HIGH) {
            stepper1.stop();
            stepper2.stop();
            stepper3.stop();
            stepper4.stop();
            stepper5.stop();
            stepper6.stop();
            move_all_to = 0;
        }
        else {
            move_all_to = 0;
            move_steppers[0] = move_to_stepper1;
            move_steppers[1] = move_to_stepper2;
            move_steppers[2] = move_to_stepper3;
            move_steppers[3] = move_to_stepper4;
            move_steppers[4] = move_to_stepper5;
            move_steppers[5] = move_to_stepper6;

            steppers.moveTo(move_steppers);
            steppers.run();
        }
    }
    */
    //servo_tool.writeMicroseconds(servo_tool_pos);

    //endeffector
    move_all_to = 0;

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

void printPosition() {
    joggen();
    static int last_var1_cw = 999;
    static int last_var1_ccw = 999;
    static int last_var2_cw = 999;
    static int last_var2_ccw = 999;
    static int last_var3_cw = 999;
    static int last_var3_ccw = 999;
    static int last_var4_cw = 999;
    static int last_var4_ccw = 999;
    static int last_var5_cw = 999;
    static int last_var5_ccw = 999;
    static int last_var6_cw = 999;
    static int last_var6_ccw = 999;

    static int In_function = 999;

    //static int lastprintposition = 999;
    bool printposition = status_knop_reset; //veranderen naar scherm input misschien ?

    // print inside function
    if (1 != In_function || printposition == 1) {
        Serial.print("In_function  ");
        Serial.println(" ");
        In_function = 1;
    }

    //axis 1
    if (last_var1_cw != status_knop_axis_1_cw || printposition == 1) {
        Serial.print("move_to_stepper1 = ");
        Serial.print(stepper1.currentPosition());
        Serial.println(" ");
    }
    last_var1_cw = status_knop_axis_1_cw;
    if (last_var1_ccw != status_knop_axis_1_ccw || printposition == 1) {
        Serial.print("move_to_stepper1 = ");
        Serial.print(stepper1.currentPosition());
        Serial.println(" ");
    }
    last_var1_ccw = status_knop_axis_1_ccw;
    //axis 2
    if (last_var2_cw != status_knop_axis_2_cw || printposition == 1) {
        Serial.print("move_to_stepper2 = ");
        Serial.print(stepper2.currentPosition());
        Serial.println(" ");
    }
    last_var2_cw = status_knop_axis_2_cw;
    if (last_var2_ccw != status_knop_axis_2_ccw || printposition == 1) {
        Serial.print("move_to_stepper2 = ");
        Serial.print(stepper2.currentPosition());
        Serial.println(" ");
    }
    last_var2_ccw = status_knop_axis_2_ccw;
    //axis 3
    if (last_var3_cw != status_knop_axis_3_cw || printposition == 1) {
        Serial.print("move_to_stepper3 = ");
        Serial.print(stepper3.currentPosition());
        Serial.println(" ");
    }
    last_var3_cw = status_knop_axis_3_cw;
    if (last_var3_ccw != status_knop_axis_3_ccw || printposition == 1) {
        Serial.print("move_to_stepper3 = ");
        Serial.print(stepper3.currentPosition());
        Serial.println(" ");
    }
    last_var3_ccw = status_knop_axis_3_ccw;
    //axis 4
    if (last_var4_cw != status_knop_axis_4_cw || printposition == 1) {
        Serial.print("move_to_stepper4 = ");
        Serial.print(stepper4.currentPosition());
        Serial.println(" ");
    }
    last_var4_cw = status_knop_axis_4_cw;
    if (last_var4_ccw != status_knop_axis_4_ccw || printposition == 1) {
        Serial.print("move_to_stepper4 = ");
        Serial.print(stepper4.currentPosition());
        Serial.println(" ");
    }
    last_var4_ccw = status_knop_axis_4_ccw;
    //axis 5
    if (last_var5_cw != status_knop_axis_5_cw || printposition == 1) {
        Serial.print("move_to_stepper5 = ");
        Serial.print(stepper5.currentPosition());
        Serial.println(" ");
    }
    last_var5_cw = status_knop_axis_5_cw;
    if (last_var5_ccw != status_knop_axis_5_ccw || printposition == 1) {
        Serial.print("move_to_stepper5 = ");
        Serial.print(stepper5.currentPosition());
        Serial.println(" ");
    }
    last_var5_ccw = status_knop_axis_5_ccw;
    //axis 6
    if (last_var6_cw != status_knop_axis_6_cw || printposition == 1) {
        Serial.print("move_to_stepper6 = ");
        Serial.print(stepper6.currentPosition());
        Serial.println(" ");
    }
    last_var6_cw = status_knop_axis_6_cw;
    if (last_var6_ccw != status_knop_axis_6_ccw || printposition == 1) {
        Serial.print("move_to_stepper6 = ");
        Serial.print(stepper6.currentPosition());
        Serial.println(" ");
    }
    last_var6_ccw = status_knop_axis_6_ccw;

    //  lastprintposition = printposition;
}

void print() {
    static int last_stopvar = 999;


    static int last_var1 = 999;
    if (last_var1 != homeorder || last_stopvar != stop_moving) {
        Serial.print(" homeorder  = ");
        Serial.print(homeorder);
        Serial.println(" ");
    }
    last_var1 = homeorder;

    static int last_var2 = 999;
    if (last_var2 != homing || last_stopvar != stop_moving) {
        Serial.print(" homing  = ");
        Serial.print(homing);
        Serial.println(" ");
    }
    last_var2 = homing;

    static int last_var5 = 999;
    if (last_var5 != homed || last_stopvar != stop_moving) {
        Serial.print(" homed  = ");
        Serial.print(homed);
        Serial.println(" ");
    }
    last_var5 = homed;

    static int last_var3 = 999;
    if (last_var3 != joging || last_stopvar != stop_moving) {
        Serial.print(" joging  = ");
        Serial.print(joging);
        Serial.println(" ");
    }
    last_var3 = joging;

    static int last_var4 = 999;
    if (last_var4 != printing_positions || last_stopvar != stop_moving) {
        Serial.print(" printing_positions  = ");
        Serial.print(printing_positions);
        Serial.println(" ");
    }
    last_var4 = printing_positions;

    //var 5 gebruikt na 2 gezet

    static int last_var6 = 999;
    if (last_var6 != running_program || last_stopvar != stop_moving) {
        Serial.print(" running_program  = ");
        Serial.print(running_program);
        Serial.println(" ");
    }
    last_var6 = running_program;

    static int last_var7 = 999;
    if (last_var7 != program_order || last_stopvar != stop_moving) {
        Serial.print(" program_order  = ");
        Serial.print(program_order);
        Serial.println(" ");
    }
    last_var7 = program_order;

    static int last_var8 = 999;
    if (last_var8 != use_moveto_withspeed || last_stopvar != stop_moving) {
        Serial.print(" use_moveto_withspeed  = ");
        Serial.print(use_moveto_withspeed);
        Serial.println(" ");
    }
    last_var8 = use_moveto_withspeed;

    static int last_var9 = 999;
    if (last_var9 != use_movespeed || last_stopvar != stop_moving) {
        Serial.print(" use_movespeed  = ");
        Serial.print(use_movespeed);
        Serial.println(" ");
    }
    last_var9 = use_movespeed;

    
    last_stopvar = stop_moving;
}

void run_program() {
    running_program = 1;
    //ready_for_functions_call = 0;
    switch (program_order) {
    case 1: // first position to go
        use_movespeed = 0;
        use_moveto_withspeed = 0;
        move_to_stepper1 = 1581;
        move_to_stepper2 = 10169;
        move_to_stepper3 = -10970;
        move_to_stepper4 = -39;
        move_to_stepper5 = -1954;
        move_to_stepper6 = 2050;
        Speed_max_stepper1 = 1000;
        Speed_max_stepper2 = 3000;
        Speed_max_stepper3 = 3000;
        Speed_max_stepper4 = 1000;
        Speed_max_stepper5 = 1500;
        Speed_max_stepper6 = 1500;
        stop_stepper1 = 0;
        stop_stepper2 = 0;
        stop_stepper3 = 0;
        stop_stepper4 = 0;
        stop_stepper5 = 0;
        stop_stepper6 = 0;
        move_all_to = 0; // alles teglijk aansturen 

        if (stepper1.currentPosition() == move_to_stepper1 && stepper2.currentPosition() == move_to_stepper2 && stepper3.currentPosition() == move_to_stepper3 && stepper4.currentPosition() == move_to_stepper4 && stepper5.currentPosition() == move_to_stepper5 && stepper6.currentPosition() == move_to_stepper6) {
            Serial.println(" position 1 reached ");
            program_order = 10;
        }
        break;
    case 10:
        servo_tool.attach(tool_servo_pin);
        servo_tool.writeMicroseconds(1500);
        static unsigned long time10 = millis();
        if (millis() >= time10 + 1000) {
            program_order = 11;
            servo_tool.detach();   
        }

    case 11:
        servo_tool.attach(tool_servo_pin);
        servo_tool.writeMicroseconds(500);
        static unsigned long time11 = millis();
        if (millis() >= time11 + 1000) {
            program_order = 20;
            servo_tool.detach();
        }
        break;
    case 20: // second position to go
        use_movespeed = 0;
        use_moveto_withspeed = 0;
        move_to_stepper1 = 1581;
        move_to_stepper2 = 8731;
        move_to_stepper3 = -21454;
        move_to_stepper4 = -39;
        move_to_stepper5 = -610;
        move_to_stepper6 = 676;
        Speed_max_stepper1 = 5000;
        Speed_max_stepper2 = 2000;
        Speed_max_stepper3 = 2000;
        Speed_max_stepper4 = 2000;
        Speed_max_stepper5 = 2000;
        Speed_max_stepper6 = 2000;
        stop_stepper1 = 0;
        stop_stepper2 = 0;
        stop_stepper3 = 0;
        stop_stepper4 = 0;
        stop_stepper5 = 0;
        stop_stepper6 = 0;
        move_all_to = 0; // alles teglijk aansturen 

        if (stepper1.currentPosition() == move_to_stepper1 && stepper2.currentPosition() == move_to_stepper2 && stepper3.currentPosition() == move_to_stepper3 && stepper4.currentPosition() == move_to_stepper4 && stepper5.currentPosition() == move_to_stepper5 && stepper6.currentPosition() == move_to_stepper6) {
            Serial.println(" position 2 reached ");
            program_order = 25;
        }
        break;
    case 25: // third position to go
        use_movespeed = 0;
        use_moveto_withspeed = 0;
        move_to_stepper1 = -18127;
        move_to_stepper2 = 9308;
        move_to_stepper3 = -24061;
        move_to_stepper4 = -81;
        move_to_stepper5 = 1290;
        move_to_stepper6 = 2542;
        Speed_max_stepper1 = 5000;
        Speed_max_stepper2 = 3000;
        Speed_max_stepper3 = 3000;
        Speed_max_stepper4 = 1000;
        Speed_max_stepper5 = 1500;
        Speed_max_stepper6 = 1500;
        stop_stepper1 = 0;
        stop_stepper2 = 0;
        stop_stepper3 = 0;
        stop_stepper4 = 0;
        stop_stepper5 = 0;
        stop_stepper6 = 0;
        move_all_to = 0; // alles teglijk aansturen 

        if (stepper1.currentPosition() == move_to_stepper1 && stepper2.currentPosition() == move_to_stepper2 && stepper3.currentPosition() == move_to_stepper3 && stepper4.currentPosition() == move_to_stepper4 && stepper5.currentPosition() == move_to_stepper5 && stepper6.currentPosition() == move_to_stepper6) {
            Serial.println(" position 1 reached ");
            program_order = 30;
        }
        break;
    case 30: // fourth position to go
        use_movespeed = 0;
        use_moveto_withspeed = 0;
        move_to_stepper1 = -16188;
        move_to_stepper2 = 10604;
        move_to_stepper3 = -12236;
        move_to_stepper4 = -81;
        move_to_stepper5 = 442;
        move_to_stepper6 = 4361;
        Speed_max_stepper1 = 2000;
        Speed_max_stepper2 = 2000;
        Speed_max_stepper3 = 2000;
        Speed_max_stepper4 = 2000;
        Speed_max_stepper5 = 2000;
        Speed_max_stepper6 = 2000;
        stop_stepper1 = 0;
        stop_stepper2 = 0;
        stop_stepper3 = 0;
        stop_stepper4 = 0;
        stop_stepper5 = 0;
        stop_stepper6 = 0;
        move_all_to = 0; // alles teglijk aansturen 

        if (stepper1.currentPosition() == move_to_stepper1 && stepper2.currentPosition() == move_to_stepper2 && stepper3.currentPosition() == move_to_stepper3 && stepper4.currentPosition() == move_to_stepper4 && stepper5.currentPosition() == move_to_stepper5 && stepper6.currentPosition() == move_to_stepper6) {
            Serial.println(" position 1 reached ");
            program_order = 35;
        }
        break;
    case 35:
        //servo_tool.writeMicroseconds(1500);
        static unsigned long time35 = millis();
        if (millis() >= time35 + 2000) {
            program_order = 40;
        }
        break;
    case 40:
        servo_tool.attach(tool_servo_pin);
        servo_tool.writeMicroseconds(1500);
        static unsigned long time40 = millis();
        if (millis() >= time40 + 2000) {
            program_order = 999;
            servo_tool.detach();
        }
        break;
    case 900: // run printpositions
        program_order = 0;
        break;
    case 999: //ga homen en dan run program again
        delay(1000);
        program_order = 1000;

    case 1000: // go to home position
        use_movespeed = 0;
        use_moveto_withspeed = 0;
        move_to_stepper1 = -2941;
        move_to_stepper2 = 1187;
        move_to_stepper3 = -2500;
        move_to_stepper4 = -431;
        move_to_stepper5 = -1577;
        move_to_stepper6 = -390;
        Speed_max_stepper1 = 3000;
        Speed_max_stepper2 = 3000;
        Speed_max_stepper3 = 3000;
        Speed_max_stepper4 = 3000;
        Speed_max_stepper5 = 3000;
        Speed_max_stepper6 = 3000;
        stop_stepper1 = 0;
        stop_stepper2 = 0;
        stop_stepper3 = 0;
        stop_stepper4 = 0;
        stop_stepper5 = 0;
        stop_stepper6 = 0;
        move_all_to = 0; // alles teglijk aansturen 

        if (stepper1.currentPosition() == move_to_stepper1 && stepper2.currentPosition() == move_to_stepper2 && stepper3.currentPosition() == move_to_stepper3 && stepper4.currentPosition() == move_to_stepper4 && stepper5.currentPosition() == move_to_stepper5 && stepper6.currentPosition() == move_to_stepper6) {
            Serial.println(" position 1 reached ");
            program_order = 1001;
        }
        break;
    case 1001:
        softwarehomen = 1;
        homed = 0;
        homed_axis1 = 0;
        homed_axis2 = 0;
        homed_axis3 = 0;
        homed_axis4 = 0;
        homed_axis5 = 0;
        homed_axis6 = 0;
        homeorder = 1;
        program_order = 0;
        break;

    }

}

void loop() {
    while (1) {
        read_inputs();
        print();
        control_stepper();

        // Serial.print(" status switch = ");

        if (!status_knop_noodstop_kast) {
            if (status_knop_reset) {
                digitalWrite(enable_all_steppers, LOW); // set all motors on
                digitalWrite(power_steppers_off, LOW); //schakel relais weer in
                stop_moving = 0;
                ready_for_functions_call = 1;
            }
            if (!status_knop_enable_all_steppers) {
                digitalWrite(enable_all_steppers, HIGH); // set all motors of
                homed = 0;
                homing = 0;
                homed_axis1 = 0;
                homed_axis2 = 0;
                homed_axis3 = 0;
                homed_axis4 = 0;
                homed_axis5 = 0;
                homed_axis6 = 0;
                homeorder = 1;
                softwarehomen = 1;
                printing_positions = 0;
            }
            if (status_knop_stop_controller || stop_moving) {
                stop_moving = 1;
            }

            if ((softwarejoggen == 1) && (stop_moving == 0) && (ready_for_functions_call == 1 || joging == 1)) {
                joggen();
            }
            else {
                joging = 0;
            }
            if ((softwarejoggen == 0 && softwarehomen == 1 && (stop_moving == 0) && (ready_for_functions_call == 1 || homing == 1))) {
                homed = home_all_axis();
                if (homed == 1) {
                    softwarehomen = 0;
                    homing = 0;
                    program_order = 1;
                }
            }

            if (homed || printing_positions) {
                printing_positions = 1;
                //printPosition();
                run_program();
            }
        }
        else {
            digitalWrite(enable_all_steppers, HIGH); // set all motors off
            digitalWrite(power_steppers_off, HIGH); //schakel relais
            homed = 0;
            homing = 0;
            homed_axis1 = 0;
            homed_axis2 = 0;
            homed_axis3 = 0;
            homed_axis4 = 0;
            homed_axis5 = 0;
            homed_axis6 = 0;
            homeorder = 1;
            softwarehomen = 1;
            printing_positions = 0;
        }
    }
}
