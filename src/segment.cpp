#include "segment.h"

Segment::Segment()
    : LedElement() {}


Segment::Segment(NeoPixelBus<NeoGrbFeature, Neo800KbpsMethod> *strip, int startIndex, RgbColor color)
    : LedElement(strip, startIndex, color) {}


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

    RgbColor off(0, 0, 0);

    for (int i = 0; i < 7; ++i)
    {
        this->strip->SetPixelColor(this->startIndex + i, segmentMap[number][i] ? this->color : off);
    }

    this->strip->Show();
}