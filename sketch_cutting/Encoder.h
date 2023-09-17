#pragma once

class Encoder
{
    static int pinA;
    static  int pinB;
    volatile static unsigned pulsesA;
    volatile static unsigned pulsesB;
    volatile static unsigned long lastTime;
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

