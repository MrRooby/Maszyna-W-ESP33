#include <Arduino.h>
#include "w_server.h"

W_Server *server;

void setup(){
    Serial.begin(115200);
    server = new W_Server();
    server->initServer();
}

void loop(){
    server->runServer();
}