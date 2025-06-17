#include "two_digit_display.h"

TwoDigitDisplay::TwoDigitDisplay(NeoPixelBus<NeoGrbFeature, Neo800KbpsMethod>* strip, int startIndex, RgbColor color)
    : LedElement(strip, startIndex, color)
{
    this->display[0] = Segment(this->strip, this->startIndex, this->color);
    this->display[1] = Segment(this->strip, this->startIndex + 7, this->color);
}

void TwoDigitDisplay::displayValue(int value)
{
    if(value < 0){
        value = 0;
    }
    else if (value > 99){
        value = 99;
    }

    int tens = (value / 10) % 10;
    int ones = value % 10;

    this->display[0].displayNumber(tens);
    this->display[1].displayNumber(ones);
}


void TwoDigitDisplay::changeColor(RgbColor color)
{
    this->color = color;
    
    this->display[0].changeColor(color);
    this->display[1].changeColor(color);
}
