#pragma once

enum Button{
    RIGHT,
    UP,
    DOWN,
    LEFT,
    SELECT,
    NONE = 1023
  };


class Input
{
private:
    Button currentButton;
    Button lastButton;
    Button holdButton;
    double holdTime;
    Button GetButton();
    bool flipFlop;
public:
    Input();
    void Update(double dTime);

    bool WasBtnPressed(Button btn);
    bool IsBtnHeld(Button btn) const;
};

