#pragma once
#include "Controller.h"

#define  A_PHASE 2
#define  B_PHASE 3
int encSteps = 0;  

Controller* controller;
unsigned long lastTime;
bool cutSignal;

void interrupt()
{

    if(!controller->IsCutting() && controller->shouldCut == false)
    {
        cutSignal = true;
        Serial.println("ping!");
    }
}

void setup()
{
    pinMode(A_PHASE, INPUT_PULLUP);

    digitalWrite(A_PHASE, HIGH);

    Serial.begin(9600);
    // attachInterrupt(digitalPinToInterrupt(A_PHASE), interrupt, RISING);
    
    controller = new Controller();
    lastTime = micros();
}
unsigned updateTimer = 0;
unsigned dTime = 0;
void loop()
{
    const unsigned long currTime = micros();
    dTime += currTime - lastTime; 

    if(!digitalRead(A_PHASE))    
        cutSignal = true;

    if(dTime>200)
    {
        if(!controller->IsCutting() && cutSignal)
        {
            // Serial.println(controller->IsCutting());
            controller->shouldCut = cutSignal;

            cutSignal = false;
        }
        controller->Update(dTime, &encSteps);
        dTime=0;
    }
    lastTime = currTime;
}