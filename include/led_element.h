#pragma once

#include <NeoPixelBus.h>

/**
 * @brief Base class for LED elements that can be controlled using NeoPixelBus
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
         * @brief Construct a new Led Element for Channel 0
         * 
         * @param strip0 NeoPixelBus controller instance for Channel 0
         * @param startIndex Starting index in the LED strip
         * @param color Initial color of the LED element
         */
        LedElement(NeoPixelBus<NeoGrbFeature, NeoEsp32Rmt0Ws2812xMethod>* strip0, int startIndex, RgbColor color);
        
        /**
         * @brief Construct a new Led Element for Channel 1
         * 
         * @param strip1 NeoPixelBus controller instance for Channel 1
         * @param startIndex Starting index in the LED strip
         * @param color Initial color of the LED element
         */
        LedElement(NeoPixelBus<NeoGrbFeature, NeoEsp32Rmt1Ws2812xMethod>* strip1, int startIndex, RgbColor color);
        
        /**
         * @brief Change the color of the LED element
         * 
         * @param color New color to set
         */
        virtual void changeColor(RgbColor color);
};