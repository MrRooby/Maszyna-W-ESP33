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
         * @brief Constructs a TwoDigitDisplay object using the specified NeoPixelBus strip, start index, and color.
         *
         * This constructor initializes a two-digit 7-segment display using the provided NeoPixelBus strip,
         * starting at the given index, and sets the initial color for the display segments.
         *
         * @param strip0 Pointer to the NeoPixelBus object controlling the LED strip (using RMT0 method).
         * @param startIndex The starting index on the LED strip where the two-digit display begins.
         * @param color The initial color to set for the display segments.
         */
        TwoDigitDisplay(NeoPixelBus<NeoGrbFeature, NeoEsp32Rmt0Ws2812xMethod>* strip0, int startIndex, RgbColor color);    

        /**
         * @brief Constructs a TwoDigitDisplay object using the specified NeoPixelBus strip, start index, and color.
         *
         * This constructor initializes a two-digit 7-segment display using the provided NeoPixelBus strip,
         * starting at the given index, and sets the initial color for the display segments.
         *
         * @param strip1 Pointer to the NeoPixelBus object controlling the LED strip (using RMT1 method).
         * @param startIndex The starting index on the LED strip where the two-digit display begins.
         * @param color The initial color to set for the display segments.
         */
        TwoDigitDisplay(NeoPixelBus<NeoGrbFeature, NeoEsp32Rmt1Ws2812xMethod>* strip1, int startIndex, RgbColor color);    

        /**
         * @brief Display a two-digit number
         * 
         * @param value Number to display (0-99)
         */
        void displayValue(int value);

        void loadingAnimation();

        /**
         * @brief Change color of both digits
         * 
         * @param color New color to set
         */
        virtual void changeColor(RgbColor color) override;
};