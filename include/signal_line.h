#pragma once

#include <FastLED.h>
#include "led_element.h"

/**
 * @brief Class representing a line of LEDs that can be turned on/off as a signal
 * 
 * This class manages a sequence of LEDs that act as a signal line,
 * allowing them to be turned on or off together with the same color.
 * 
 * @author Bartosz Faruga / MrRooby
 * @date 2025
 */
class SignalLine : protected LedElement {
    private:
        int length = 0;     ///< Number of LEDs in the signal line

    public:
        /**
         * @brief Default constructor
         */
        SignalLine();

        /**
         * @brief Construct a new Signal Line
         * 
         * @param strip FastLED controller instance
         * @param leds Pointer to the LED strip array
         * @param startIndex Starting index in the LED strip
         * @param length Number of LEDs in the signal line
         * @param color Initial color of the signal line
         */
        SignalLine(CFastLED strip, CRGB *leds, int startIndex, int length, CRGB color);

        /**
         * @brief Turn the signal line on or off
         * 
         * @param choice true to turn on (set to color), false to turn off (set to black)
         */
        void turnOnLine(bool choice);
};