#pragma once

#include <FastLED.h>

/**
 * @brief Base class for LED elements that can be controlled using FastLED
 * 
 * This class provides basic functionality for controlling LED elements,
 * including color management and position tracking in an LED strip.
 */
class LedElement {
    protected:
        int startIndex;         ///< Starting index in the LED strip
        CRGB *leds;             ///< Pointer to the LED strip array
        CRGB color = CRGB::Red; ///< Current color of the LED element
        CRGB off = CRGB::Black; ///< Color used when LED is turned off
        CFastLED strip;         ///< FastLED controller instance

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
        LedElement(CFastLED strip, CRGB *leds, int startIndex, CRGB color);

        /**
         * @brief Change the color of the LED element
         * 
         * @param color New color to set
         */
        virtual void changeColor(CRGB color);
};