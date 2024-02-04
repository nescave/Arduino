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
    cutting(false),
    maxSpeed(300)
{
    pinMode(pinPull, OUTPUT);
    pinMode(pinDirection, OUTPUT);
    pinMode(pinDisabler, OUTPUT);
    pinMode(pinRestPosition, INPUT);
    digitalWrite(pinDisabler, powerOff);
    digitalWrite(pinDirection, reverse);
    digitalWrite(pinPull, false);

    stepper.setAcceleration(1600);
    stepper.setMaxSpeed(maxSpeed);
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
    
    RotationsInTime(2,5.5f);
}

bool Motor::RotationsInTime(float rotations, float sec)
{
    if(!IsFree()) return false;
    SetPower(true);
    stepper.move(long(stepsPerRev*rotations));
    SetSpeedSteps((float)stepper.distanceToGo()/sec);
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
    stepper.setMaxSpeed(min(max(speed,30.0f), maxSpeed));
    // Serial.println(stepper.maxSpeed());
}

void Motor::SetSpeedLinear(float speed)
{
    stepper.setMaxSpeed(min(max(speed/0.3768f, 30.0f), maxSpeed)); 
    // Serial.println(stepper.maxSpeed());
}

void Motor::SetPower(bool powerOn)
{
    digitalWrite(pinDisabler, !powerOn);
}

void Motor::Update()
{
    if(stepper.distanceToGo() !=0)
    {
        stepper.run();
        if(cutting)
        {
            Serial.println("tnie");
        }else{
            Serial.println("wraca");
        }
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
