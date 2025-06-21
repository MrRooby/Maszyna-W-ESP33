#pragma once

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
        int channel;
    public:
        /**
         * @brief Default constructor
         */
        SignalLine();

        /**
         * @brief Constructs a SignalLine object for a specific NeoPixelBus strip using RMT channel 0.
         * 
         * This constructor initializes a SignalLine that controls a segment of LEDs on the specified strip,
         * starting at the given index and spanning the specified length, with the provided color.
         * 
         * @param strip0 Pointer to the NeoPixelBus object using NeoEsp32Rmt0Ws2812xMethod (RMT channel 0).
         * @param startIndex The starting index of the LED segment on the strip.
         * @param length The number of LEDs in the signal line.
         * @param color The color to set for the signal line (as an RgbColor object).
         */
        SignalLine(NeoPixelBus<NeoGrbFeature, NeoEsp32Rmt0Ws2812xMethod>* strip0, int startIndex, int length, RgbColor color);
        
        /**
         * @brief Constructs a SignalLine object for a specific NeoPixelBus strip using RMT channel 1.
         * 
         * This constructor initializes a SignalLine that controls a segment of LEDs on the specified strip,
         * starting at the given index and spanning the specified length, with the provided color.
         * 
         * @param strip1 Pointer to the NeoPixelBus object using NeoEsp32Rmt0Ws2812xMethod (RMT channel 1).
         * @param startIndex The starting index of the LED segment on the strip.
         * @param length The number of LEDs in the signal line.
         * @param color The color to set for the signal line (as an RgbColor object).
         */
        SignalLine(NeoPixelBus<NeoGrbFeature, NeoEsp32Rmt1Ws2812xMethod>* strip1, int startIndex, int length, RgbColor color);

        /**
         * @brief Turn the signal line on or off
         * 
         * @param choice true to turn on (set to color), false to turn off (set to black)
         */
        void turnOnLine(bool choice);
};