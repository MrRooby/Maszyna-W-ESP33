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
        int channel;
    public:
        /**
         * @brief Default constructor
         * 
         * Initializes an empty bus line with zero length
         */
        BusLine();

  
        /**
         * @brief Constructs a BusLine object for channel 0 using the specified NeoPixelBus strip.
         *
         * Initializes a BusLine instance that controls a segment of an LED strip using the NeoEsp32Rmt0Ws2812xMethod.
         *
         * @param strip0 Pointer to the NeoPixelBus object representing the LED strip (channel 0).
         * @param startIndex The starting index of the LED segment controlled by this BusLine.
         * @param length The number of LEDs in the segment.
         * @param color The color to be used for the LEDs in this BusLine.
         */
        BusLine(NeoPixelBus<NeoGrbFeature, NeoEsp32Rmt0Ws2812xMethod>* strip0, int startIndex, int length, RgbColor color);
        
        /**
         * @brief Constructs a BusLine object for channel 1 using the specified NeoPixelBus strip.
         *
         * Initializes a BusLine instance that controls a segment of an LED strip using the NeoEsp32Rmt0Ws2812xMethod.
         *
         * @param strip1 Pointer to the NeoPixelBus object representing the LED strip (channel 1).
         * @param startIndex The starting index of the LED segment controlled by this BusLine.
         * @param length The number of LEDs in the segment.
         * @param color The color to be used for the LEDs in this BusLine.
         */
        BusLine(NeoPixelBus<NeoGrbFeature, NeoEsp32Rmt1Ws2812xMethod>* strip1, int startIndex, int length, RgbColor color);

        /**
         * @brief Turn the bus line on or off
         * 
         * @param choice true to turn on (set to color), false to turn off (set to black)
         */
        void turnOnLine(bool choice);
};