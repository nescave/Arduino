#include "Controller.h"
#include <LiquidCrystal.h>
#include "Motor.h"
#include "Input.h"
#include "Arduino.h"

Controller::Controller() :
    activeName(nullptr),
    activeValue(nullptr),
    valueSelector(0),
    fastUpdateTimer(0),
    values{335, 67.5f},
    names{"Dlug wezy:","Srednica:"},
    lcd(LiquidCrystal(8, 9, 4, 5, 6, 7)),
    speed(0),
    numPieces(-1),
    numAllPieces(-1),
    timeBtwCuts(0),
    shouldCut(false)
{
    Serial.begin(9600);
    lcd.begin(16, 2);
    
    activeName = &names[0];
    activeValue = &values[0];
    
    input = new Input();
    motor = new Motor(11,12,13,1);
   
    motor->FindRestPos();
}

Controller::~Controller()
{
    delete input;
    delete motor;
}

void Controller::Update(unsigned dTime, int* encSteps)
{
    motor->Update();

    timeBtwCuts+=double(dTime)/1000000;
    //start cutting when distance reaches limit
    if(shouldCut && motor->IsFree())
    {
        shouldCut = false;
        StartCutting(GetMaterialLen()/timeBtwCuts);
        timeBtwCuts = 0;
    }

    //Input
    constexpr unsigned fastUpdateDelay = 200;
    if(fastUpdateTimer >= fastUpdateDelay)
    {
        input->Update(fastUpdateDelay*.5f);
        if(input->WasBtnPressed(SELECT)) ToggleValues();
        if(input->WasBtnPressed(LEFT)) AdjustActiveValueBy(-1);
        if(input->WasBtnPressed(RIGHT)) AdjustActiveValueBy(1);
        if(input->IsBtnHeld(LEFT)) AdjustActiveValueBy(-1);
        if(input->IsBtnHeld(RIGHT)) AdjustActiveValueBy(1);
        if(input->WasBtnPressed(DOWN)) ResetPieces();
        if(input->WasBtnPressed(UP)) ResetPieces();

        ScreenDraw();
        fastUpdateTimer -= fastUpdateDelay;
    }

    AccTime(dTime);
}

void Controller::ToggleValues()
{
    ++valueSelector;
    if(valueSelector>2) valueSelector=0;
    activeName = &names[valueSelector];
    activeValue = &values[valueSelector];
}

void Controller::AdjustActiveValueBy(float val) const
{
    switch (valueSelector)
    {
    case 1:
        val*=0.05;
        break;
    case 2:
        val*=0.1;
        break;
    default:
        break;
    }
    *activeValue += val;
}

void Controller::AccTime(unsigned micros)
{
    fastUpdateTimer += (double)micros/1000;
}

float Controller::GetMaterialLen() const
{
    return values[0];
}

void Controller::ResetPieces()
{
    numPieces = 0;
}

void Controller::StartCutting(float averageSpeed)
{
    motor->cutting = true;
    motor->searching = false;
    numPieces++;
    numAllPieces++;
    motor->RotationsWithSpeed(0.25f, averageSpeed*1.5f);
}

void Controller::ScreenDraw()
{
    lcd.clear();
    //first row
    lcd.setCursor(0, 0);
    lcd.print("szt:");
    lcd.setCursor(10, 0);
    lcd.print(numAllPieces);

    //second row
    lcd.setCursor(0, 1);
    lcd.print("teraz:");
    lcd.setCursor(10, 1);
    lcd.print(numPulses);
}
