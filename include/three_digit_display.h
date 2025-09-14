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
         * @brief Constructs a ThreeDigitDisplay object using channel 0 (NeoEsp32Rmt0Ws2812xMethod).
         * 
         * This constructor initializes a three-digit 7-segment display using the specified NeoPixelBus strip,
         * starting LED index, and color. Each digit is represented by a Segment object, and the display will
         * use the provided color for all segments.
         * 
         * @param strip0 Pointer to the NeoPixelBus<NeoGrbFeature, NeoEsp32Rmt0Ws2812xMethod> controlling the LEDs (channel 0).
         * @param startIndex The starting index of the first LED for the display on the strip.
         * @param color The color to use for the digits (as an RgbColor).
         */
        ThreeDigitDisplay(NeoPixelBus<NeoGrbFeature, NeoEsp32Rmt0Ws2812xMethod>* strip1, int startIndex, RgbColor color);    
        
        /**
         * @brief Constructs a ThreeDigitDisplay object using channel 1 (NeoEsp32Rmt0Ws2812xMethod).
         * 
         * This constructor initializes a three-digit 7-segment display using the specified NeoPixelBus strip,
         * starting LED index, and color. Each digit is represented by a Segment object, and the display will
         * use the provided color for all segments.
         * 
         * @param strip1 Pointer to the NeoPixelBus<NeoGrbFeature, NeoEsp32Rmt0Ws2812xMethod> controlling the LEDs (channel 1).
         * @param startIndex The starting index of the first LED for the display on the strip.
         * @param color The color to use for the digits (as an RgbColor).
         */
        ThreeDigitDisplay(NeoPixelBus<NeoGrbFeature, NeoEsp32Rmt1Ws2812xMethod>* strip1, int startIndex, RgbColor color);    
        
        /**
         * @brief Display a three-digit number
         * 
         * @param value Number to display (0-999)
         */
        void displayValue(int value);

        void loadingAnimation();

        /**
         * @brief Change color of all three digits
         * 
         * @param color New color to set
         */
        virtual void setColor(RgbColor color) override;
};