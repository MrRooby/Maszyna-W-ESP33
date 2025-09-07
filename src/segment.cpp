#include "segment.h"

Segment::Segment()
    : LedElement() {}


Segment::Segment(NeoPixelBus<NeoGrbFeature, NeoEsp32Rmt0Ws2812xMethod> *strip0, int startIndex, RgbColor color)
    : LedElement(strip0, startIndex, color) {
        this->channel = 0;
    }

Segment::Segment(NeoPixelBus<NeoGrbFeature, NeoEsp32Rmt1Ws2812xMethod> *strip1, int startIndex, RgbColor color)
    : LedElement(strip1, startIndex, color) {
        this->channel = 1;
    }

void Segment::displayNumber(int number)
{
    /*
     --3--
    |     |
    4     2
    |     |
     --1--
    |     |
    5     7
    |     |
    --6--
    */

    static const bool segmentMap[10][7] = {
        {false, true,  true,  true,  true,  true,  true},  // 0
        {false, true,  false, false, false, false, true},  // 1
        {true,  true,  true,  false, true,  true,  false}, // 2
        {true,  true,  true,  false, false, true,  true},  // 3
        {true, true,  false, true,  false, false, true},   // 4
        {true,  false, true,  true,  false, true,  true},  // 5
        {true,  false, true,  true,  true,  true,  true},  // 6
        {false, true,  true,  false, false, false, true},  // 7
        {true,  true,  true,  true,  true,  true,  true},  // 8
        {true,  true,  true,  true,  false, true,  true}   // 9
    };

    if (number < 0 || number > 9)
        return;

    RgbColor off(0, 0, 0);

    if (this->channel == 0){
        for (int i = 0; i < 7; ++i){
            this->strip0->SetPixelColor(this->startIndex + i, segmentMap[number][i] ? this->color : off);
        }
        this->strip0->Show();
    }
    else if (this->channel == 1){
        for (int i = 0; i < 7; ++i){
            this->strip1->SetPixelColor(this->startIndex + i, segmentMap[number][i] ? this->color : off);
        }
        this->strip1->Show();
    }
}

void Segment::loadingAnimation(){
    static const bool loadingMap[6][7] = {
        {false, true, false, false, false, false, false},        
        {false, false, true, false, false, false, false},        
        {false, false, false, true, false, false, false},        
        {false, false, false, false, true, false, false},        
        {false, false, false, false, false, true, false},        
        {false, false, false, false, false, false, true},        
    };
    
    RgbColor off(0, 0, 0);
    unsigned long now = millis();

    // Use instance variables instead of static
    if (now - this->lastUpdate >= this->timeBetweenAnimationFramesMilliseconds) {
        // Clear all segments first
        for (int n = 0; n < 7; n++) {
            if (this->channel == 0) {
                this->strip0->SetPixelColor(this->startIndex + n, off);
            } else if (this->channel == 1) {
                this->strip1->SetPixelColor(this->startIndex + n, off);
            }
        }

        // Light up current frame segments
        for (int n = 0; n < 7; n++) {
            if (loadingMap[this->currentFrame][n]) {
                if (this->channel == 0) {
                    this->strip0->SetPixelColor(this->startIndex + n, this->color);
                } else if (this->channel == 1) {
                    this->strip1->SetPixelColor(this->startIndex + n, this->color);
                }
            }
        }

        // Show the changes
        if (this->channel == 0) {
            this->strip0->Show();
        } else if (this->channel == 1) {
            this->strip1->Show();
        }

        this->currentFrame = (this->currentFrame + 1) % 6;
        this->lastUpdate = now;
    }
}