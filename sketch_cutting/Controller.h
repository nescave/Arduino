#pragma once
#include <LiquidCrystal.h>

class Encoder;
class Input;
class Motor;

class Controller
{
    Encoder* encoder;
    Input* input;
    Motor* motor;
    LiquidCrystal lcd;

    short valueSelector; 
    String* activeName;
    float* activeValue;
    float distanceTraveled;
    
    String names[3];
    float values[3];
    // cuttingLength;
    // diameter;
    // speedFactor;

    double superSlowUpdateTimer;
    double slowUpdateTimer;
    double fastUpdateTimer;

    float pulsesCounter;
    unsigned sps;
    bool enabled;
    
    float GetDistancePerStep() const;

    void ToggleValues();
    void AdjustActiveValueBy(float val) const;
    void AccTime(unsigned micros);
    float GetMaterialLen() const;
    float GetDiameter() const;
    float GetSpeedFactor() const;
    unsigned GetSpm(unsigned pulses, unsigned milisecs) const;
    void StartCutting();
    void ToggleEnable();
    void ScreenDraw();
public:
    Controller();
    ~Controller();

    void Update(unsigned dTime);
};
