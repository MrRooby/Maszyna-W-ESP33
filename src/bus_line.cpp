#include "bus_line.h"

BusLine::BusLine()
    : LedElement(), length(0) {}


BusLine::BusLine(CFastLED strip, CRGB *leds, int startIndex, int length, CRGB color)
    : LedElement(strip, leds, startIndex, color)
{
    this->length = length;
}


void BusLine::turnOnLine(bool choice) {
    // while (choice) {
    //     for (int i = 0; i < length; ++i) {
    //         leds[startIndex + i] = color;
    //         this->strip.show();
    //         delay(100); // Adjust delay for desired speed
    //         leds[startIndex + i] = CRGB::Black;

    //         // Check if choice has changed during the animation
    //         if (!choice) {
    //             break;
    //         }
    //     }
    // }

    // // Turn off all LEDs when choice is false
    // for (int i = 0; i < length; ++i) {
    //     leds[startIndex + i] = CRGB::Black;
    // }
    // this->strip.show();

    // Temporary implementation before issue with system crash is resolved
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
