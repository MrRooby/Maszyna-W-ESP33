#include <Arduino.h>
#include "w_server.h"
#include "display_manager.h"
#include "human_interface.h"

DisplayManager *dispMan = nullptr;
HumanInterface *humInter = nullptr;
W_Server *webMachine = nullptr;


void setup(){
    delay(2000);
    Serial.begin(115200);
    
    dispMan = new DisplayManager(30);
    humInter = new HumanInterface();

    webMachine = new W_Server(dispMan, humInter);
}

void loop() {
    webMachine->runServer();
}