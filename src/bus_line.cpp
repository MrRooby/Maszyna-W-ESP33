#include "bus_line.h"

BusLine::BusLine()
    : LedElement(), length(0) {}


BusLine::BusLine(NeoPixelBus<NeoGrbFeature, Neo800KbpsMethod>* strip, int startIndex, int length, RgbColor color)
    : LedElement(strip, startIndex, color)
{
    this->length = length;
}


void BusLine::turnOnLine(bool choice) {
    RgbColor off(0, 0, 0);
    for(int i = 0; i < this->length; i++){
        this->strip->SetPixelColor(this->startIndex + i, choice ? this->color : off);
    }
    this->strip->Show();
}
