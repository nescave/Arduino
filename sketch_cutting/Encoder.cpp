#include "Encoder.h"
#include <Arduino.h>

bool Encoder::Init(const int pinRight, const int pinLeft) 
{
    pinA = pinRight;
    pinB = pinLeft;

    pinMode(pinA, INPUT_PULLUP);
    pinMode(pinB, INPUT_PULLUP);
    digitalWrite(pinA, HIGH);
    digitalWrite(pinB, HIGH);

    attachInterrupt(digitalPinToInterrupt(2), [](){Encoder::Get().OnPulseA();}, RISING);
    attachInterrupt(digitalPinToInterrupt(3), [](){Encoder::Get().OnPulseB();}, RISING);
 
    return true;
}

void Encoder::OnPulseA()
{
    const auto a = digitalRead(pinA);
    const auto b = digitalRead(pinB);
    if(a == 1 && b == 0){
        // Serial.println("PulseA");
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
