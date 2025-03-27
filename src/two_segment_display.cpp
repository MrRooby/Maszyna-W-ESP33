#include "two_segment_display.h"

TwoSegmentDisplay::TwoSegmentDisplay(CFastLED strip, CRGB *leds, int startIndex, CRGB color)
    : LedElement(strip, leds, startIndex, color)
{
    this->display[0] = Segment(this->strip, this->leds, this->startIndex, this->color);
    this->display[1] = Segment(this->strip, this->leds, this->startIndex + 7, this->color);
}

void TwoSegmentDisplay::displayValue(int value)
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


void TwoSegmentDisplay::changeColor(CRGB color)
{
    this->display[0].changeColor(color);
    this->display[1].changeColor(color);
}
