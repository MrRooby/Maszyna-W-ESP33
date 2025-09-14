#pragma once

#include <NeoPixelBus.h>
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

    int currentFrame = 0;

public:
    Segment();

    /**
     * @brief Constructs a Segment object for a 7-segment display using the specified NeoPixelBus strip (channel 0).
     *
     * This constructor initializes the Segment to use the provided NeoPixelBus strip (configured for channel 0),
     * sets the starting index for the segment's LEDs, and assigns the color to be used for illumination.
     *
     * @param strip0 Pointer to the NeoPixelBus object configured with NeoEsp32Rmt1Ws2812xMethod (channel 0).
     * @param startIndex The index of the first LED in the segment on the strip.
     * @param color The RgbColor to use for the segment when illuminated.
     */
    Segment(NeoPixelBus<NeoGrbFeature, NeoEsp32Rmt0Ws2812xMethod>* strip0, int startIndex, RgbColor color);
    
    /**
     * @brief Constructs a Segment object for a 7-segment display using the specified NeoPixelBus strip (channel 1).
     *
     * This constructor initializes the Segment to use the provided NeoPixelBus strip (configured for channel 1),
     * sets the starting index for the segment's LEDs, and assigns the color to be used for illumination.
     *
     * @param strip1 Pointer to the NeoPixelBus object configured with NeoEsp32Rmt1Ws2812xMethod (channel 1).
     * @param startIndex The index of the first LED in the segment on the strip.
     * @param color The RgbColor to use for the segment when illuminated.
     */
    Segment(NeoPixelBus<NeoGrbFeature, NeoEsp32Rmt1Ws2812xMethod>* strip1, int startIndex, RgbColor color);

    /**
     * @brief Display selected number
     *
     * @param number number from 0 -> 9
     */
    void displayNumber(int number);

    void loadingAnimation();
};