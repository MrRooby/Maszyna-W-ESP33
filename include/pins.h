#pragma once

/**
 * @file pins.h
 * @brief GPIO pin definitions for the ESP32 machine control system
 * 
 * Centralized pin configuration for all hardware interfaces including LED strips,
 * input controls (encoder, buttons, switches), multiplexer addressing, and status LEDs.
 * 
 * **Hardware Overview:**
 * @li **LED Strips** - Two NeoPixel strips controlled via RMT channels
 * @li **Encoder** - Rotary encoder with push switch for navigation
 * @li **Multiplexer** - 4-to-16 multiplexer for button matrix scanning
 * @li **Backlight** - Dual-channel LED backlight control
 * @li **Status Indicators** - WiFi switch and on-board status LEDs
 * 
 * @author Bartosz Faruga / MrRooby
 * @date 2025
 */

///////////////////////////////////////////////////////////////////////
/////////////////////////// PIN DECLARATION ///////////////////////////

/**
 * @defgroup LED_Strips LED Strip Ports
 * @brief GPIO pins for NeoPixel LED strip control
 * @{
 */

/** @brief GPIO pin for right LED strip (RMT Channel 0) */
#define LED_PORT_R 9

/** @brief GPIO pin for left LED strip (RMT Channel 1) */
#define LED_PORT_L 10

/** @} */

/**
 * @defgroup Encoder Rotary Encoder Pins
 * @brief GPIO pins for rotary encoder input device
 * @{
 */

/** @brief Rotary encoder switch (push button) pin */
#define ENC_SW 6

/** @brief Rotary encoder data pin (phase B) */
#define ENC_DT 7

/** @brief Rotary encoder clock pin (phase A) */
#define ENC_CLK 8

/** @} */

/**
 * @defgroup Multiplexer 4-to-16 Button Multiplexer
 * @brief GPIO pins for multiplexer addressing and data input
 * 
 * Uses 4 selection lines (S0-S3) to address 16 channels.
 * Selection pattern creates channel number: (S3 << 3) | (S2 << 2) | (S1 << 1) | S0
 * @{
 */

/** @brief Multiplexer address select line S0 (LSB) */
#define MUX_S0 11

/** @brief Multiplexer address select line S1 */
#define MUX_S1 12

/** @brief Multiplexer address select line S2 */
#define MUX_S2 13

/** @brief Multiplexer address select line S3 (MSB) */
#define MUX_S3 14

/** @brief Multiplexer common output (data input) pin */
#define MUX_COM 1

/** @} */

/**
 * @defgroup Backlight LED Backlight Control
 * @brief GPIO pins for dual-channel backlight LED control
 * @{
 */

/** @brief Primary backlight LED control pin (PWM) */
#define LED_BACKLIGHT_1 38

/** @brief Secondary backlight LED control pin (PWM) */
#define LED_BACKLIGHT_2 2

/** @} */

/**
 * @defgroup Controls Input Switches and Buttons
 * @brief GPIO pins for mode switches and control buttons
 * @{
 */

/** @brief WiFi mode switch pin (determines WiFi server vs local mode) */
#define WIFI_SWITCH 16

/** @brief WYS (special function) button pin */
#define WYS_BTN 15

/** @} */

/**
 * @defgroup StatusLEDs On-Board Status LEDs
 * @brief GPIO pins for on-board indicator LEDs
 * @{
 */

/** @brief Top on-board status LED (typically indicates system state) */
#define ONBOARD_LED_TOP 47

/** @brief Bottom on-board status LED (typically indicates system state) */
#define ONBOARD_LED_BOTTOM 48

/** @} */

///////////////////////////////////////////////////////////////////////