#include <WiFi.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <LittleFS.h>
#include <ArduinoJson.h>

#include "credentials.h"
#include "display_manager.h"

class W_Server{
    private:
        const char* ssid = WIFI_SSID;
        const char* password = WIFI_PASS;

        AsyncWebServer server;
        AsyncWebSocket ws;

        DisplayManager dispMan;

        IPAddress local_IP;
        IPAddress gateway;
        IPAddress subnet;

        void connectToWifi();

        void createAccessPoint();

        void handleWebSocketMessage(void *arg, uint8_t *data, size_t len);
        
        void onEvent(AsyncWebSocket *server, AsyncWebSocketClient *client, AwsEventType type, void *arg, uint8_t *data, size_t len);

        void mountWebFiles();
    
    public:
        W_Server() : 
            server(80), 
            ws("/ws"),
            dispMan(50, 50, 60),
            local_IP(192, 168, 4, 1),
            gateway(192, 168, 4, 1),
            subnet(255, 255, 255, 0)
        {}

        void initServer();

        void runServer();
};