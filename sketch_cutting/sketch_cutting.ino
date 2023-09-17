#pragma once
#include "Controller.h"

#define  A_PHASE 2
#define  B_PHASE 3
int encSteps = 0;  
int encSteps2 = 0;
Controller* controller;

unsigned long lastTime;
void setup()
{
    pinMode(A_PHASE, INPUT_PULLUP);
    pinMode(B_PHASE, INPUT_PULLUP);

    digitalWrite(A_PHASE, HIGH);
    digitalWrite(B_PHASE, HIGH);

    Serial.begin(9600);   //Serial Port Baudrate: 9600
    attachInterrupt(digitalPinToInterrupt(A_PHASE), interrupt, FALLING); //Interrupt trigger mode: RISING
    
    controller = new Controller();
    lastTime = micros();
}
unsigned updateTimer = 0;
void loop()
{
    // delayMicroseconds(5);
    const unsigned long currTime = micros();
    if(lastTime - currTime > 300)
    {
        const unsigned dTime = currTime - lastTime; 
        Serial.print("steps:  ");
        Serial.println(encSteps2);
        
        controller->Update(dTime, &encSteps);
    }
    lastTime = currTime;
}

void interrupt()
{
    int i = digitalRead( B_PHASE);
    if (i == 0)
    {
        encSteps -= 1;
        encSteps2 -= 1;
    }
    else{
        encSteps += 1;
        encSteps2 += 1;
    }
}