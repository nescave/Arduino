#pragma once
#include "Controller.h"

Controller* controller;

unsigned long lastTime;
void setup()
{
    controller = new Controller();
    lastTime = micros();
}
unsigned updateTimer = 0;
void loop()
{
    // delayMicroseconds(5);
    const unsigned long currTime = micros();
    const unsigned dTime = currTime - lastTime; 
    if(dTime>500)
    {
        Serial.println(dTime);
    }else
    {
        controller->Update(dTime);
    }
    lastTime = currTime;
}