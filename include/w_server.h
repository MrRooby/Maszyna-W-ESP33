#include <WiFi.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <LittleFS.h>
#include <WebSocketsServer.h>
#include <ArduinoJson.h>

#include "credentials.h"

class W_Server{
    private:
        const char* ssid = WIFI_SSID;
        const char* password = WIFI_PASS;

        AsyncWebServer server;
        WebSocketsServer webSocket;
        
        void connectToWifi();
        
        void webSocketEvent(uint8_t num, WStype_t type, uint8_t* payload, size_t length);

        void handleWebSocketMessage(uint8_t num, uint8_t* payload);

        void mountWebFiles();
    
    public:
        W_Server() : server(80), webSocket(81) {}

        void initServer();

        void runServer();
};