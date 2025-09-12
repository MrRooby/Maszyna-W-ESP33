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
    Serial.println("Serial initialized");
    
    dispMan = new DisplayManager(30);
    humInter = new HumanInterface();

    webMachine = new W_Server(dispMan, humInter);
}

int n = 0;

void loop() {
    webMachine->runServer();

    EncoderState state = humInter->getEncoderState();
    if(humInter->getEncoderButtonState() == true){
        humInter->controlBacklightLED(true);
    }
    else
        humInter->controlBacklightLED(false);

    if(state == EncoderState::DOWN){
        n--;
    }
    else if(state == EncoderState::UP){
        n++;
    }

    Serial.println(n);
}