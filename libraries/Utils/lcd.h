#pragma once
#include <LiquidCrystal.h>
#include <string.h>

LiquidCrystal lcd( 8,  9,  4,  5, 6,  7);

void LCDPrint(unsigned short x, unsigned short y, const char* cont, unsigned short len ){
    lcd.setCursor(x,y);
    for(int i = 0; i<=len; i++){
        lcd.print(" ");
    }
    lcd.setCursor(x,y);
    lcd.print(cont);
}
//test comment
void LCDPrint(unsigned short x, unsigned short y, String cont){
    LCDPrint(x,y,cont.c_str(), cont.length());
}

void LCDPrint(unsigned short x, unsigned short y, long cont){
    String str = "";
    str += cont;
    LCDPrint(x,y, str.c_str(), str.length());
}
void LCDPrint(unsigned short x, unsigned short y, unsigned cont){
    String str = "";
    str += cont;
    LCDPrint(x,y, str.c_str(), str.length());
}
void LCDPrint(unsigned short x, unsigned short y, int cont){
    String str = "";
    str += cont;
    LCDPrint(x,y, str.c_str(), str.length());
}
void LCDPrint(unsigned short x, unsigned short y, float cont){
    String str = "";
    str += cont;
    LCDPrint(x,y, str.c_str(), str.length());
}