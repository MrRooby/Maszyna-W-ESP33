#include <Arduino.h>

#include "w_server.h"
#include "display_manager.h"
#include "human_interface.h"
#include "w_local.h"
#include "file_system.h"



DisplayManager *dispMan      = nullptr;
HumanInterface *humInter     = nullptr;
W_Server       *webMachine   = nullptr;
W_Local        *localMachine = nullptr;
FileSystem     *fileSystem   = nullptr;

bool lastWiFiState   = false;
bool modeInitialized = false;

bool TestMode = false;
bool prevTestMode = false;


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


void setup(){
    delay(2000);
    Serial.begin(115200);
    
    fileSystem = new FileSystem();
    fileSystem->begin();

    dispMan    = new DisplayManager(fileSystem->loadColorConfig(DisplayElement::SIGNAL_LINE), 
                                    fileSystem->loadColorConfig(DisplayElement::DIGIT_DISPLAY),
                                    fileSystem->loadColorConfig(DisplayElement::BUS_LINE));
    humInter   = new HumanInterface();

    humInter->controlBacklightLED(30);

    if(!TestMode){
        initializeMode();    
    }
}


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