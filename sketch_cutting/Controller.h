#pragma once
#include <LiquidCrystal.h>

class Encoder;
class Input;
class Motor;

class Controller
{
    // Encoder* encoder;
    Input* input;
    Motor* motor;
    LiquidCrystal lcd;

    short valueSelector; 
    String* activeName;
    float* activeValue;
    float distanceTraveled;
    
    String names[2];
    float values[2];

    double superSlowUpdateTimer;
    double slowUpdateTimer;
    double fastUpdateTimer;

    unsigned pulsesCounter;
    float speed;
    int numPieces;
    
    float GetDistancePerStep() const;

    void ToggleValues();
    void AdjustActiveValueBy(float val) const;
    void AccTime(unsigned micros);
    float GetMaterialLen() const;
    float GetDiameter() const;
    void ResetPieces();
    void StartCutting();
    void ScreenDraw();
public:
    
    Controller();
    ~Controller();

    void Update(unsigned dTime, int* encSteps);
};
