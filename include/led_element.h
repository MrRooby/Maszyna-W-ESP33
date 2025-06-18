#pragma once

// #include <FastLED.h>
#include <NeoPixelBus.h>

/**
 * @brief Base class for LED elements that can be controlled using FastLED
 * 
 * This class provides basic functionality for controlling LED elements,
 * including color management and position tracking in an LED strip.
 */
class LedElement {
    protected:
        NeoPixelBus<NeoGrbFeature, NeoEsp32Rmt0Ws2812xMethod>* strip0;
        NeoPixelBus<NeoGrbFeature, NeoEsp32Rmt1Ws2812xMethod>* strip1;

        int startIndex;
        RgbColor color;

    public:
        /**
         * @brief Default constructor
         */
        LedElement() = default;
        
        /**
         * @brief Construct a new Led Element
         * 
         * @param strip FastLED controller instance
         * @param leds Pointer to the LED strip array
         * @param startIndex Starting index in the LED strip
         * @param color Initial color of the LED element
         */
        LedElement(NeoPixelBus<NeoGrbFeature, NeoEsp32Rmt0Ws2812xMethod>* strip0, int startIndex, RgbColor color);
        
        LedElement(NeoPixelBus<NeoGrbFeature, NeoEsp32Rmt1Ws2812xMethod>* strip1, int startIndex, RgbColor color);
        
        /**
         * @brief Change the color of the LED element
         * 
         * @param color New color to set
         */
        virtual void changeColor(RgbColor color);
};