#pragma once

#include <WiFi.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <LittleFS.h>
#include <ArduinoJson.h>
#include <DNSServer.h>

#include "credentials.h"
#include "display_manager.h"

/**
 * @brief Web server class managing WiFi, DNS and WebSocket connections
 * 
 * This class handles the setup and management of:
 * - WiFi Access Point
 * - DNS server for captive portal
 * - WebSocket server for real-time communication
 * - Web file serving from LittleFS
 * 
 * @author Bartosz Faruga / MrRooby
 * @date 2025
 */
class W_Server {
    private:
        const char* ssid = WIFI_SSID;      ///< WiFi SSID from credentials.h
        const char* password = WIFI_PASS;  ///< WiFi password from credentials.h
        
        AsyncWebServer server;             ///< Main web server instance
        AsyncWebSocket ws;                 ///< WebSocket server instance
        DNSServer dnsServer;               ///< DNS server for captive portal
        
        IPAddress local_IP;                ///< Local IP address for AP
        IPAddress gateway;                 ///< Gateway IP address
        IPAddress subnet;                  ///< Subnet mask
        
        DisplayManager dispMan;            ///< Display manager instance

        /**
         * @brief Connect to existing WiFi network
         */
        void connectToWifi();

        /**
         * @brief Create WiFi Access Point
         */
        void createAccessPoint();

        /**
         * @brief Setup DNS server for captive portal
         */
        void createDNSServer();

        /**
         * @brief Initialize WebSocket server
         */
        void createWebSocketServer();

        /**
         * @brief Handle incoming WebSocket messages
         * @param arg Frame info
         * @param data Message data
         * @param len Data length
         */
        void handleWebSocketMessage(void *arg, uint8_t *data, size_t len);
        
        /**
         * @brief WebSocket event handler
         * @param server WebSocket server instance
         * @param client Connected client
         * @param type Event type
         * @param arg Additional arguments
         * @param data Event data
         * @param len Data length
         */
        void onEvent(AsyncWebSocket *server, AsyncWebSocketClient *client, 
                    AwsEventType type, void *arg, uint8_t *data, size_t len);

        /**
         * @brief Mount web files from LittleFS
         */
        void mountWebFiles();
    
    public:
        /**
         * @brief Construct a new W_Server object
         */
        W_Server();

        /**
         * @brief Initialize server components
         * 
         * Sets up web files, access point, DNS server and WebSocket
         */
        void initServer();

        /**
         * @brief Run server main loop
         * 
         * Processes DNS requests and handles status LED
         */
        void runServer();
};