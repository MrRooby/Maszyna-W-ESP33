#pragma once

#include "segment.h"
#include "led_element.h"

/**
 * @file two_digit_display.h
 * @brief Two-digit seven-segment LED display controller
 * 
 * Controls a composite display consisting of two independent seven-segment displays
 * to show two-digit numbers (0-99). Each digit is controlled independently using
 * the Segment class, allowing for flexible digit manipulation and animations.
 * 
 * All two segments are controlled by the same LED strip and can be synchronized
 * for color changes and animations.
 * 
 * @author Bartosz Faruga / MrRooby
 * @date 2025
 */
class TwoDigitDisplay : protected LedElement {
    private:
        Segment display[2];    ///< Array of two seven-segment displays

    public:
        /**
         * @brief Construct a TwoDigitDisplay for Channel 0 (RMT0)
         * 
         * Initializes a two-digit seven-segment display that controls 14 LEDs (2 segments × 7 LEDs each)
         * using the NeoEsp32Rmt0Ws2812xMethod (RMT channel 0) of the ESP32.
         * 
         * **LED Layout:**
         * ```
         * [Segment 0: 7 LEDs] [Segment 1: 7 LEDs]
         * (Tens digit)        (Ones digit)
         * ```
         * 
         * @param strip0 Pointer to the NeoPixelBus object configured for RMT channel 0.
         *               Must not be nullptr.
         * @param startIndex The starting index of the first LED in the display on the strip.
         *                   Should be >= 0 and account for all 14 LEDs needed (startIndex + 14 ≤ total LEDs).
         * @param color The RgbColor to display for both digits.
         *              Can be changed later with setColor().
         * 
         * @note The two internal Segment objects are automatically initialized starting at
         *       startIndex and startIndex+7 respectively.
         * @see displayValue()
         * @see setColor()
         */
        TwoDigitDisplay(NeoPixelBus<NeoGrbFeature, NeoEsp32Rmt0Ws2812xMethod>* strip0, int startIndex, RgbColor color);    

        /**
         * @brief Construct a TwoDigitDisplay for Channel 1 (RMT1)
         * 
         * Initializes a two-digit seven-segment display that controls 14 LEDs (2 segments × 7 LEDs each)
         * using the NeoEsp32Rmt1Ws2812xMethod (RMT channel 1) of the ESP32.
         * 
         * **LED Layout:**
         * ```
         * [Segment 0: 7 LEDs] [Segment 1: 7 LEDs]
         * (Tens digit)        (Ones digit)
         * ```
         * 
         * @param strip1 Pointer to the NeoPixelBus object configured for RMT channel 1.
         *               Must not be nullptr.
         * @param startIndex The starting index of the first LED in the display on the strip.
         *                   Should be >= 0 and account for all 14 LEDs needed (startIndex + 14 ≤ total LEDs).
         * @param color The RgbColor to display for both digits.
         *              Can be changed later with setColor().
         * 
         * @note The two internal Segment objects are automatically initialized starting at
         *       startIndex and startIndex+7 respectively.
         * @see displayValue()
         * @see setColor()
         */
        TwoDigitDisplay(NeoPixelBus<NeoGrbFeature, NeoEsp32Rmt1Ws2812xMethod>* strip1, int startIndex, RgbColor color);    

        /**
         * @brief Display a two-digit number on both segments
         * 
         * Updates both seven-segment displays to show the provided value. The number
         * is broken down into individual digits and displayed:
         * @li Tens digit → Segment 0
         * @li Ones digit → Segment 1
         * 
         * **Display examples:**
         * @li displayValue(0) → displays "00"
         * @li displayValue(5) → displays "05"
         * @li displayValue(42) → displays "42"
         * @li displayValue(99) → displays "99"
         * 
         * @param value Number to display (valid range: 0-99).
         *              Values > 99 may wrap or be clamped by the underlying display.
         * 
         * @note This method updates both segments and sends data to the LED strip immediately.
         * @see loadingAnimation()
         */
        void displayValue(int value, bool enableLeadingZero = true);

        void displayLetters(const char firstLetter, const char secondLetter);

        /**
         * @brief Display a loading animation on both digits
         * 
         * Runs an animated sequence across both seven-segment displays to indicate
         * system loading or processing state. The animation typically cycles through
         * segments in a visually appealing pattern synchronized across both digits.
         * 
         * @note This is typically called in a loop with delays to create smooth animation.
         *       Consider calling at regular intervals (e.g., every 50-100ms).
         * @see displayValue()
         */ 
        void loadingAnimation();

        /**
         * @brief Change the color of both digits simultaneously
         * 
         * Updates the display color for both seven-segment displays at once.
         * Useful for visual feedback (e.g., error indication, state highlighting).
         * 
         * Inherits color swapping behavior from LedElement if swappedRG flag is set.
         * 
         * @param color New RgbColor to apply to both digits.
         *              Example: RgbColor(255, 0, 0) for red.
         * 
         * @note This overrides the base LedElement::setColor() method.
         * @note Changes are applied to both segment displays immediately.
         * @see LedElement::setColor()
         */
        void setColor(RgbColor color) override;
};