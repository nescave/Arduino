#pragma once
#include <LiquidCrystal.h>

class Encoder;
class Input;
class Motor;

class Controller
{
    // Encoder* encoder;
    Input* input;
    LiquidCrystal lcd;

    short valueSelector; 
    String* activeName;
    float* activeValue;
    
    String names[2];
    float values[2];

    double fastUpdateTimer;

    float speed;
    int numPieces;
    int numAllPieces;
    double timeBtwCuts;

    void ToggleValues();
    void AdjustActiveValueBy(float val) const;
    void AccTime(unsigned micros);
    void ResetPieces();
    void StartCutting(float averageSpeed);
    void ScreenDraw();
public:
    Motor* motor;
    bool shouldCut;
    
    Controller();
    ~Controller();

    void Update(unsigned dTime, int* encSteps);
};
