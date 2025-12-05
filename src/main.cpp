/**
 * @file main.cpp
 * @brief Main entry point for the ESP32-based machine control system
 * 
 * Handles initialization and switching between WiFi SERVER Mode and LOCAL Mode.
 * Manages the lifecycle of all major system components including display, human interface,
 * and machine operation modes.
 */

#include <Arduino.h>

#include "w_server.h"
#include "display_manager.h"
#include "human_interface.h"
#include "w_local.h"
#include "file_system.h"

/*
====================================== TODO ========================================
- Odświeżanie wyświetlaczy po wykryciu zmiany koloru
    Wyświetlacze chyba będą zmuszone do pamiętania swojego stanu kurwa mać
- Korekta działania enkodera
- korekta działania maszyny lokalne (jak działają wykluczające się sygnały)
- miganie na czerwono w przypadku niedozwolonych sygnałów
====================================================================================
*/

DisplayManager *dispMan      = nullptr;
HumanInterface *humInter     = nullptr;
W_Server       *webMachine   = nullptr;
W_Local        *localMachine = nullptr;
FileSystem     *fileSystem   = nullptr;

bool lastWiFiState   = false;
bool modeInitialized = false;

bool TestMode        = false; // TEST MODE used for testing LED strip continuity
bool prevTestMode    = false;


/**
 * @brief Initializes or switches between operation modes
 * 
 * Detects WiFi Switch state changes and manages the transition between WiFi server mode
 * and local machine mode. Properly cleans up resources when switching modes.
 * 
 * When switching modes:
 * - Clears the display
 * - Deletes the previous machine instance
 * - Creates a new instance for the current mode
 * 
 * @details
 * - WiFi enabled  → W_Server mode (network-based control)
 * - WiFi disabled → W_Local mode (local button/encoder control)
 * 
 * @see W_Server
 * @see W_Local
 */
void initializeMode() {
    bool currentWiFiState = humInter->WiFiEnabled();
    
    // Clean up previous mode if switching
    if (modeInitialized && (currentWiFiState != lastWiFiState)) {
        Serial.println("[MAIN]: Mode switch detected - cleaning up...");
        
        // Add safety check
        if (dispMan) {
            Serial.println("[MAIN]: Clearing display...");
            dispMan->clearDisplay();
            Serial.println("[MAIN]: Display cleared successfully");
        }
        
        if (webMachine) {
            Serial.println("[MAIN]: Deleting web machine...");
            delete webMachine;
            webMachine = nullptr;
            Serial.println("[MAIN]: Web server stopped");
        }
        
        if (localMachine) {
            Serial.println("[MAIN]: Deleting local machine...");
            delete localMachine;
            localMachine = nullptr;
            Serial.println("[MAIN]: Local machine stopped");
        }
    }
    
    // Initialize new mode if needed
    if (currentWiFiState && !webMachine) {
        Serial.println("[MAIN]: Starting WiFi Server mode...");
        webMachine = new W_Server(dispMan, humInter, fileSystem);
    }
    else if (!currentWiFiState && !localMachine) {
        Serial.println("[MAIN]: Starting Local mode...");
        localMachine = new W_Local(dispMan, humInter);
    }
    
    lastWiFiState = currentWiFiState;
    modeInitialized = true;
}


/**
 * @brief Arduino setup function - initializes all system components
 * 
 * Initializes in order:
 * 1. Serial communication (115200 baud)
 * 2. File system and configuration loading
 * 3. Display manager with color configuration
 * 4. Human interface (buttons, encoder, backlight)
 * 5. Operating mode (unless TestMode is active)
 * 
 * @note 2-second delay before Serial initialization allows ESP32 to stabilize
 * 
 * @see DisplayManager
 * @see HumanInterface
 * @see FileSystem
 */
void setup(){
    delay(2000);
    Serial.begin(115200);
    
    fileSystem = new FileSystem();
    fileSystem->begin();

    dispMan    = new DisplayManager(fileSystem->loadColorConfig(DisplayElement::SIGNAL_LINE), 
                                    fileSystem->loadColorConfig(DisplayElement::DIGIT_DISPLAY),
                                    fileSystem->loadColorConfig(DisplayElement::BUS_LINE));
    humInter   = new HumanInterface();

    humInter->controlBacklightLED(255);

    if(!TestMode){
        initializeMode();    
    }
}


/**
 * @brief Arduino main loop - handles continuous operation
 * 
 * **Normal Operation Mode:**
 * - Initializes/switches between operating modes as needed
 * - Runs WiFi server if enabled: W_Server::runServer()
 * - Runs local machine if WiFi disabled: W_Local::runLocal()
 * 
 * **Test Mode (TestMode = true):**
 * - Bypasses normal operation
 * - Runs LED strip continuity test animation
 * - Displays different colors based on WiFi switch position
 * - WiFi ON: LED test animation (color sweep)
 * - WiFi OFF: Red LED (indicator)
 * - Monitors button presses via HumanInterface::testButtons()
 * 
 * @note Test mode is useful for verifying LED strip functionality before deployment
 * 
 * @see W_Server::runServer()
 * @see W_Local::runLocal()
 * @see DisplayManager::ledTestAnimation()
 */
void loop() {
    if(!TestMode){
        initializeMode();
    
        if(humInter->WiFiEnabled() && webMachine){
            webMachine->runServer();
        }
        else if(!humInter->WiFiEnabled() && localMachine){
            localMachine->runLocal();
        }
    }
    else {
        if(humInter->WiFiEnabled()){
            if(!prevTestMode){
                dispMan->controlAllLEDs(0, 0, 0);
                prevTestMode = humInter->WiFiEnabled();
                dispMan->ledTestAnimation(100, false, true);
            }
            
            dispMan->ledTestAnimation(100);
        }
        else {
            prevTestMode = humInter->WiFiEnabled();
            dispMan->controlAllLEDs(100, 0, 0);
        }

        dispMan->refreshDisplay();
        humInter->testButtons();
    }
}
