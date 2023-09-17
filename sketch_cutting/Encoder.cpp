#include "Encoder.h"
#include <Arduino.h>

int Encoder::pinA = 0;
int Encoder::pinB = 0;
volatile unsigned Encoder::pulsesA = 0;
volatile unsigned Encoder::pulsesB = 0;

void Encoder::Init(const int pinRight, const int pinLeft) 
{
    pinA = pinRight;
    pinB = pinLeft;

    pulsesA = 0;
    pulsesB = 0;
    
    pinMode(pinA, INPUT_PULLUP);
    pinMode(pinB, INPUT_PULLUP);
    // digitalWrite(pinA, HIGH);
    // digitalWrite(pinB, HIGH);

    attachInterrupt(digitalPinToInterrupt(2), [](){Encoder::Get().OnPulseA();}, FALLING);
}

void Encoder::OnPulseA()
{
    if(digitalRead(pinB) == LOW){
        ++pulsesB;
    }
    else
    {
        ++pulsesA;
    }
}

void Encoder::OnPulseB()
{
    const auto a = digitalRead(pinA);
    const auto b = digitalRead(pinB);
    if(a == 0 && b == 1){
        // Serial.println("PulseB");
        ++pulsesB;
    }
}

Encoder& Encoder::Get()
{
    static Encoder instance;
    return instance;
}

unsigned Encoder::GetPositivePulses()
{
    const auto ret = pulsesA;
    return ret;
}

int Encoder::GetPulsesSum()
{
    const auto ret = (int)pulsesA - (int)pulsesB;
    return ret;
}
void Encoder::ResetPulses()
{
    pulsesA = pulsesB = 0;
}
