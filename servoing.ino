#include <time.h>

//pin definitions
#define APIN 5
#define BPIN 6
#define IRPIN_0 10
#define IRPIN_1 11

//global variables
#define MOTOR_RATIO 75
#define MIN_ANGLE -720
#define MAX_ANGLE 720
#define STOP_MOTOR 2
#define CW_MOTOR 0
#define CCW_MOTOR 1
#define MOTOR_SPEED 110
#define ANGLE_PER_TICK 45
volatile int ir_cnt = 0;
volatile float des_angle = 0;
volatile float cur_angle = 0;
volatile float angle_delta = 0;
volatile int cur_dir = 2;


void setup() {
  pinMode(APIN, OUTPUT);
  pinMode(BPIN, OUTPUT);
  pinMode(IRPIN_0, INPUT);
  pinMode(IRPIN_1, INPUT);
  attachInterrupt(IRPIN_0, infraredISR, CHANGE);
  attachInterrupt(IRPIN_1, infraredISR, CHANGE);
  Serial1.begin(9600);
}

void loop() {
  
  if (Serial1.available() > 0){  //check if data is received from serial
    String curString = Serial1.readString(); //data is a string
    char curChar[curString.length()+1];
    strcpy(curChar, curString.c_str());     //convert string to char array 
    des_angle = atoi(curChar);             //returns exact int send by user
    if (MIN_ANGLE > des_angle){
      des_angle = MIN_ANGLE;
    }
    if (MAX_ANGLE < des_angle){
      des_angle = MAX_ANGLE;
    } 
  }
  
  angle_delta = des_angle - cur_angle;

  //may cause the motor to continuously change directions around the desired angle
  if (0==angle_delta){
    setMotor(STOP_MOTOR);
    ir_cnt = 0;
  }
  else if (0 < angle_delta){
    setMotor(CW_MOTOR);
  }
  else if (0 > angle_delta){
    setMotor(CCW_MOTOR);
  }
}


//sets motor speed/direction
void setMotor(int dir){
  cur_dir = dir;
  if (CW_MOTOR == dir) {
    analogWrite(BPIN, 0);
    analogWrite(APIN, MOTOR_SPEED);
  }
  else if (CCW_MOTOR == dir) {
    analogWrite(APIN, 0);
    analogWrite(BPIN, MOTOR_SPEED);
  }
  else if (STOP_MOTOR == dir) {
    analogWrite(APIN, 0);
    analogWrite(BPIN, 0);
  }
}



//Interrupt for both infrared sensors.
void infraredISR(){
  ir_cnt = ir_cnt + 1;
  if (CW_MOTOR == cur_dir){
    ir_cnt = ir_cnt + 1;
  }
  else if (CCW_MOTOR == cur_dir){
    ir_cnt = ir_cnt - 1;
  }
  cur_angle = cur_angle+(float)(ANGLE_PER_TICK/MOTOR_RATIO);
}
