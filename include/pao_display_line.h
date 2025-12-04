#pragma once

#include "three_digit_display.h"
#include "two_digit_display.h"
#include "led_element.h"

/**
 * @file pao_display_line.h
 * @brief Manages a Program-Address-Operand (PAO) display line with three separate displays
 * 
 * Controls a composite display consisting of three independent digit displays that
 * represent a machine instruction format:
 * @li **Address** - 2-digit display for memory/instruction address
 * @li **Value** - 3-digit display for the main instruction value/opcode
 * @li **Argument** - 2-digit display for instruction arguments/parameters
 * 
 * All displays are controlled by the same LED strip controller and can be synchronized
 * for color changes and animations.
 * 
 * @author Bartosz Faruga / MrRooby
 * @date 2025
 */
class PaODisplayLine: protected LedElement {
    public:
        
        TwoDigitDisplay *addr = nullptr;    ///< Address display (2 digits)
        ThreeDigitDisplay *val = nullptr;   ///< Value display (3 digits)
        TwoDigitDisplay *arg = nullptr;     ///< Argument display (2 digits)

        /**
         * @brief Construct a new PAO Display Line using Channel 0 (RMT0)
         * 
         * Initializes a PAO display line that controls three separate digit displays
         * (address, value, argument) using RMT channel 0 of the ESP32.
         * 
         * @param strip0 Pointer to the NeoPixelBus controller for RMT channel 0.
         *               Must not be nullptr.
         * @param startIndex The starting pixel index in the LED strip where this display begins.
         *                   Should be >= 0 and account for all pixels needed by the three displays.
         * @param color The initial color to display on all three display components.
         *              Can be changed later with setColor().
         * 
         * @note The constructor creates and initializes the three internal display objects
         *       (addr, val, arg) automatically.
         * @see displayLine()
         * @see setColor()
         */
        PaODisplayLine(NeoPixelBus<NeoGrbFeature, NeoEsp32Rmt0Ws2812xMethod>* strip0, int startIndex, RgbColor color);    
        
        /**
         * @brief Construct a new PAO Display Line using Channel 1 (RMT1)
         * 
         * Initializes a PAO display line that controls three separate digit displays
         * (address, value, argument) using RMT channel 1 of the ESP32.
         * 
         * @param strip1 Pointer to the NeoPixelBus controller for RMT channel 1.
         *               Must not be nullptr.
         * @param startIndex The starting pixel index in the LED strip where this display begins.
         *                   Should be >= 0 and account for all pixels needed by the three displays.
         * @param color The initial color to display on all three display components.
         *              Can be changed later with setColor().
         * 
         * @note The constructor creates and initializes the three internal display objects
         *       (addr, val, arg) automatically.
         * @see displayLine()
         * @see setColor()
         */
        PaODisplayLine(NeoPixelBus<NeoGrbFeature, NeoEsp32Rmt1Ws2812xMethod>* strip1, int startIndex, RgbColor color);    

        /**
         * @brief Display values on all three displays simultaneously
         * 
         * Updates the address, value, and argument displays to show the provided values.
         * All three displays are refreshed in the LED strip.
         * 
         * **Display format:**
         * ```
         * [Address] [Value] [Argument]
         *  (0-99)   (0-999)  (0-99)
         * ```
         * 
         * @param addr Address value to display (valid range: 0-99).
         *             Values > 99 will be clamped or wrapped by the underlying display.
         * @param val Main instruction value to display (valid range: 0-999).
         *            Values > 999 will be clamped or wrapped by the underlying display.
         * @param arg Argument value to display (valid range: 0-99).
         *            Values > 99 will be clamped or wrapped by the underlying display.
         * 
         * @note This method updates the internal display buffers and sends data to the LED strip.
         * @see loadingAnimation()
         */
        void displayLine(int addr, int val, int arg);

        void displayLettersOnAddrField(const char firstLetter, const char secondLetter);
        
        void displayLettersOnValField(const char firstLetter, const char secondLetter, const char thirdLetter);

        void displayLettersOnArgField(const char firstLetter, const char secondLetter);

        /**
         * @brief Display a loading animation on the PAO display
         * 
         * Runs an animated sequence on the display to indicate system loading or
         * processing state. The animation typically cycles through the display LEDs
         * in a visually appealing pattern.
         * 
         * @note This is a blocking call; consider running in a separate task if
         *       responsiveness is critical.
         * @see displayLine()
         */
        void loadingAnimation();
        
        /**
         * @brief Change the color of all three display components
         * 
         * Updates the color of the address, value, and argument displays simultaneously.
         * Useful for visual feedback (e.g., changing color on error, highlighting active display).
         * 
         * Inherits color swapping behavior from LedElement if swappedRG flag is set.
         * 
         * @param color New RgbColor to apply to all display components.
         *              Example: RgbColor(255, 0, 0) for red.
         * 
         * @note This overrides the base LedElement::setColor() method.
         * @see LedElement::setColor()
         */
        void setColor(RgbColor color);
};