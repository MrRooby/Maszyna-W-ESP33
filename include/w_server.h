#pragma once

#include <WiFi.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <LittleFS.h>
#include <ArduinoJson.h>
#include <DNSServer.h>

#include "credentials.h"
#include "display_manager.h"
#include "human_interface.h"

/**
 * @brief Web server class managing WiFi, DNS and WebSocket connections, main logic controller
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
    
    static AsyncWebServer *server;             ///< Main web server instance
    AsyncWebSocket *ws = nullptr;              ///< WebSocket server instance
    DNSServer *dnsServer = nullptr;            ///< DNS server for captive portal
    
    IPAddress local_IP;                ///< Local IP address for AP
    IPAddress gateway;                 ///< Gateway IP address
    IPAddress subnet;                  ///< Subnet mask
    
    DisplayManager *dispMan = nullptr;            ///< Display manager instance
    HumanInterface *humInter = nullptr;
    
    char* lastSignal = nullptr;

    bool loading = true;

    int lastClientCount = 0;

    /**
     * @brief Initialize server components
     *
     * Sets up web files, access point, DNS server and WebSocket
     */
    void initServer();

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

public:
    /**
     * @brief Construct a new W_Server object
     */
    W_Server(DisplayManager *dispMan, HumanInterface *humInter);

    ~W_Server();
    
    /**
     * @brief Run server main loop
     *
     * Processes DNS requests and handles status LED
     */
    void runServer();
};