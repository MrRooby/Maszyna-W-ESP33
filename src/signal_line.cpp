#include "signal_line.h"

SignalLine::SignalLine() 
 : LedElement() {}


SignalLine::SignalLine(NeoPixelBus<NeoGrbFeature, Neo800KbpsMethod>* strip, int startIndex, int length, RgbColor color)
    : LedElement(strip, startIndex, color) {
    this->length = length;
}

void SignalLine::turnOnLine(bool choice) {
    RgbColor off(0, 0, 0);

        for(int i = 0; i < this->length; i++){
            this->strip->SetPixelColor(this->startIndex + i, choice ? this->color : off);
        }

    this->strip->Show();
}