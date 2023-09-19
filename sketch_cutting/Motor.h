#pragma once

#include "AccelStepper.h"

class Motor
{
    const int pinPull;
    const int pinDirection;
    const int pinDisabler;
    const int pinRestPosition;
    
    AccelStepper stepper;

    unsigned stepTimer;
    double localSps;
    double wheelRatio;
    
    void SetPullHigh();
    void SetPullLow();

    double GetDelay();
    double GetLowestDelay();
    
    bool reverse;
    bool powerOff;

    bool useSpeed;
    bool isPulling;
public:
    bool searching;
    Motor(int pinMotorPull, int pinMotorDirection, int pinMotorDisabler, int pinMotorRestPosition);
    float stepsPerRev;
    double stepDelay;
    double workingTime;
    double motorSpeed;

    unsigned stepsToMake;
    unsigned stepsDone;
    
    bool cutting;
    bool IsFree(); 
    void SwitchDirection(){reverse = !reverse;}
    void SetPower(bool isOff){powerOff = isOff;}

    void FindRestPos();
    void ComeBackInTime(double milisecs = 4000);
    bool RotationsInTime(float rotations, double sec);
    bool RotationsWithSpeed(float rotations, float speed);
    double EnsureCorrectDelay(double delay) const;
    void CleanupAfterStep();
    void SetSpeed(double speed);
    void Update();    
};
