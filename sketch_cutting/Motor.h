#pragma once

#include "AccelStepper.h"

class Motor
{
    const int pinPull;
    const int pinDirection;
    const int pinDisabler;
    const int pinRestPosition;
    
    AccelStepper stepper;
    float wheelRatio;
    
    bool reverse;
    bool powerOff;
    float stepsPerRev;
    float maxSpeed;

    void SetSpeedSteps(float speed);
    void SetSpeedLinear(float speed);
    void SetPower(bool powerOn);
public:
    bool searching;
    bool cutting;
    
    Motor(int pinMotorPull, int pinMotorDirection, int pinMotorDisabler, int pinMotorRestPosition);

    bool IsFree(); 

    void SwitchDirection();

    void FindRestPos();
    bool RotationsInTime(float rotations, float sec);
    bool RotationsWithSpeed(float rotations, float speed);

    void Update();    
};
