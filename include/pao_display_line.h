#pragma once

#include <FastLED.h>
#include "three_digit_display.h"
#include "two_digit_display.h"
#include "led_element.h"

/**
 * @brief Class for controlling a Program-Address-Operand display line
 * 
 * This class manages three separate displays to show:
 * - Two-digit address display
 * - Three-digit value display
 * - Two-digit argument display
 * 
 * @author Bartosz Faruga / MrRooby
 * @date 2025
 */
class PaODisplayLine: protected LedElement {
    public:
        
        TwoDigitDisplay *addr = nullptr;    ///< Address display (2 digits)
        ThreeDigitDisplay *val = nullptr;   ///< Value display (3 digits)
        TwoDigitDisplay *arg = nullptr;     ///< Argument display (2 digits)

        /**
         * @brief Construct a new PaO Display Line
         * 
         * @param strip FastLED controller instance
         * @param leds Pointer to LED array
         * @param startIndex Starting index in LED strip
         * @param color Initial color for all displays
         */
        PaODisplayLine(CFastLED strip, CRGB *leds, int startIndex, CRGB color);    

        /**
         * @brief Display values on all three displays
         * 
         * @param addr Address value (0-99)
         * @param val Main value (0-999)
         * @param arg Argument value (0-99)
         */
        void displayLine(int addr, int val, int arg);

        /**
         * @brief Change color of all displays
         * 
         * @param color New color to set
         */
        void changeColor(CRGB color);
};