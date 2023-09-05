#include "Controller.h"
#include <LiquidCrystal.h>
#include "Motor.h"
#include "Input.h"
#include "Encoder.h"
#include "Arduino.h"

Controller::Controller() :
    encoder(&Encoder::Get()),
    activeName(nullptr),
    activeValue(nullptr),
    valueSelector(0),
    fastUpdateTimer(0),
    slowUpdateTimer(0),
    superSlowUpdateTimer(0),
    values{330, 88.25, 2.5f},
    distanceTraveled(0),
    names{"Dlug wezy:","Srednica:", "ilosc szt", "speed fac:"},
    lcd(LiquidCrystal(8, 9, 4, 5, 6, 7)),
    pulsesCounter(0),
    sps(0),
    enabled(true),
    cutNum(0)
{
    encoder->Init(2,3);
    Serial.begin(9600);
    lcd.begin(16, 2);
    
    activeName = &names[0];
    activeValue = &values[0];
    
    input = new Input();
    motor = new Motor(11,12,13,1);
   
    motor->SwitchDirection();
    motor->FindRestPos();
}

Controller::~Controller()
{
    delete encoder;
    delete input;
    delete motor;
}

void Controller::Update(unsigned dTime)
{
    constexpr unsigned fastUpdateDelay = 500;
    if(fastUpdateTimer >= fastUpdateDelay)
    {
        // Serial.println("fastUpdate!");
        motor->Update(fastUpdateDelay, (double)sps* GetSpeedFactor());
        fastUpdateTimer -= fastUpdateDelay;
    }
    constexpr double slowUpdateDelay = 50;
    if(slowUpdateTimer>= slowUpdateDelay)
    {
        if(input->WasBtnPressed(SELECT)) ToggleValues();
        if(input->WasBtnPressed(LEFT)) AdjustActiveValueBy(-1);
        if(input->WasBtnPressed(RIGHT)) AdjustActiveValueBy(1);
        if(input->IsBtnHeld(LEFT)) AdjustActiveValueBy(-1);
        if(input->IsBtnHeld(RIGHT)) AdjustActiveValueBy(1);
        if(input->WasBtnPressed(DOWN)) motor->RotationsInTime(1, 2000);
        if(input->WasBtnPressed(UP)) StartCutting();

        const auto pulses = encoder->GetPositivePulses();
        // const auto pulses = 1.5f;
        if(enabled)
        {
            pulsesCounter += pulses;
            distanceTraveled +=  GetDistancePerStep() * (float)pulses;
        }
        if(distanceTraveled >= GetMaterialLen() && motor->IsFree())
        {
            StartCutting();
            distanceTraveled -= GetMaterialLen();
        }
        
        ScreenDraw();
        input->Update(slowUpdateDelay);
        // encoder->Update(slowUpdateDelay);
        slowUpdateTimer-=slowUpdateDelay;
    }

    constexpr double superSlowUpdateDelay = 0.25;
    if(superSlowUpdateTimer >= superSlowUpdateDelay)
    {

        sps = unsigned((float)pulsesCounter/superSlowUpdateDelay);
        // sps = 75;
        pulsesCounter = 0;
        superSlowUpdateTimer -= superSlowUpdateDelay;
    }
    
    AccTime(dTime);
}

float Controller::GetDistancePerStep() const
{
    return float(GetDiameter() * 3.14159265 / 400.0);
}

void Controller::ToggleValues()
{
    ++valueSelector;
    if(valueSelector>3) valueSelector=0;
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
    fastUpdateTimer += (double)micros;
    slowUpdateTimer += (double)micros/1000;
    superSlowUpdateTimer += (double)micros/1000000;
}

float Controller::GetMaterialLen() const
{
    return values[0];
}

float Controller::GetDiameter() const
{
    return values[1];
}

float Controller::GetNumberPieces() const
{
    return values[2];
}

float Controller::GetSpeedFactor() const
{
    return values[3];
}

unsigned Controller::GetSpm(unsigned pulses, unsigned milisecs) const
{
    return unsigned((float)pulses/(float)milisecs);
}

void Controller::StartCutting()
{
    motor->cutting = true;
    motor->searching = false;
    cutNum++;
    motor->RotationsWithSpeed(0.28);
}

void Controller::ToggleEnable()
{
    if(enabled == true)
    {
        distanceTraveled = 0;
        pulsesCounter= 0;
    }
    enabled = !enabled;
}

void Controller::ScreenDraw()
{
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print(*activeName);
    lcd.setCursor(11, 0);
    lcd.print(*activeValue);
    // lcd.setCursor(0, 1);
    // lcd.print(sps);
    // lcd.print(motor->stepDelay);
    // lcd.print("ilosc szt");
    lcd.setCursor(8, 1);
    lcd.print(distanceTraveled);
}