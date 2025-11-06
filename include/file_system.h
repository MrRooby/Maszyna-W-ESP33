#pragma once

#include <LittleFS.h>
#include <ArduinoJson.h>
#include <string>
#include "display_manager.h"

/**
 * @file file_system.h
 * @brief Manages persistent storage and configuration on the ESP32 using LittleFS
 * 
 * Provides file operations and JSON-based configuration management for display
 * element colors and other persistent settings. All data is stored in LittleFS
 * filesystem with JSON serialization.
 * 
 * @author Bartosz Faruga / MrRooby
 * @date 2025
 */
class FileSystem
{
private:
    bool mounted = false;
    const char* configPath = "/config.json";
    
    /**
     * @brief Converts a DisplayElement enum to its JSON key name
     * 
     * Maps display element types to their corresponding configuration key strings:
     * @li SIGNAL_LINE → "signal_line"
     * @li DIGIT_DISPLAY → "display"
     * @li BUS_LINE → "bus"
     * 
     * @param element The DisplayElement enum value to convert
     * @return Pointer to the corresponding string name, or nullptr if element is invalid
     */
    const char* getDisplayElementName(const DisplayElement element);
    
public:
    /**
     * @brief Default constructor
     * 
     * Initializes the FileSystem with no filesystem mounted.
     */
    FileSystem();
    
    /**
     * @brief Destructor
     * 
     * Cleans up FileSystem resources. The filesystem is not automatically unmounted;
     * call end() explicitly if needed.
     */
    ~FileSystem();

    /**
     * @brief Initializes and mounts the LittleFS filesystem
     * 
     * Attempts to mount the LittleFS filesystem on the ESP32. If mounting fails,
     * can optionally format the filesystem and retry.
     * 
     * @param formatOnFail If true, formats the filesystem if initial mount fails
     *                     and attempts to remount. Default is true.
     * 
     * @return true if filesystem is successfully mounted, false otherwise
     * 
     * @note If the config file doesn't exist after mounting, a debug message is printed
     * @see end()
     */ 
    bool begin(bool formatOnFail = true);

    /**
     * @brief Checks if the filesystem is currently mounted
     * 
     * @return true if filesystem is mounted and ready for operations, false otherwise
     */
    bool isMounted() const;

    /**
     * @brief Unmounts the LittleFS filesystem
     * 
     * Safely unmounts the filesystem and updates the mounted flag.
     * Safe to call multiple times.
     * 
     * @see begin()
     */
    void end();

    /**
     * @brief Checks if a file exists in the filesystem
     * 
     * @param path The file path to check (e.g., "/config.json")
     * 
     * @return true if the file exists and filesystem is mounted, false otherwise
     */
    bool fileExists(const char* path);

    /**
     * @brief Saves a color configuration for a specific display element
     * 
     * Stores a color value in the configuration file under the specified display element.
     * If the config file doesn't exist, creates a new one. Existing configurations are
     * preserved when adding new entries.
     * 
     * **Configuration structure:**
     * ```json
     * {
     *   "colors": {
     *     "signal_line": "#FF0000",
     *     "display": "#00FF00",
     *     "bus": "#0000FF"
     *   }
     * }
     * ```
     * 
     * @param element The DisplayElement type to save the color for
     * @param colorHEX Color value as hex string (e.g., "#FF0000" for red)
     * 
     * @return true if color was successfully saved, false if an error occurred
     * 
     * @note Requires filesystem to be mounted
     * @see loadColorConfig()
     */
    bool saveColorConfig(const DisplayElement element, const char* colorHEX);

    /**
     * @brief Loads a color configuration for a specific display element
     * 
     * Retrieves the stored color value for a given display element from the config file.
     * 
     * @param element The DisplayElement type to retrieve the color for
     * 
     * @return Pointer to the color hex string (e.g., "#FF0000"), or empty string "" if:
     *         @li The config file doesn't exist
     *         @li The JSON cannot be parsed
     *         @li The element name is invalid
     *         @li The color value hasn't been set for this element
     * 
     * @note Requires filesystem to be mounted
     * @see saveColorConfig()
     */
    const char* loadColorConfig(const DisplayElement element);

    /**
     * @brief Saves an entire JSON color configuration document to file
     * 
     * Persists a complete color configuration JSON document to the config file,
     * overwriting any existing configuration.
     * 
     * **Expected document structure:**
     * ```json
     * {
     *   "colors": {
     *     "signal_line": "#RRGGBB",
     *     "display": "#RRGGBB",
     *     "bus": "#RRGGBB"
     *   }
     * }
     * ```
     * 
     * @param doc Reference to a JsonDocument containing the complete color configuration
     * 
     * @return true if document was successfully written to file, false otherwise
     * 
     * @note Requires filesystem to be mounted
     * @see loadAllColors()
     */
    bool saveAllColors(JsonDocument& doc);

    /**
     * @brief Loads the entire JSON color configuration document from file
     * 
     * Reads the complete configuration file and deserializes it into the provided
     * JsonDocument. Allows bulk loading and manipulation of all color settings.
     * 
     * @param doc Reference to a JsonDocument that will be populated with the loaded configuration
     * 
     * @return true if configuration was successfully loaded and parsed, false if:
     *         @li The config file doesn't exist
     *         @li JSON deserialization failed
     * 
     * @note Requires filesystem to be mounted
     * @note The passed JsonDocument should have sufficient capacity for the config
     * @see saveAllColors()
     */
    bool loadAllColors(JsonDocument& doc);
};

