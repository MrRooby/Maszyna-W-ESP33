#pragma once

#include <WiFi.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <LittleFS.h>
#include <ArduinoJson.h>
#include <DNSServer.h>
#include <esp_wifi.h>

#include "credentials.h"
#include "display_manager.h"
#include "human_interface.h"
#include "file_system.h"

#define MAX_CLIENTS 2
#define WIFI_CHANNEL 6	// 2.4ghz channel 6 https://en.wikipedia.org/wiki/List_of_WLAN_channels#2.4_GHz_(802.11b/g/n/ax)


/**
 * @class W_Server
 * @brief Web server implementation for ESP32 that handles WebSocket connections and serves web content
 *
 * This class implements a web server with WebSocket capabilities for ESP32. It provides:
 * - WiFi Access Point creation
 * - DNS server for captive portal functionality
 * - WebSocket server for real-time communication
 * - Static web file serving from LittleFS
 * - Display management for visual feedback
 * - Human interface integration for button inputs
 * - File system management for configuration storage
 *
 * The server supports:
 * - Real-time display updates through WebSocket
 * - Button press event handling and broadcasting
 * - Loading animation when no clients are connected
 * - LED status indicators for server operation
 * - Color configuration for display elements
 * 
 * Communication Protocol:
 * Supports three types of WebSocket messages:
 * - "reg-update": For partial updates of display values or signal lines
 * - "mem-update": For full memory state updates
 * - "color-update": For updating display element colors
 *
 * @note The server uses AsyncWebServer for handling HTTP requests and WebSocket connections
 * @note Default IP address is set to 192.168.4.1 when operating as an Access Point
 * 
 * @see DisplayManager
 * @see HumanInterface
 * @see FileSystem
 * @see AsyncWebServer
 * @see AsyncWebSocket
 * @see DNSServer
 * 
 * @author Bartosz Faruga / MrRooby
 * @date 2025
 */
class W_Server {
private:
    const char* ssid     = WIFI_SSID;      ///< WiFi SSID from credentials.h
    const char* password = WIFI_PASS;      ///< WiFi password from credentials.h
    
    static AsyncWebServer *server;         ///< Main web server instance
    AsyncWebSocket *ws   = nullptr;        ///< WebSocket server instance
    DNSServer *dnsServer = nullptr;        ///< DNS server for captive portal
    
    String localURL = "";
    const IPAddress LOCAL_IP    = IPAddress(192, 168, 4,   1); ///< Local IP address for AP
    const IPAddress GATEWAY     = IPAddress(192, 168, 4,   1); ///< Gateway IP address
    const IPAddress SUBNET_MASK = IPAddress(255, 255, 255, 0); ///< Subnet mask
    
    DisplayManager *dispMan    = nullptr;  
    HumanInterface *humInter   = nullptr;
    FileSystem     *fileSystem = nullptr;
    
    char* lastSignal = nullptr;

    bool loading = true;

    int lastClientCount = 0;

    /**
     * @brief Initialize server components
     *
     * Sets up web files, access point, DNS server and WebSocket
     */
    void initServer();


    void createWebServer();

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
     * @brief Processes partial updates received via WebSocket as JSON data.
     *
     * This function handles incoming WebSocket messages of type "reg-update" containing
     * partial updates for display values or signal lines. It parses the provided JSON document,
     * determines the field and value to update, and updates the corresponding display or signal
     * line accordingly.
     *
     * @param doc The JSON document containing the partial update data. Expected to include
     *            a "field" key specifying the target and a "value" key with the new value.
     *            For some fields, arrays such as "addrs", "args", and "vals" may also be present.
     */
    void processPartialWebSocketData(StaticJsonDocument<512> doc);

    /**
     * @brief Processes a full WebSocket data update received from the client.
     *
     * This function handles incoming JSON documents containing a complete set of data updates,
     * typically sent with the "mem-update" message type. It extracts register values and display
     * information from the provided JSON document and updates the corresponding display elements.
     *
     * @param doc The JSON document containing the full data update, expected to have fields such as
     *            "acc", "a", "s", "c", "i", and arrays "addrs", "args", "vals" for updating display lines.
     */
    void processFullWebSocketData(StaticJsonDocument<512> doc);

    /**
     * @brief Sends a button press event to all connected WebSocket clients.
     *
     * This function constructs a JSON message containing the type of event ("button_press")
     * and the name or identifier of the button that was pressed. The message is then sent
     * to all clients currently connected via WebSocket.
     *
     * @param buttonNum Pointer to a character array representing the name or identifier of the button pressed.
     */
    void sendDataToClient(char *buttonNum);

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

    /**
     * @brief Controls the inbuilt LED indicator to show the server's running status.
     */
    void runningServerLED();

    /**
     * @brief Sends the currently active signal value to all connected WebSocket clients.
     *
     * This function checks if the active signal from the Buttons instance has changed
     * since the last call. If it has, it sends the signal value to all connected clients
     * via WebSocket and updates the last sent signal. This is typically used to notify
     * clients about button press events or signal changes.
     */
    void sendSignalValue();
    
    void handleLoadingAnimation();

    void updateColors(StaticJsonDocument<512> doc);

public:
    /**
     * @brief Construct a new W_Server object
     */
    W_Server(DisplayManager *dispMan, HumanInterface *humInter, FileSystem *fileSystem);


    /**
     * @brief Destructor for the W_Server class
     * 
     * Performs cleanup operations including:
     * - Cleaning up WebSocket clients
     * - Stopping the DNS server
     * - Ending the HTTP server
     * - Disconnecting WiFi Access Point
     * - Deleting WebSocket and DNS server objects
     * - Setting pointers to null
     * - Turning off the onboard LED
     */
    ~W_Server();
    

    /**
     * @brief Main server loop function that handles server operations.
     * 
     * This function performs the following tasks:
     * - Processes DNS requests
     * - Handles loading animation display based on client connection status
     * - Sends signal values to connected clients when buttons are pressed
     * - Controls server status LED indicators
     * - Refreshes the display
     * 
     * The function includes a 30ms delay between operations to prevent overwhelming the system.
     * It should be called repeatedly in the main program loop to maintain server functionality.
     */
    void runServer();
};