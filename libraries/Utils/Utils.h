#pragma once
#include <Arduino.h>

enum Button{
  RIGHT,
  UP,
  DOWN,
  LEFT,
  SELECT,
  NONE = 1023
};

int GetButton(){
  int anal = analogRead(0);
  
  if (anal<60){return (int)Button::RIGHT;}
  if (anal<200){return (int)Button::UP;}
  if (anal<400){return (int)Button::DOWN;}
  if (anal<600){return (int)Button::LEFT;}
  if (anal<800){return (int)Button::SELECT;}
  return -1;
}
Button lastButton;
void SingleButtonAction(Button bt, void (*action)(void) ){
  if(bt != lastButton){
    action();
  }
}
void ReleaseButton(Button bt){
  lastButton = bt;
}

int Sign(int val){return val <0 ? -1 : 1;}