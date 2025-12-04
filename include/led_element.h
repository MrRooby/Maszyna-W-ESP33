#pragma once

#include <NeoPixelBus.h>

/**
 * @file led_element.h
 * @brief Base class for LED elements controlled via NeoPixelBus
 * 
 * Provides foundational functionality for controlling individual LED elements
 * within NeoPixel (WS2812B) LED strips. Supports dual RMT channels for independent
 * strip control and includes color management with channel swapping capabilities.
 * 
 * @author Bartosz Faruga / MrRooby
 * @date 2025
 */
class LedElement {
    protected:
        NeoPixelBus<NeoGrbFeature, NeoEsp32Rmt0Ws2812xMethod>* strip0 = nullptr;
        NeoPixelBus<NeoGrbFeature, NeoEsp32Rmt1Ws2812xMethod>* strip1 = nullptr;

        int startIndex;
        int pixelCount = 0;
        RgbColor color;

    public:
        /**
         * @brief Default constructor
         * 
         * Initializes an empty LED element with no strip assigned.
         * Must be configured via parametrized constructors before use.
         */
        LedElement() = default;
        
        /**
         * @brief Construct a new Led Element for Channel 0 (RMT0)
         * 
         * Initializes an LED element that controls a segment of an LED strip using
         * the NeoEsp32Rmt0Ws2812xMethod (RMT channel 0).
         * 
         * @param strip0 Pointer to the NeoPixelBus object representing the LED strip (channel 0).
         *               Must not be nullptr.
         * @param startIndex The starting index (pixel position) of this LED element within the strip.
         *                   Should be >= 0 and < total number of pixels.
         * @param color The initial RgbColor value to assign to this element.
         *              Can be changed later with setColor().
         * 
         * @note The pixel count is not set in this constructor; derived classes should set
         *       pixelCount as needed.
         * @see setColor()
         */
        LedElement(NeoPixelBus<NeoGrbFeature, NeoEsp32Rmt0Ws2812xMethod>* strip0, int startIndex, RgbColor color);
        
        /**
         * @brief Construct a new Led Element for Channel 1 (RMT1)
         * 
         * Initializes an LED element that controls a segment of an LED strip using
         * the NeoEsp32Rmt1Ws2812xMethod (RMT channel 1).
         * 
         * @param strip1 Pointer to the NeoPixelBus object representing the LED strip (channel 1).
         *               Must not be nullptr.
         * @param startIndex The starting index (pixel position) of this LED element within the strip.
         *                   Should be >= 0 and < total number of pixels.
         * @param color The initial RgbColor value to assign to this element.
         *              Can be changed later with setColor().
         * 
         * @note The pixel count is not set in this constructor; derived classes should set
         *       pixelCount as needed.
         * @see setColor()
         */
        LedElement(NeoPixelBus<NeoGrbFeature, NeoEsp32Rmt1Ws2812xMethod>* strip1, int startIndex, RgbColor color);
        
        /**
         * @brief Set the color of the LED element
         * 
         * Updates the color of this LED element. If the R and G channels are swapped
         * (indicated by the swappedRG flag), the color will be automatically adjusted
         * before being stored.
         * 
         * **Color adjustment:**
         * @li If swappedRG is true: (R, G, B) → (G, R, B)
         * @li If swappedRG is false: color is stored as-is
         * 
         * @param color The new RgbColor to set for this element
         * 
         * @note This is a virtual method; derived classes can override to add custom behavior
         *       such as immediately updating the LED strip.
         * @see swapRG()
         */
        virtual void setColor(RgbColor color);

        /**
         * @brief Swaps the Red and Green channels of an RGB color
         * 
         * Useful for correcting color channel ordering mismatches between different
         * LED strips or configurations. Converts (R, G, B) to (G, R, B).
         * 
         * **Example:**
         * @li Input: RgbColor(255, 128, 0) (red-ish orange)
         * @li Output: RgbColor(128, 255, 0) (green-ish yellow)
         * 
         * @param color The original RgbColor to swap channels for
         * 
         * @return A new RgbColor with R and G channels swapped, B channel unchanged
         * 
         * @note This is a utility function and does not modify the element's internal color.
         *       Use setColor() to apply a swapped color to the element.
         */
        RgbColor swapRG(const RgbColor &color);
        
        /**
         * @brief Swaps the Red and Blue channels of an RGB color
         * 
         * @param color The original RgbColor to swap channels for
         * 
         * @return A new RgbColor with R and B channels swapped, G channel unchanged
         * 
         * @note This is a utility function and does not modify the element's internal color.
         *       Use setColor() to apply a swapped color to the element.
         */
        RgbColor swapRB(const RgbColor &color);
};