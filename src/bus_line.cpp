#include "bus_line.h"

BusLine::BusLine()
    : LedElement(), length(0) {}


BusLine::BusLine(NeoPixelBus<NeoGrbFeature, NeoEsp32Rmt0Ws2812xMethod>* strip0, int startIndex, int length, RgbColor color)
    : LedElement(strip0, startIndex, color)
{
    this->length = length;
    this->channel = 0;
}

BusLine::BusLine(NeoPixelBus<NeoGrbFeature, NeoEsp32Rmt1Ws2812xMethod>* strip1, int startIndex, int length, RgbColor color)
    : LedElement(strip1, startIndex, color)
{
    this->length = length;
    this->channel = 1;
}

void BusLine::turnOnLine(bool choice) {
    RgbColor off(0, 0, 0);
    
    if(this->channel == 0){
        for (int i = 0; i < this->length; i++){
            this->strip0->SetPixelColor(this->startIndex + i, choice ? this->color : off);
        }

        this->strip0->Show();
    }
    else if(this->channel == 1){
        for (int i = 0; i < this->length; i++){
            this->strip1->SetPixelColor(this->startIndex + i, choice ? this->color : off);
        }

        this->strip1->Show();
    }
}
