#pragma once

#include "led_element.h"

/**
 * @file signal_line.h
 * @brief LED signal line controller for visual signal indication
 * 
 * Provides control for a contiguous line of LEDs that represent a machine signal line.
 * The entire line can be turned on/off simultaneously with a uniform color.
 * 
 * @author Bartosz Faruga / MrRooby
 * @date 2025
 */
class SignalLine : public LedElement {
    private:
        int length = 0;     ///< Number of LEDs in the signal line
        int channel;
    public:
        /**
         * @brief Default constructor
         * 
         * Initializes an empty SignalLine with no strip assigned.
         * Must be configured via parametrized constructors before use.
         */
        SignalLine();

        /**
         * @brief Construct a SignalLine for Channel 0 (RMT0)
         * 
         * Initializes a signal line that controls a contiguous segment of LEDs on the specified
         * strip using the NeoEsp32Rmt0Ws2812xMethod (RMT channel 0) of the ESP32.
         * 
         * @param strip0 Pointer to the NeoPixelBus object configured for RMT channel 0.
         *               Must not be nullptr.
         * @param startIndex The starting index of the first LED in the signal line.
         *                   Should be >= 0 and < total number of pixels.
         * @param length The number of consecutive LEDs to include in this signal line.
         *               Ensure startIndex + length does not exceed total strip LEDs.
         * @param color The RgbColor to display when the signal is on.
         *              Can be changed later with setColor().
         * 
         * @note All LEDs in the signal line display the same color when active.
         * @see turnOnLine()
         * @see setColor()
         */
        SignalLine(NeoPixelBus<NeoGrbFeature, NeoEsp32Rmt0Ws2812xMethod>* strip0, int startIndex, int length, RgbColor color);
        
        /**
         * @brief Construct a SignalLine for Channel 1 (RMT1)
         * 
         * Initializes a signal line that controls a contiguous segment of LEDs on the specified
         * strip using the NeoEsp32Rmt1Ws2812xMethod (RMT channel 1) of the ESP32.
         * 
         * @param strip1 Pointer to the NeoPixelBus object configured for RMT channel 1.
         *               Must not be nullptr.
         * @param startIndex The starting index of the first LED in the signal line.
         *                   Should be >= 0 and < total number of pixels.
         * @param length The number of consecutive LEDs to include in this signal line.
         *               Ensure startIndex + length does not exceed total strip LEDs.
         * @param color The RgbColor to display when the signal is on.
         *              Can be changed later with setColor().
         * 
         * @note All LEDs in the signal line display the same color when active.
         * @see turnOnLine()
         * @see setColor()
         */
        SignalLine(NeoPixelBus<NeoGrbFeature, NeoEsp32Rmt1Ws2812xMethod>* strip1, int startIndex, int length, RgbColor color);

        /**
         * @brief Turn the signal line on or off
         * 
         * Sets all LEDs in the signal line to either the configured color (on) or black/off (off).
         * Provides binary signal indication (active/inactive state) for machine signals.
         * 
         * @param choice true to turn on (set all LEDs to current color),
         *               false to turn off (set all LEDs to black/RgbColor(0,0,0))
         * 
         * @note The change is applied immediately to the LED strip.
         * @see setColor()
         */
        void turnOnLine(bool choice);
};