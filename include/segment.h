#pragma once

#include <NeoPixelBus.h>
// #include <FastLED.h>
#include "led_element.h"

/**
 * @file segment.cpp
 * @brief Implementation of the Segment class for controlling a 7-segment LED display.
 * 
 * This file contains the implementation of the Segment class, which is used to control
 * a 7-segment LED display using the FastLED library. The class provides functionality
 * to display numbers from 0 to 9 on the LED segments.
 * 
 * @author Bartosz Faruga / MrRooby
 * @date 2025
 */
class Segment : public LedElement
{
private:
    int channel;

public:
    Segment();

    /**
     * @brief Construct a new Segment object
     *
     * @param strip Reference to a CFastLED object representing the LED strip
     * @param leds Pointer to the CRGB array representing the LED colors
     * @param startIndex The starting index of the segment in the LED strip
     * @param color The default color of the segment
     */
    Segment(NeoPixelBus<NeoGrbFeature, NeoEsp32Rmt0Ws2812xMethod>* strip0, int startIndex, RgbColor color);
    
    Segment(NeoPixelBus<NeoGrbFeature, NeoEsp32Rmt1Ws2812xMethod>* strip1, int startIndex, RgbColor color);

    /**
     * @brief Display selected number
     *
     * @param number number from 0 -> 9
     */
    void displayNumber(int number);
};