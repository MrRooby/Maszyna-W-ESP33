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
    static const bool segmentMap[10][7] = {
        {false, true,  true,  true,  true,  true,  true},  // 0
        {false, true,  false, false, false, false, true},  // 1
        {true,  true,  true,  false, true,  true,  false}, // 2
        {true,  true,  true,  false, false, true,  true},  // 3
        {false, true,  false, true,  false, false, true},  // 4
        {true,  false, true,  true,  false, true,  true},  // 5
        {true,  false, true,  true,  true,  true,  true},  // 6
        {false, true,  true,  false, false, false, true},  // 7
        {true,  true,  true,  true,  true,  true,  true},  // 8
        {true,  true,  true,  true,  false, true,  true}   // 9
    };

    if (number < 0 || number > 9)
        return;

    for (int i = 0; i < 7; ++i)
    {
        this->leds[startIndex + i] = segmentMap[number][i] ? segmentColor : off;
    }
}

void Segment::changeColor(CRGB color)
{
    this->segmentColor = color;
}
