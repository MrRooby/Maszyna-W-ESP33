#include "signal_line.h"

SignalLine::SignalLine() 
 : LedElement() {}


SignalLine::SignalLine(NeoPixelBus<NeoGrbFeature, NeoEsp32Rmt0Ws2812xMethod>* strip0, int startIndex, int length, RgbColor color)
    : LedElement(strip0, startIndex, color) {
    this->length = length;
    this->channel = 0;
    this->pixelCount = length;
}

SignalLine::SignalLine(NeoPixelBus<NeoGrbFeature, NeoEsp32Rmt1Ws2812xMethod>* strip1, int startIndex, int length, RgbColor color)
    : LedElement(strip1, startIndex, color) {
    this->length = length;
    this->channel = 1;
    this->pixelCount = length;
}

void SignalLine::turnOnLine(bool choice) {
    RgbColor off(0, 0, 0);

    if(this->channel == 0){
        for (int i = 0; i < this->length; i++){
            this->strip0->SetPixelColor(this->startIndex + i, choice ? this->color : off);
        }
    }
    else if(this->channel == 1){
        for (int i = 0; i < this->length; i++){
            this->strip1->SetPixelColor(this->startIndex + i, choice ? this->color : off);
        }
    }
}
