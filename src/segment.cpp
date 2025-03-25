#include "segment.h"

Segment::Segment() : leds(nullptr), startIndex(0), segmentColor(CRGB::Black){}

Segment::Segment(CRGB *leds, int startIndex, CRGB color)
{
    this->leds = leds;
    this->segmentColor = color;
    this->startIndex = startIndex;
}

void Segment::displayNumber(int number)
{
    switch (number)
    {
    case 0:
        this->leds[startIndex + 1] = segmentColor;
        this->leds[startIndex + 2] = segmentColor;
        this->leds[startIndex + 3] = segmentColor;
        this->leds[startIndex + 4] = segmentColor;
        this->leds[startIndex + 5] = segmentColor;
        this->leds[startIndex + 6] = segmentColor;
        break;

    case 1:
        this->leds[startIndex + 1] = segmentColor;
        this->leds[startIndex + 6] = segmentColor;
        break;

    case 2:
        this->leds[startIndex + 0] = segmentColor;
        this->leds[startIndex + 1] = segmentColor;
        this->leds[startIndex + 2] = segmentColor;
        this->leds[startIndex + 4] = segmentColor;
        this->leds[startIndex + 5] = segmentColor;
        break;

    case 3:
        this->leds[startIndex + 0] = segmentColor;
        this->leds[startIndex + 1] = segmentColor;
        this->leds[startIndex + 2] = segmentColor;
        this->leds[startIndex + 5] = segmentColor;
        this->leds[startIndex + 6] = segmentColor;
        break;

    case 4:
        this->leds[startIndex + 0] = segmentColor;
        this->leds[startIndex + 1] = segmentColor;
        this->leds[startIndex + 3] = segmentColor;
        this->leds[startIndex + 6] = segmentColor;
        break;

    case 5:
        this->leds[startIndex + 0] = segmentColor;
        this->leds[startIndex + 2] = segmentColor;
        this->leds[startIndex + 3] = segmentColor;
        this->leds[startIndex + 5] = segmentColor;
        this->leds[startIndex + 6] = segmentColor;
        break;

    case 6:
        this->leds[startIndex + 0] = segmentColor;
        this->leds[startIndex + 2] = segmentColor;
        this->leds[startIndex + 3] = segmentColor;
        this->leds[startIndex + 4] = segmentColor;
        this->leds[startIndex + 5] = segmentColor;
        this->leds[startIndex + 6] = segmentColor;
        break;

    case 7:
        this->leds[startIndex + 1] = segmentColor;
        this->leds[startIndex + 2] = segmentColor;
        this->leds[startIndex + 6] = segmentColor;
        break;
    
    case 8:
        for(int i = 0; i < 7; i++){
            this->leds[startIndex + i] = segmentColor;
        }
        break;

    case 9:
        this->leds[startIndex + 0] = segmentColor;
        this->leds[startIndex + 1] = segmentColor;
        this->leds[startIndex + 2] = segmentColor;
        this->leds[startIndex + 3] = segmentColor;
        this->leds[startIndex + 5] = segmentColor;
        this->leds[startIndex + 6] = segmentColor;
        break;

    default:
        break;
    }
}

void Segment::changeColor(CRGB color)
{
    this->segmentColor = color;
}
