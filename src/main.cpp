#include <Arduino.h>
#include "w_server.h"

W_Server server;

void setup(){
    Serial.begin(115200);
    while(!Serial){
        ;
    }

    delay(500);
    
    Serial.println("Serial Started");
    
    server.initServer();
}

void loop(){

}