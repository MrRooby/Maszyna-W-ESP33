#include <Arduino.h>
#include "w_server.h"
#include "display_manager.h"
#include "human_interface.h"
#include "w_local.h"

// TODO:  zmiana trybów działa aczkolwiek resetuje się przy zmiane trybu na lokalny
DisplayManager *dispMan = nullptr;
HumanInterface *humInter = nullptr;
W_Server *webMachine = nullptr;
W_Local *localMachine = nullptr;

bool lastWiFiState = false;
bool modeInitialized = false;

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
        dispMan->changeDisplayColor("0000FF", "0000FF", "0000FF");
        webMachine = new W_Server(dispMan, humInter);
    }
    else if (!currentWiFiState && localMachine == nullptr) {
        Serial.println("Starting Local mode...");
        dispMan->changeDisplayColor("", "", "");
        localMachine = new W_Local(dispMan, humInter);
    }
    
    lastWiFiState = currentWiFiState;
    modeInitialized = true;
}

void setup(){
    delay(2000);
    Serial.begin(115200);
    Serial.println("Serial initialized");
    
    dispMan = new DisplayManager("sex", "BUS", "00FF00");
    humInter = new HumanInterface();

    humInter->controlBacklightLED(30);

    initializeMode();    
}


void loop() {
    initializeMode();

    if(humInter->WiFiEnabled() && webMachine != nullptr){
        webMachine->runServer();
    }
    else if(!humInter->WiFiEnabled() && localMachine != nullptr){
        localMachine->runLocal();
    }
}
