#include "segment.h"

Segment::Segment()
    : LedElement() {}


Segment::Segment(NeoPixelBus<NeoGrbFeature, NeoEsp32Rmt0Ws2812xMethod> *strip0, int startIndex, RgbColor color)
    : LedElement(strip0, startIndex, color) {
        this->channel = 0;
        this->pixelCount = 7;
    }

Segment::Segment(NeoPixelBus<NeoGrbFeature, NeoEsp32Rmt1Ws2812xMethod> *strip1, int startIndex, RgbColor color)
    : LedElement(strip1, startIndex, color) {
        this->channel = 1;
        this->pixelCount = 7;
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

    static const bool segmentMap[11][7] = {
        {false, true,  true,  true,  true,  true,  true},  // 0
        {false, true,  false, false, false, false, true},  // 1
        {true,  true,  true,  false, true,  true,  false}, // 2
        {true,  true,  true,  false, false, true,  true},  // 3
        {true, true,  false, true,  false, false, true},   // 4
        {true,  false, true,  true,  false, true,  true},  // 5
        {true,  false, true,  true,  true,  true,  true},  // 6
        {false, true,  true,  false, false, false, true},  // 7
        {true,  true,  true,  true,  true,  true,  true},  // 8
        {true,  true,  true,  true,  false, true,  true},  // 9
        {false,  false,  false,  false,  false, false,  false}   // OFF
    };

    if (number > 9)
        return;

    if (number < 0)
        number = 11;

    RgbColor off(0, 0, 0);

    if (this->channel == 0){
        for (int i = 0; i < 7; ++i){
            this->strip0->SetPixelColor(this->startIndex + i, segmentMap[number][i] ? this->color : off);
        }
    }
    else if (this->channel == 1){
        for (int i = 0; i < 7; ++i){
            this->strip1->SetPixelColor(this->startIndex + i, segmentMap[number][i] ? this->color : off);
        }
    }
}

void Segment::displayLetter(const char letter)
{
    static const std::map<char, std::array<bool, 7>> characterMap = {
        {'A', {true,  true,  true,  true,  true,  false, true}},   // A
        {'B', {true,  true,  true,  true,  true,  true,  true}},   // B (same as 8)
        {'C', {false, false, true,  true,  true,  true,  false}},  // C
        {'D', {false, true,  true,  true,  true,  true,  true}},   // D (same as 0)
        {'E', {true,  false, true,  true,  true,  true,  false}},  // E
        {'F', {true,  false, true,  true,  true,  false, false}},  // F
        {'G', {true,  true,  true,  true,  false, true,  true}},   // G (same as 9)
        {'H', {true,  true,  false, true,  true,  false, true}},   // H
        {'I', {false, true,  false, false, false, false, true}},   // I (same as 1)
        {'J', {false, true,  false, false, true,  true,  true}},   // J
        {'L', {false, false, false, true,  true,  true,  false}},  // L
        {'N', {true,  false, false, false, true,  false, true}},   // N (lowercase n)
        {'O', {false, true,  true,  true,  true,  true,  true}},   // O (same as 0)
        {'P', {true,  true,  true,  true,  true,  false, false}},  // P
        {'Q', {true,  true,  true,  true,  false, true,  true}},   // Q (same as 9)
        {'R', {true,  false, false, false, true,  false, false}},  // R (lowercase r)
        {'S', {true,  false, true,  true,  false, true,  true}},   // S (same as 5)
        {'T', {true,  false, false, true,  true,  true,  false}},  // T (lowercase t)
        {'U', {false, true,  false, true,  true,  true,  true}},   // U
        {'Y', {true,  true,  true,  true,  false, false, true}},   // Y
        {'Z', {true,  true,  true,  false, true,  true,  false}}   // Z (same as 2)
    };

    // Convert to uppercase
    char upperLetter = letter;
    if (letter >= 'a' && letter <= 'z') {
        upperLetter = letter - 32;
    }

    // Check if character exists in map
    auto it = characterMap.find(upperLetter);
    if (it == characterMap.end()) {
        // Invalid character - turn off all segments
        RgbColor off(0, 0, 0);
        if (this->channel == 0) {
            for (int i = 0; i < 7; ++i) {
                this->strip0->SetPixelColor(this->startIndex + i, off);
            }
        } else if (this->channel == 1) {
            for (int i = 0; i < 7; ++i) {
                this->strip1->SetPixelColor(this->startIndex + i, off);
            }
        }
        return;
    }

    RgbColor off(0, 0, 0);
    const std::array<bool, 7> segments = it->second;

    // Display the character
    if (this->channel == 0) {
        for (int i = 0; i < 7; ++i) {
            this->strip0->SetPixelColor(this->startIndex + i, 
                                       segments[i] ? this->color : off);
        }
    } else if (this->channel == 1) {
        for (int i = 0; i < 7; ++i) {
            this->strip1->SetPixelColor(this->startIndex + i, 
                                       segments[i] ? this->color : off);
        }
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

    for (int n = 0; n < 7; n++){
        if (this->channel == 0)
            this->strip0->SetPixelColor(this->startIndex + n, off);
        else if (this->channel == 1)
            this->strip1->SetPixelColor(this->startIndex + n, off);}

    for (int n = 0; n < 7; n++){
        if (loadingMap[this->currentFrame][n]){
            if (this->channel == 0)
                this->strip0->SetPixelColor(this->startIndex + n, this->color);
            else if (this->channel == 1)
                this->strip1->SetPixelColor(this->startIndex + n, this->color);
        }
    }

    this->currentFrame = (this->currentFrame + 1) % 6;
}