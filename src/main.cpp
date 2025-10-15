#include <Arduino.h>

#include "w_server.h"
#include "display_manager.h"
#include "human_interface.h"
#include "w_local.h"


DisplayManager *dispMan      = nullptr;
HumanInterface *humInter     = nullptr;
W_Server       *webMachine   = nullptr;
W_Local        *localMachine = nullptr;

bool lastWiFiState   = false;
bool modeInitialized = false;

bool TestMode = false;


void initializeMode() {
    bool currentWiFiState = humInter->WiFiEnabled();
    
    // Clean up previous mode if switching
    if (modeInitialized && (currentWiFiState != lastWiFiState)) {
        Serial.println("Mode switch detected - cleaning up...");
        
        // Add safety check
        if (dispMan != nullptr) {
            Serial.println("Clearing display...");
            dispMan->clearDisplay();
            Serial.println("Display cleared successfully");
        }
        
        if (webMachine != nullptr) {
            Serial.println("Deleting web machine...");
            delete webMachine;
            webMachine = nullptr;
            Serial.println("Web server stopped");
        }
        
        if (localMachine != nullptr) {
            Serial.println("Deleting local machine...");
            delete localMachine;
            localMachine = nullptr;
            Serial.println("Local machine stopped");
        }
    }
    
    // Initialize new mode if needed
    if (currentWiFiState && webMachine == nullptr) {
        Serial.println("Starting WiFi Server mode...");
        dispMan->changeDisplayColor("FF0000", "000011", "000011");
        webMachine = new W_Server(dispMan, humInter);
    }
    else if (!currentWiFiState && localMachine == nullptr) {
        Serial.println("Starting Local mode...");
        dispMan->changeDisplayColor("000900", "010101", "010101");
        localMachine = new W_Local(dispMan, humInter);
    }
    
    lastWiFiState = currentWiFiState;
    modeInitialized = true;
}


void setup(){
    delay(2000);
    Serial.begin(115200);
    
    dispMan = new DisplayManager();
    humInter = new HumanInterface();

    humInter->controlBacklightLED(30);

    if(!TestMode){
        initializeMode();    
    }
}


void loop() {
    if(!TestMode){
        initializeMode();
    
        if(humInter->WiFiEnabled() && webMachine != nullptr){
            webMachine->runServer();
        }
        else if(!humInter->WiFiEnabled() && localMachine != nullptr){
            localMachine->runLocal();
        }
    }
    else{
        dispMan->ledTest(100);
        dispMan->refreshDisplay();
        humInter->testButtons();
    }
}
