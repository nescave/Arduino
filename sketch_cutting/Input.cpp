#include "Input.h"
#include "Arduino.h"

Input::Input() :
    lastButton(NONE),
    holdButton(NONE),
    holdTime(0),
    currentButton(NONE),
    flipFlop(false)
{}

void Input::Update(double dTime)
{
    // Serial.println(dTime);
    if(currentButton == lastButton)
    {
        holdButton = currentButton;
        holdTime += dTime;
    }else
    {
        holdButton = NONE;
        holdTime = 0;
    }
    if(flipFlop) currentButton = GetButton();
    else lastButton = currentButton;
    flipFlop = !flipFlop;
}

bool Input::WasBtnPressed(Button btn)
{
    if(currentButton == Button::NONE) return false;
    if(currentButton == btn && currentButton != lastButton) return true;
    return false;
}

bool Input::IsBtnHeld(Button btn) const
{
    if(holdButton == btn)
    {
    }
    if(holdButton != NONE && holdButton == btn && holdTime > 750)
    {
        return true;
    }
 
    return false;
}

Button Input::GetButton(){
    const int anal = analogRead(0);

    if (anal<60)return Button::RIGHT;
    if (anal<200)return Button::UP;
    if (anal<400)return Button::DOWN;
    if (anal<600)return Button::LEFT;
    if (anal<800)return Button::SELECT;
    return Button::NONE;
}
