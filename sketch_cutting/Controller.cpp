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
    slowUpdateTimer(0),
    SlowUpdateTimer(0),
    values{335, 53.3f},
    distanceTraveled(0),
    names{"Dlug wezy:","Srednica:"},
    lcd(LiquidCrystal(8, 9, 4, 5, 6, 7)),
    pulsesCounter(0),
    speed(0),
    numPieces(-1),
    timeBtwCuts(0)
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

    //pulses and distance
    const auto pulses = *encSteps;
    *encSteps = 0;        
    distanceTraveled +=  GetDistancePerStep() * (float)pulses;
    
    timeBtwCuts+=double(dTime)/1000000;
    //start cutting when distance reaches limit
    if(distanceTraveled >= GetMaterialLen() && motor->IsFree())
    {
        StartCutting(GetMaterialLen()/timeBtwCuts);
        timeBtwCuts = 0;
        distanceTraveled -= GetMaterialLen();
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

double Controller::GetDistancePerStep() const
{
    return float(GetDiameter() * 3.14159265 / 400.0);
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
    SlowUpdateTimer += (double)micros/1000000;
}

float Controller::GetMaterialLen() const
{
    return values[0];
}

float Controller::GetDiameter() const
{
    return values[1];
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
    motor->RotationsWithSpeed(0.25f, averageSpeed*1.1f);
}

void Controller::ScreenDraw()
{
    lcd.clear();
    //first row
    lcd.setCursor(0, 0);
    lcd.print(*activeName);
    lcd.setCursor(11, 0);
    lcd.print(*activeValue);

    //second row
    lcd.setCursor(0, 1);
    lcd.print("szt:");
    lcd.setCursor(5, 1);
    lcd.print(max(numPieces,0));
}
