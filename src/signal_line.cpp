#include "signal_line.h"

SignalLine::SignalLine() 
 : LedElement() {}


SignalLine::SignalLine(CFastLED strip, CRGB *leds, int startIndex, int length, CRGB color)
    : LedElement(strip, leds, startIndex, color) {
    this->length = length;
}

void SignalLine::turnOnLine(bool choice) {
    if(choice){
        for(int i = 0; i < this->length; i++){
            this->leds[i + this->startIndex] = color;
        }
    }
    else{
        for(int i = 0; i < this->length; i++){
            this->leds[i + this->startIndex] = off;
        }
    }

    this->strip.show();
}