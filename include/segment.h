#pragma once

#include <NeoPixelBus.h>
#include "led_element.h"

/**
 * @file segment.h
 * @brief 7-segment LED display controller using NeoPixelBus
 * 
 * Provides control for individual 7-segment LED displays within NeoPixel LED strips.
 * Supports displaying digits 0-9 with customizable colors and animation effects.
 * Each segment is independently addressable on the LED strip.
 * 
 * Segment Layout:
 * ``` 
 *    --3--
 *   |     |
 *   4     2
 *   |     |
 *    --1--
 *   |     |
 *   5     7
 *   |     |
 *    --6--
 * ```
 * @author Bartosz Faruga / MrRooby
 * @date 2025
 */
class Segment : public LedElement
{
private:
    int channel;

    int currentFrame = 0;

public:
    /**
     * @brief Default constructor
     * 
     * Initializes an empty Segment with no strip assigned.
     * Must be configured via parametrized constructors before use.
     */
    Segment();

    /**
     * @brief Construct a Segment for Channel 0 (RMT0)
     *
     * Initializes a 7-segment display that controls seven LEDs using the NeoEsp32Rmt0Ws2812xMethod
     * (RMT channel 0) of the ESP32.
     *
     * @param strip0 Pointer to the NeoPixelBus object configured for RMT channel 0.
     *               Must not be nullptr.
     * @param startIndex The starting index of the first LED (segment 'a') in the strip.
     *                   The segment uses 7 consecutive LEDs starting from this index.
     * @param color The RgbColor to use for illuminating the segment.
     *              Can be changed later with setColor().
     * 
     * @note Ensure startIndex + 7 does not exceed the total number of LEDs in the strip.
     * @see displayNumber()
     * @see setColor()
     */
    Segment(NeoPixelBus<NeoGrbFeature, NeoEsp32Rmt0Ws2812xMethod>* strip0, int startIndex, RgbColor color);
    
    /**
     * @brief Construct a Segment for Channel 1 (RMT1)
     *
     * Initializes a 7-segment display that controls seven LEDs using the NeoEsp32Rmt1Ws2812xMethod
     * (RMT channel 1) of the ESP32.
     *
     * @param strip1 Pointer to the NeoPixelBus object configured for RMT channel 1.
     *               Must not be nullptr.
     * @param startIndex The starting index of the first LED (segment 'a') in the strip.
     *                   The segment uses 7 consecutive LEDs starting from this index.
     * @param color The RgbColor to use for illuminating the segment.
     *              Can be changed later with setColor().
     * 
     * @note Ensure startIndex + 7 does not exceed the total number of LEDs in the strip.
     * @see displayNumber()
     * @see setColor()
     */
    Segment(NeoPixelBus<NeoGrbFeature, NeoEsp32Rmt1Ws2812xMethod>* strip1, int startIndex, RgbColor color);

    /**
     * @brief Display a digit (0-9) on the 7-segment display
     *
     * Activates the appropriate LED segments to form the requested digit.
     * Uses standard 7-segment encoding to display digits 0-9.
     *
     * @param number The digit to display (valid range: 0-9).
     *               Values outside this range may result in no display or unpredictable behavior.
     * 
     * @note The display updates the LED strip immediately with the new segments.
     */
    void displayNumber(int number);

    /**
     * @brief Display a loading animation on the segment
     * 
     * Runs an animated sequence on the 7-segment display to indicate system loading
     * or processing state. The animation cycles through the segments in a visually
     * appealing pattern.
     * 
     * Uses currentFrame to track animation state across multiple calls.
     * 
     * @note This is typically called in a loop with a delay between calls to create
     *       smooth animation. Consider calling at regular intervals (e.g., every 50-100ms).
     * @see displayNumber()
     */
    void loadingAnimation();
};