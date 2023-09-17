#pragma once

class Encoder
{
    static int pinA;
    static  int pinB;
    static volatile unsigned pulsesA;
    static volatile unsigned pulsesB;
    Encoder() = default;
    
public:
    void Init(const int pinRight, const int pinLeft);

    static void OnPulseA();
    static void OnPulseB();
    static Encoder& Get();
    static unsigned GetPositivePulses();
    static int GetPulsesSum(); 
    static void ResetPulses();

    static void Update(unsigned dTime); //TODO encoder's update must resolve last
};

