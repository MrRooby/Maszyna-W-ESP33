#pragma once

/**
 * @file credentials.h
 * @brief WiFi credentials configuration
 * 
 * Contains SSID and password for the ESP32's WiFi Access Point.
 * These credentials are used by the W_Server class to create
 * the WiFi hotspot.
 * 
 * @author Bartosz Faruga / MrRooby
 * @date 2025
 */

/**
 * @brief WiFi network name (SSID)
 * 
 * The SSID that will be broadcasted by the ESP32
 * when operating in Access Point mode.
 */
static const char* WIFI_SSID = "Maszyna W(eb)";

/**
 * @brief WiFi password
 * 
 * The password required to connect to the ESP32's
 * WiFi Access Point. Must be at least 8 characters long.
 */
static const char* WIFI_PASS = "czytwysweiil";