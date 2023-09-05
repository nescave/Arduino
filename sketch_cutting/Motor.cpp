#include "Motor.h"
#include <Arduino.h>

void Motor::SetPullHigh()
{
    digitalWrite(pinPull, HIGH);
    // Serial.println("HIGH");
}

void Motor::SetPullLow()
{
    digitalWrite(pinPull, LOW);
    ++stepsDone;
    // Serial.println("LOW");
}

double Motor::GetDelay()
{
    if(useSpeed && localSps > 0) return EnsureCorrectDelay(1000000/localSps);
    return stepDelay;
}

double Motor::GetLowestDelay()
{
    return EnsureCorrectDelay(0);
}

Motor::Motor(const int pinMotorPull, const int pinMotorDirection, const int pinMotorDisabler, const int pinMotorRestPosition) :
    pinPull(pinMotorPull),
    pinDirection(pinMotorDirection),
    pinDisabler(pinMotorDisabler),
    pinRestPosition(pinMotorRestPosition),
    reverse(false),
    powerOff(true),
    searching(false),
    isPulling(true),
    stepDelay(300),
    stepTimer(0),
    wheelRatio(55.0/27.5),
    stepsPerRev(round(400*wheelRatio)),
    stepsDone(0),
    stepsToMake(0),
    cutting(false),
    localSps(0),
    useSpeed(false),
    workingTime(0)
{
    pinMode(pinPull, OUTPUT);
    pinMode(pinDirection, OUTPUT);
    pinMode(pinDisabler, OUTPUT);
    pinMode(pinRestPosition, INPUT);
    digitalWrite(pinDisabler, powerOff);
    digitalWrite(pinDirection, reverse);
    digitalWrite(pinPull, false);

    Serial.println("motor const");
}

bool Motor::IsFree()
{
    const auto ret = stepsDone >= stepsToMake;
    powerOff = ret;
    return ret;
}

void Motor::FindRestPos()
{
    Serial.println("FindRestPos");

    cutting = false;
    searching = true;
    RotationsInTime(1,6000);
}

void Motor::ComeBackInTime(double milisecs)
{
    searching = true;
    // RotationsInTime(0.72, 0.72*stepsPerRev*GetLowestDelay()/1000);
    RotationsInTime(0.72, milisecs);
}

bool Motor::RotationsInTime(float rotations, double milisecs)
{
    Serial.println("RotationsInTime");
    // Serial.print()
    if(!IsFree()) return false;
    stepsToMake = unsigned((float)stepsPerRev*rotations);
    stepDelay = EnsureCorrectDelay(double(milisecs*1000)/stepsToMake);

    return true;
}

bool Motor::RotationsWithSpeed(float rotations)
{
    Serial.println("RotationsWithSpeed");

    if(!IsFree()) return false;
    stepsToMake = unsigned((float)stepsPerRev*rotations);
    useSpeed = true;
    return true;
}

double Motor::EnsureCorrectDelay(double delay) const
{
    return max(min(delay, 6000),1250);
}

void Motor::CleanupAfterStep()
{
    stepsToMake = stepsDone = 0;
    if(cutting)
    {
        cutting = false;
        ComeBackInTime();
    }
    useSpeed = false;
    // searching = false;
}

void Motor::Update(unsigned dTime, double sps)
{
    localSps = max(sps, 1);
    digitalWrite(pinDisabler, powerOff);
    const double delay = GetDelay()/2;
    for(;stepTimer >= delay; stepTimer -= (unsigned)delay){
        if(IsFree())
        {
            continue;
        }
        
        digitalWrite(pinDirection, reverse);
        if(isPulling)
        {
            SetPullHigh();
        }else
        {
            SetPullLow();
        }
        isPulling = !isPulling;
    
        if(stepsDone >= stepsToMake) //TODO: extract method
        {
            CleanupAfterStep();
        }
        if(searching)
        {
            if(digitalRead(pinRestPosition)){
                stepsToMake = stepsDone = 0;
                searching = false;
            }
        }
    }
    stepTimer+=dTime;
}
