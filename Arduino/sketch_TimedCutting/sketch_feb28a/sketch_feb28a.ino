// #include <AccelStepper.h>
// #include <MultiStepper.h>
// #pragma once
#include <LiquidCrystal.h>

const int APIN = 2;
const int BPIN = 3;

const int SWPIN = 12;
const int LEDPIN = 13;

const unsigned maxLength = 600;
const unsigned minLength = 120;

int a = 0;
int b = 0;

int count = 0;
double distanceTraveled = 0;

unsigned short mmDiameter = 85;
LiquidCrystal lcd( 8,  9,  4,  5, 6,  7);
  
unsigned mmLength = 330;
double GetDistancePerStep(){
  return mmDiameter * 3.14159265 / 400.0;
}

void TickA(){
  a = digitalRead(APIN);
  b = digitalRead(BPIN);
  if(a == 1 && b == 0){
    count++;
    distanceTraveled += GetDistancePerStep();
    Serial.println(count);
  }
}
void TickB(){
  a = digitalRead(APIN);
  b = digitalRead(BPIN);
  if (a==0 && b == 1){
    count--;
    distanceTraveled -= GetDistancePerStep();
    Serial.println(count);
  }
}

unsigned long lastTime;
unsigned long timer1 = 0;
unsigned long timer2 = 0;

void AccTime(unsigned long* timer){
  *timer += millis() - lastTime;
}

void StartCutting(){
  digitalWrite(LEDPIN, HIGH);
  AccTime(&timer1);
  if (timer1 >= 4500){
    count -= (int)ceil(mmLength/ GetDistancePerStep());
    distanceTraveled -= mmLength;
    Serial.println(count);

    timer1 = 0;
    digitalWrite(LEDPIN, LOW);
  }
}

void AdjustValue(int bt, unsigned* value){
  
  switch(bt){
    case (Button::RIGHT):
      if(mmLength +1 <= maxLength) *value+=1;
      break;
   case (Button::LEFT):
      if(mmLength -1 >= minLength) *value-=1;
      break;
  }
}
String currentString; 
unsigned* currentValue;
short valueSelector = -1;
void NextValue(){
  valueSelector++;
  if (valueSelector >1) valueSelector = 0;
  switch(valueSelector){
    case 0:
      currentString = "Dlugosc wezy:";
      currentValue = &mmLength;
      break;
    case 1:
      currentString = "Srednica:";
      currentValue = &mmDiameter;
      break;
  }
}
void setup() {
  pinMode(SWPIN, INPUT_PULLUP);
  pinMode(APIN, INPUT_PULLUP);
  pinMode(BPIN, INPUT_PULLUP);

  digitalWrite(SWPIN, HIGH);
  digitalWrite(APIN, HIGH);
  digitalWrite(BPIN, HIGH);

  attachInterrupt(digitalPinToInterrupt(APIN), TickA, RISING);
  attachInterrupt(digitalPinToInterrupt(BPIN), TickB, RISING);
  Serial.begin(9600);

  lastTime = micros();

  lcd.begin(16, 2);
  
  NextValue();
}

Button lastButton = 1023;
void loop() {

  Button bt = GetButton();
  
  if(bt != lastButton){
    
    if(bt == Button::SELECT){
      NextValue();
    }else if(bt == Button::UP){
      count += mmLength;
      Serial.println(count);

    }else{
      AdjustValue(bt, currentValue);
    }

    timer2 = 0;
  }else if(bt != Button::NONE){
    AccTime(&timer2);

    if(timer2 > 1000){
      AdjustValue(bt, currentValue);
    }
  }
  
  String abc = currentString.c_str();
  abc += *currentValue;
  lcd.setCursor(0,0);
  lcd.clear();
  lcd.print(abc);

  lcd.setCursor(0, 1);
  lcd.print(GetDistancePerStep());
  lcd.setCursor(5, 1);
  lcd.print(timer1);
  lcd.setCursor(9, 1);
  lcd.print(distanceTraveled);

  if(distanceTraveled >= mmLength){
    StartCutting();
  }

  lastButton = bt;
  lastTime = millis();
  delay(50);
}
