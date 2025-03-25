#include <Arduino.h>
#include "w_server.h"
#include "display_manager.h"
W_Server server;

// DisplayManager *dm = new DisplayManager(50, 50, 60);

void setup(){
    Serial.begin(9600);
    while(!Serial){
        ;
    }

    delay(500);
    
    Serial.println("Serial Started");
    
    server.initServer();
}

void loop(){
    // for(int i = 0; i < 999; i++){
    //     dm->acc->displayValue(i);
    //     delay(500);
    //     // dm->clearDisplay();
    // }


    // Serial.println("dziala");
}