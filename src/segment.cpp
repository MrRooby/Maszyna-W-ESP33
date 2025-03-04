#include "segment.h"

Segment::Segment(CRGB* leds, int numOfFirstLED, CRGB color)
{
    this->leds = leds;
    this->segmentColor = color;
    this->numOfFirstLED = numOfFirstLED;
}

void Segment::displayNumber(int number)
{
    switch (number)
    {
    case 0:
        this->leds[numOfFirstLED + 1] = segmentColor;
        this->leds[numOfFirstLED + 2] = segmentColor;
        this->leds[numOfFirstLED + 3] = segmentColor;
        this->leds[numOfFirstLED + 4] = segmentColor;
        this->leds[numOfFirstLED + 5] = segmentColor;
        this->leds[numOfFirstLED + 6] = segmentColor;
        break;

    case 1:
        this->leds[numOfFirstLED + 1] = segmentColor;
        this->leds[numOfFirstLED + 6] = segmentColor;
        break;

    case 2:
        this->leds[numOfFirstLED + 0] = segmentColor;
        this->leds[numOfFirstLED + 1] = segmentColor;
        this->leds[numOfFirstLED + 2] = segmentColor;
        this->leds[numOfFirstLED + 4] = segmentColor;
        this->leds[numOfFirstLED + 5] = segmentColor;
        break;

    case 3:
        this->leds[numOfFirstLED + 0] = segmentColor;
        this->leds[numOfFirstLED + 1] = segmentColor;
        this->leds[numOfFirstLED + 2] = segmentColor;
        this->leds[numOfFirstLED + 5] = segmentColor;
        this->leds[numOfFirstLED + 6] = segmentColor;
        break;

    case 4:
        this->leds[numOfFirstLED + 0] = segmentColor;
        this->leds[numOfFirstLED + 1] = segmentColor;
        this->leds[numOfFirstLED + 3] = segmentColor;
        this->leds[numOfFirstLED + 6] = segmentColor;
        break;

    case 5:
        this->leds[numOfFirstLED + 0] = segmentColor;
        this->leds[numOfFirstLED + 2] = segmentColor;
        this->leds[numOfFirstLED + 3] = segmentColor;
        this->leds[numOfFirstLED + 5] = segmentColor;
        this->leds[numOfFirstLED + 6] = segmentColor;
        break;

    case 6:
        this->leds[numOfFirstLED + 0] = segmentColor;
        this->leds[numOfFirstLED + 2] = segmentColor;
        this->leds[numOfFirstLED + 3] = segmentColor;
        this->leds[numOfFirstLED + 4] = segmentColor;
        this->leds[numOfFirstLED + 5] = segmentColor;
        this->leds[numOfFirstLED + 6] = segmentColor;
        break;

    case 7:
        this->leds[numOfFirstLED + 1] = segmentColor;
        this->leds[numOfFirstLED + 2] = segmentColor;
        this->leds[numOfFirstLED + 6] = segmentColor;
        break;
    
    case 8:
        for(int i = 0; i < 7; i++){
            this->leds[numOfFirstLED + i] = segmentColor;
        }
        break;

    case 9:
        this->leds[numOfFirstLED + 0] = segmentColor;
        this->leds[numOfFirstLED + 1] = segmentColor;
        this->leds[numOfFirstLED + 2] = segmentColor;
        this->leds[numOfFirstLED + 3] = segmentColor;
        this->leds[numOfFirstLED + 5] = segmentColor;
        this->leds[numOfFirstLED + 6] = segmentColor;
        break;

    default:
        break;
    }
}
