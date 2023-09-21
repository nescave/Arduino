#include "Motor.h"
#include <Arduino.h>

Motor::Motor(const int pinMotorPull, const int pinMotorDirection, const int pinMotorDisabler, const int pinMotorRestPosition) :
    pinPull(pinMotorPull),
    pinDirection(pinMotorDirection),
    pinDisabler(pinMotorDisabler),
    pinRestPosition(pinMotorRestPosition),
    stepper(AccelStepper::DRIVER, pinMotorPull, pinMotorDirection),
    reverse(true),
    powerOff(true),
    searching(false),
    wheelRatio(2.0/1.0),
    stepsPerRev(400.0f*wheelRatio),
    cutting(false)
{
    pinMode(pinPull, OUTPUT);
    pinMode(pinDirection, OUTPUT);
    pinMode(pinDisabler, OUTPUT);
    pinMode(pinRestPosition, INPUT);
    digitalWrite(pinDisabler, powerOff);
    digitalWrite(pinDirection, reverse);
    digitalWrite(pinPull, false);

    stepper.setAcceleration(1000);
    stepper.setMaxSpeed(600);
    stepper.setSpeed(200);
    stepper.setCurrentPosition(0);
}

bool Motor::IsFree()
{
    return stepper.distanceToGo()==0;
}

void Motor::SwitchDirection()
{
    reverse = !reverse;
    digitalWrite(pinDirection, reverse);
}

void Motor::FindRestPos()
{
    cutting = false;
    searching = true;
    SetPower(true);
    
    RotationsInTime(2,3);
}

bool Motor::RotationsInTime(float rotations, float sec)
{
    if(!IsFree()) return false;
    SetPower(true);
    stepper.move(long(stepsPerRev*rotations));
    SetSpeedSteps((float)stepper.distanceToGo()/sec);
    // Serial.print("RotationsInTime   ");
    // Serial.println(stepper.speed());
    digitalWrite(pinDisabler, false);
    return true;
}

bool Motor::RotationsWithSpeed(float rotations, float speed)
{
    if(!IsFree()) return false;
    SetPower(true);

    stepper.move(long(stepsPerRev*rotations));
    SetSpeedLinear(speed);
    
    return true;
}

void Motor::SetSpeedSteps(float speed)
{
    stepper.setSpeed(speed);
}

void Motor::SetSpeedLinear(float speed)
{
    stepper.setSpeed(max(float(speed/0.3768)*2, 50)); 
}

void Motor::SetPower(bool powerOn)
{
    digitalWrite(pinDisabler, !powerOn);
}

void Motor::Update()
{
    if(stepper.distanceToGo() !=0)
    {
        stepper.runSpeed();
    }else{
        if(cutting)
        {
            FindRestPos();
            cutting = false;
        }else{
            SetPower(false);
        }
    }

    if(searching)
    {
        if(digitalRead(pinRestPosition)){
            stepper.setCurrentPosition(0);
            stepper.stop();
            searching = false;
        }
    }
}
