#pragma once

class Encoder
{
    inline static int pinA = 0;
    inline static  int pinB = 0;
    inline static unsigned pulsesA = 0;
    inline static unsigned pulsesB = 0;
    Encoder() = default;
    
public:
    bool Init(const int pinRight, const int pinLeft);
    static void OnPulseA();
    static void OnPulseB();
    static Encoder& Get();
    static unsigned GetPositivePulses();
    static int GetPulsesSum(); 

    static void Update(unsigned dTime); //TODO encoder's update must resolve last
};

