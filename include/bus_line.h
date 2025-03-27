#pragma once

#include "led_element.h"

/**
 * @brief Class for controlling a line of LEDs representing a data bus
 * 
 * This class manages a sequence of LEDs that represent a data bus,
 * allowing them to be turned on/off together with animation effects.
 * 
 * @author Bartosz Faruga / MrRooby
 * @date 2025
 */
class BusLine : public LedElement {
    private:
        int length = 0;    ///< Number of LEDs in the bus line

    public:
        /**
         * @brief Default constructor
         * 
         * Initializes an empty bus line with zero length
         */
        BusLine();

        /**
         * @brief Construct a new Bus Line
         * 
         * @param strip FastLED controller instance
         * @param leds Pointer to LED array
         * @param startIndex Starting index in LED strip
         * @param length Number of LEDs in the bus
         * @param color Initial color for the bus
         */
        BusLine(CFastLED strip, CRGB *leds, int startIndex, int length, CRGB color);

        /**
         * @brief Turn the bus line on or off
         * 
         * @param choice true to turn on (set to color), false to turn off (set to black)
         */
        void turnOnLine(bool choice);
};