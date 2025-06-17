#pragma once

#include "segment.h"
#include "led_element.h"

/**
 * @brief Class for controlling a two-digit seven-segment display
 * 
 * This class manages two seven-segment displays to show two-digit numbers.
 * Each digit is controlled independently using the Segment class.
 * 
 * @author Bartosz Faruga / MrRooby
 * @date 2025
 */
class TwoDigitDisplay : protected LedElement {
    private:
        Segment display[2];    ///< Array of two seven-segment displays

    public:
        /**
         * @brief Construct a new Two Digit Display
         * 
         * @param fastLED FastLED controller instance
         * @param leds Pointer to LED array
         * @param startIndex Starting index in LED strip
         * @param color Initial color for both digits
         */
        TwoDigitDisplay(NeoPixelBus<NeoGrbFeature, Neo800KbpsMethod>* strip, int startIndex, RgbColor color);    

        /**
         * @brief Display a two-digit number
         * 
         * @param value Number to display (0-99)
         */
        void displayValue(int value);

        /**
         * @brief Change color of both digits
         * 
         * @param color New color to set
         */
        virtual void changeColor(RgbColor color) override;
};