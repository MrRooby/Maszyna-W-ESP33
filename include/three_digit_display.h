#pragma once

#include "segment.h"
#include "led_element.h"

/**
 * @brief Class for controlling a three-digit seven-segment display
 * 
 * This class manages three seven-segment displays to show three-digit numbers.
 * Each digit is controlled independently using the Segment class.
 * 
 * @author Bartosz Faruga / MrRooby
 * @date 2025
 */
class ThreeDigitDisplay : protected LedElement {
    private:
        Segment display[3];    ///< Array of three seven-segment displays

    public:
        /**
         * @brief Construct a new Three Digit Display
         * 
         * @param fastLED FastLED controller instance
         * @param leds Pointer to LED array
         * @param startIndex Starting index in LED strip
         * @param color Initial color for all digits
         */
        ThreeDigitDisplay(NeoPixelBus<NeoGrbFeature, Neo800KbpsMethod>* strip, int startIndex, RgbColor color);    
        /**
         * @brief Display a three-digit number
         * 
         * @param value Number to display (0-999)
         */
        void displayValue(int value);

        /**
         * @brief Change color of all three digits
         * 
         * @param color New color to set
         */
        virtual void changeColor(RgbColor color) override;
};