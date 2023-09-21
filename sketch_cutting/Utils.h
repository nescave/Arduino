// #pragma once
// #include <Arduino.h>
//
// class Screen
// {
//     
// public:
//   LiquidCrystal lcd;
//   Screen(
//       const int pinA,
//       const int pinB,
//       const int pinC,
//       const int pinD,
//       const int pinE,
//       const int pinF,
//       const int width,
//       const int height) :
//     lcd(LiquidCrystal(pinA, pinB, pinC, pinD, pinE, pinF))
//   {
//     // lcd = LiquidCrystal();
//     lcd.begin(width, height);
//   }
//   void Print(unsigned short x, unsigned short y, const char* cont, unsigned short len ){
//     this->lcd.setCursor(x,y);
//     for(int i = 0; i<=len; i++){
//       lcd.print(" ");
//     }
//     lcd.setCursor(x,y);
//     lcd.print(cont);
//   }
//   void Print(unsigned short x, unsigned short y, String cont) {
//     Print(x,y,cont.c_str(), cont.length());
//   }
//
//   void Print(unsigned short x, unsigned short y, long cont) {
//     String str = "";
//     str += cont;
//     Print(x,y, str.c_str(), str.length());
//   }
//   void Print(unsigned short x, unsigned short y, unsigned cont) {
//     String str = "";
//     str += cont;
//     Print(x,y, str.c_str(), str.length());
//   }
//   void Print(unsigned short x, unsigned short y, int cont) {
//     String str = "";
//     str += cont;
//     Print(x,y, str.c_str(), str.length());
//   }
//   void Print(unsigned short x, unsigned short y, float cont) {
//     String str = "";
//     str += cont;
//     Print(x,y, str.c_str(), str.length());
//   }
// };
