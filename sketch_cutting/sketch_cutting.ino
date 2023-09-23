#pragma once
#include "Controller.h"

#define  A_PHASE 2
#define  B_PHASE 3
int encSteps = 0;  

Controller* controller;

unsigned long lastTime;

void interrupt()
{
    int i = digitalRead( B_PHASE);
    if (i == 0)
    {
        encSteps -= 1;
    }
    else{
        encSteps += 1;
    }
}

void setup()
{
    pinMode(A_PHASE, INPUT_PULLUP);
    pinMode(B_PHASE, INPUT_PULLUP);

    digitalWrite(A_PHASE, HIGH);
    digitalWrite(B_PHASE, HIGH);

    Serial.begin(9600);
    attachInterrupt(digitalPinToInterrupt(A_PHASE), interrupt, FALLING);
    
    controller = new Controller();
    lastTime = micros();
}
unsigned updateTimer = 0;
unsigned dTime = 0;
void loop()
{
    const unsigned long currTime = micros();
    dTime += currTime - lastTime; 
    
    if(dTime>200)
    {
        controller->Update(dTime, &encSteps);
        dTime=0;
    }
    lastTime = currTime;
}