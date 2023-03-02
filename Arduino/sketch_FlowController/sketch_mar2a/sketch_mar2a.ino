#pragma once
#include <AccelStepper.h>
#include "Utils.h"
#include "lcd.h"

AccelStepper motor(AccelStepper::FULL4WIRE, 0,1,11,12, true);
unsigned maxSpeed = 250;
void setup() {
  // put your setup code here, to run once:
  motor.setMaxSpeed(750.0);
  motor.setAcceleration(250.0);
  motor.setSpeed(maxSpeed);
  // motor.
  // motor.move(500);
  lcd.begin(16,2);
}
// bool motorOutputs = true;
void ToggleMotor(){
  motor.disableOutputs();
}

long targetPos = 0;
bool RunMotor(){
  int currPos = motor.currentPosition();
  long diff = currPos - targetPos;
  if (diff == 0) {
    motor.setSpeed(0);
    String a = "    ";
    return false;
  }
  float mod = Sign(diff)*-1*min(abs(float(diff)/10.0),1.0f);
  motor.setSpeed(maxSpeed*mod);
  motor.runSpeed();
  return true;
}

const int steps = 400;
void AddStepsForward(){
  targetPos -= steps;
  LCDPrint(0,0, "forward");
}
void AddStepsBackward(){
  targetPos += steps;
  LCDPrint(0,0, "backward");
}

void IncreaseMaxSpeed(){
  maxSpeed+=100;
}
void DecreaseMaxSpeed(){
  maxSpeed-=100;
}

void loop() {
  Button bt = GetButton();
  if(bt == Button::SELECT){
    SingleButtonAction(bt, ToggleMotor);
  }else if(bt == Button::LEFT){
    SingleButtonAction(bt, AddStepsBackward);
  }else if(bt == Button::RIGHT){
    SingleButtonAction(bt, AddStepsForward);
  }else if(bt == Button::UP){
    SingleButtonAction(bt, IncreaseMaxSpeed);
  }else if(bt == Button::DOWN){
    SingleButtonAction(bt, DecreaseMaxSpeed);
  }
  ReleaseButton(bt);
  
  if(!RunMotor()){
    ToggleMotor();
  }

  long pos = motor.currentPosition(); 
  LCDPrint(1,1, -pos);
  LCDPrint(9,0, maxSpeed);
  delay(5);
  // motor
  // motor.runSpeed();
}
