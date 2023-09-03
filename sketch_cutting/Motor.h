#pragma once

class Motor
{
    const int pinPull;
    const int pinDirection;
    const int pinDisabler;
    const int pinRestPosition;

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
    bool searching;
    bool isPulling;
public:
    Motor(int pinMotorPull, int pinMotorDirection, int pinMotorDisabler, int pinMotorRestPosition);
    unsigned stepsPerRev;
    double stepDelay;
    double workingTime;
    
    unsigned stepsToMake;
    unsigned stepsDone;
    
    bool cutting;
    bool IsFree(); 
    void SwitchDirection(){reverse = !reverse;}
    void SetPower(bool isOff){powerOff = isOff;}

    void FindRestPos();
    void ComeBackInTime(double milisecs = 200);
    bool RotationsInTime(float rotations, double milisecs);
    bool RotationsWithSpeed(float rotations);
    double EnsureCorrectDelay(double delay) const;
    void CleanupAfterStep();
    void Update(unsigned dTime, double sps);    
};
