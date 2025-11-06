#pragma once

#include "segment.h"
#include "led_element.h"

/**
 * @file three_digit_display.h
 * @brief Three-digit seven-segment LED display controller
 * 
 * Controls a composite display consisting of three independent seven-segment displays
 * to show three-digit numbers (0-999). Each digit is controlled independently using
 * the Segment class, allowing for flexible digit manipulation and animations.
 * 
 * All three segments are controlled by the same LED strip and can be synchronized
 * for color changes and animations.
 * 
 * @author Bartosz Faruga / MrRooby
 * @date 2025
 */
class ThreeDigitDisplay : public LedElement {
    private:
        Segment display[3];    ///< Array of three seven-segment displays

    public:
        /**
         * @brief Construct a ThreeDigitDisplay for Channel 0 (RMT0)
         * 
         * Initializes a three-digit seven-segment display that controls 21 LEDs (3 segments × 7 LEDs each)
         * using the NeoEsp32Rmt0Ws2812xMethod (RMT channel 0) of the ESP32.
         * 
         * LED Layout:
         * ```
         * [Segment 0: 7 LEDs] [Segment 1: 7 LEDs] [Segment 2: 7 LEDs]
         * (Hundreds digit)    (Tens digit)        (Ones digit)
         * ```
         * 
         * @param strip0 Pointer to the NeoPixelBus object configured for RMT channel 0.
         *               Must not be nullptr.
         * @param startIndex The starting index of the first LED in the display on the strip.
         *                   Should be >= 0 and account for all 21 LEDs needed (startIndex + 21 ≤ total LEDs).
         * @param color The RgbColor to display for all three digits.
         *              Can be changed later with setColor().
         * 
         * @note The three internal Segment objects are automatically initialized starting at
         *       startIndex, startIndex+7, and startIndex+14 respectively.
         * @see displayValue()
         * @see setColor()
         */
        ThreeDigitDisplay(NeoPixelBus<NeoGrbFeature, NeoEsp32Rmt0Ws2812xMethod>* strip1, int startIndex, RgbColor color);    
        
        /**
         * @brief Construct a ThreeDigitDisplay for Channel 1 (RMT1)
         * 
         * Initializes a three-digit seven-segment display that controls 21 LEDs (3 segments × 7 LEDs each)
         * using the NeoEsp32Rmt1Ws2812xMethod (RMT channel 1) of the ESP32.
         * 
         * **LED Layout:**
         * ```
         * [Segment 0: 7 LEDs] [Segment 1: 7 LEDs] [Segment 2: 7 LEDs]
         * (Hundreds digit)    (Tens digit)        (Ones digit)
         * ```
         * 
         * @param strip1 Pointer to the NeoPixelBus object configured for RMT channel 1.
         *               Must not be nullptr.
         * @param startIndex The starting index of the first LED in the display on the strip.
         *                   Should be >= 0 and account for all 21 LEDs needed (startIndex + 21 ≤ total LEDs).
         * @param color The RgbColor to display for all three digits.
         *              Can be changed later with setColor().
         * 
         * @note The three internal Segment objects are automatically initialized starting at
         *       startIndex, startIndex+7, and startIndex+14 respectively.
         * @see displayValue()
         * @see setColor()
         */
        ThreeDigitDisplay(NeoPixelBus<NeoGrbFeature, NeoEsp32Rmt1Ws2812xMethod>* strip1, int startIndex, RgbColor color);    
        
        /**
         * @brief Display a three-digit number on all segments
         * 
         * Updates all three seven-segment displays to show the provided value. The number
         * is broken down into individual digits and displayed:
         * @li Hundreds digit → Segment 0
         * @li Tens digit → Segment 1
         * @li Ones digit → Segment 2
         * 
         * **Display examples:**
         * @li displayValue(0) → displays "000"
         * @li displayValue(42) → displays "042"
         * @li displayValue(999) → displays "999"
         * @li displayValue(1000+) → clamps and displays "999"
         * 
         * @param value Number to display (valid range: 0-999).
         *              Values > 999 may wrap or be clamped by the underlying display.
         * 
         * @note This method updates all three segments and sends data to the LED strip immediately.
         * @see loadingAnimation()
         */
        void displayValue(int value);

        /**
         * @brief Display a loading animation on all three digits
         * 
         * Runs an animated sequence across all three seven-segment displays to indicate
         * system loading or processing state. The animation typically cycles through
         * segments in a visually appealing pattern synchronized across all digits.
         * 
         * @note This is typically called in a loop with delays to create smooth animation.
         *       Consider calling at regular intervals (e.g., every 50-100ms).
         * @see displayValue()
         */
        void loadingAnimation();

        /**
         * @brief Change the color of all three digits simultaneously
         * 
         * Updates the display color for all three seven-segment displays at once.
         * Useful for visual feedback (e.g., error indication, state highlighting).
         * 
         * Inherits color swapping behavior from LedElement if swappedRG flag is set.
         * 
         * @param color New RgbColor to apply to all three digits.
         *              Example: RgbColor(255, 0, 0) for red.
         * 
         * @note This overrides the base LedElement::setColor() method.
         * @note Changes are applied to all three segment displays immediately.
         * @see LedElement::setColor()
         */
        void setColor(RgbColor color) override;

};