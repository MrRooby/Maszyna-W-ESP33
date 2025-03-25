#include "three_segment_display.h"

ThreeSegmentDisplay::ThreeSegmentDisplay(CFastLED strip, CRGB *leds, int startIndex, CRGB color)
{
    this->strip = strip;
    this->leds = leds;
    this->color = color;

    this->display[0] = Segment(this->leds, startIndex, this->color);
    this->display[1] = Segment(this->leds, startIndex + 7, this->color);
    this->display[2] = Segment(this->leds, startIndex + 14, this->color);
}

void ThreeSegmentDisplay::displayValue(int value)
{
    if(value < 0){
        value = 0;
    }
    else if (value > 999){
        value = 999;
    }

    int huns = (value / 100) % 10;
    int tens = (value / 10) % 10;
    int ones = value % 10;

    this->display[0].displayNumber(huns);
    this->display[1].displayNumber(tens);
    this->display[2].displayNumber(ones);

    // this->strip.show();
}

void ThreeSegmentDisplay::changeColor(CRGB color)
{
    this->color = color;

    this->display[0].changeColor(this->color);
    this->display[1].changeColor(this->color);
    this->display[2].changeColor(this->color);
}
