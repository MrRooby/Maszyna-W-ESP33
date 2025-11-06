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

/** @brief Maximum number of simultaneous WiFi client connections */
#define MAX_CLIENTS 2

/** @brief WiFi channel for Access Point (2.4GHz channel 6) */
#define WIFI_CHANNEL 6

/**
 * @file w_server.h
 * @brief Web server implementation for ESP32 with WebSocket and captive portal support
 * 
 * Implements a WiFi Access Point with WebSocket communication for remote machine control.
 * Provides real-time display updates, button event handling, and color configuration management.
 * 
 * **Features:**
 * @li WiFi Access Point creation with captive portal
 * @li WebSocket server for real-time bidirectional communication
 * @li DNS server for client redirect to portal
 * @li Static web file serving from LittleFS
 * @li Real-time display synchronization
 * @li Button press event broadcasting
 * @li Loading animation when idle
 * @li LED status indicators
 * @li Color configuration persistence
 * 
 * **WebSocket Message Types:**
 * @li "reg-update" - Partial display/signal updates (single field)
 * @li "mem-update" - Full machine state update (all registers)
 * @li "color-update" - Display element color configuration
 * @li "ping" - Connection keep-alive probe
 * 
 * **Network Configuration:**
 * @li IP Address: 192.168.4.1
 * @li Subnet Mask: 255.255.255.0
 * @li Channel: 6 (2.4GHz)
 * @li Max Clients: 2 simultaneous connections
 * 
 * @author Bartosz Faruga / MrRooby
 * @date 2025
 * 
 * @see DisplayManager
 * @see HumanInterface
 * @see FileSystem
 * @see AsyncWebServer
 * @see AsyncWebSocket
 * @see DNSServer
 */

class W_Server {
private:
    const char* ssid     = WIFI_SSID;      ///< WiFi SSID from credentials.h
    const char* password = WIFI_PASS;      ///< WiFi password from credentials.h
    
    static AsyncWebServer *server;         ///< Main web server instance (static to prevent crash on deletion)
    AsyncWebSocket *ws   = nullptr;        ///< WebSocket server instance for real-time communication
    DNSServer *dnsServer = nullptr;        ///< DNS server for captive portal redirection
    
    String localURL = "";                  ///< Formatted URL string for the server

    const IPAddress LOCAL_IP    = IPAddress(192, 168, 4,   1); ///< Access Point IP address
    const IPAddress GATEWAY     = IPAddress(192, 168, 4,   1); ///< Gateway IP address
    const IPAddress SUBNET_MASK = IPAddress(255, 255, 255, 0); ///< Subnet mask for AP network
    
    DisplayManager *dispMan    = nullptr;  ///< Pointer to display manager for hardware control
    HumanInterface *humInter   = nullptr;  ///< Pointer to human interface for button input
    FileSystem     *fileSystem = nullptr;  ///< Pointer to file system for configuration storage
    
    char* lastSignal = nullptr;            ///< Tracks last button signal to detect changes
    bool loading = true;                   ///< Flag indicating loading animation state
    int lastClientCount = 0;               ///< Tracks previous client count for state change detection

    /**
     * @brief Initialize all server components
     * 
     * Sets up the complete server stack in order:
     * @li Create WiFi Access Point
     * @li Initialize DNS server for captive portal
     * @li Mount web files from LittleFS
     * @li Create HTTP web server routes
     * @li Start AsyncWebServer
     * @li Initialize WebSocket server
     * 
     * @note Called automatically in constructor
     */
    void initServer();

    /**
     * @brief Define HTTP routes and handlers for web server
     * 
     * Sets up all web endpoints including:
     * @li Captive portal detection endpoints (connecttest.txt, generate_204, etc.)
     * @li Static file serving from LittleFS
     * @li Default route redirection to portal
     * 
     * @note Includes debug logging for all endpoint accesses
     */
    void createWebServer();

    /**
     * @brief Connect to an existing WiFi network
     * 
     * Attempts to connect to a WiFi network using SSID and password from credentials.h.
     * Blocks until connection is established.
     * 
     * @note This method is currently not used; the server operates as an Access Point instead
     */
    void connectToWifi();

    /**
     * @brief Create WiFi Access Point for client connections
     * 
     * Configures the ESP32 as a WiFi Access Point with the following steps:
     * @li Set WiFi mode to AP only
     * @li Configure IP address, gateway, and subnet mask
     * @li Apply WiFi configuration
     * @li Disable AMPDU RX to fix Android compatibility issues
     * @li Print AP information to serial console
     * 
     * @note Max 2 simultaneous clients allowed (MAX_CLIENTS)
     */
    void createAccessPoint();

    /**
     * @brief Initialize DNS server for captive portal functionality
     * 
     * Sets up a DNS server on port 53 that redirects all domain queries to the local IP (192.168.4.1).
     * This forces connected clients to open the captive portal page.
     * 
     * @note TTL (Time To Live) is set to 3600 seconds
     */
    void createDNSServer();

    /**
     * @brief Initialize WebSocket server
     * 
     * Sets up the WebSocket endpoint at "/ws" with event handlers for:
     * @li Connection/disconnection events
     * @li Message reception
     * @li Error handling
     * 
     * @note Called after HTTP server initialization
     */
    void createWebSocketServer();

    /**
     * @brief Handle incoming WebSocket message frame
     * 
     * Parses the raw WebSocket frame data and deserializes JSON content.
     * Determines message type and routes to appropriate handler:
     * @li "reg-update" → processPartialWebSocketData()
     * @li "mem-update" → processFullWebSocketData()
     * @li "color-update" → updateColors()
     * @li "ping" → Connection verification (no action)
     * 
     * @param arg Pointer to AwsFrameInfo structure containing frame metadata
     * @param data Raw message data bytes
     * @param len Message data length in bytes
     * 
     * @note Validates JSON deserialization and logs errors to serial console
     */
    void handleWebSocketMessage(void *arg, uint8_t *data, size_t len);

    /**
     * @brief Process partial WebSocket updates for individual display elements
     * 
     * Handles "reg-update" messages that update single display values or signal lines.
     * Supports the following field types:
     * 
     * **Display Values (integer):**
     * @li "acc", "a", "s", "c", "i" - Three-digit displays
     * @li "addrs", "args", "vals" - PAO display array updates
     * 
     * **Signal Lines (boolean):**
     * @li "il", "wel", "wyl", "wyad", "wei", "weak", "dod", "ode"
     * @li "przep", "weja", "wyak", "wea", "czyt", "pisz", "wes", "wys"
     * @li "busA", "busS", "stop"
     * 
     * @param doc StaticJsonDocument containing the update with keys:
     *            - "field": Target display/signal name
     *            - "value": New value (int for displays, bool for signals)
     *            - "addrs"/"args"/"vals": Arrays for PAO updates
     * 
     * @note Logs all updates to serial console
     * @see processFullWebSocketData()
     */
    void processPartialWebSocketData(StaticJsonDocument<512> doc);

    /**
     * @brief Process full WebSocket updates for entire machine state
     * 
     * Handles "mem-update" messages containing a complete snapshot of machine state.
     * Updates all registers and displays from the provided JSON document.
     * 
     * **Fields in update:**
     * @li "data.acc" - Accumulator value (0-999)
     * @li "data.a" - Address register (0-999)
     * @li "data.s" - Stack pointer (0-999)
     * @li "data.c" - Counter register (0-999)
     * @li "data.i" - Instruction register (0-999)
     * @li "data.addrs[]" - PAO address array (4 entries)
     * @li "data.args[]" - PAO argument array (4 entries)
     * @li "data.vals[]" - PAO value array (4 entries)
     * 
     * @param doc StaticJsonDocument with nested "data" object containing all state
     * 
     * @note Used for synchronizing complete state after client connection
     * @see processPartialWebSocketData()
     */
    void processFullWebSocketData(StaticJsonDocument<512> doc);

    /**
     * @brief Send button press event to all connected WebSocket clients
     * 
     * Constructs a JSON message with button press information and broadcasts it
     * to all currently connected WebSocket clients. Enables remote display of
     * local button presses when hybrid control is used.
     * 
     * **Message format:**
     * ```json
     * {
     *   "type": "button_press",
     *   "buttonName": "IL"
     * }
     * ```
     * 
     * @param buttonNum Pointer to button name string (e.g., "IL", "WEL", "TAKT")
     * 
     * @note Called when button is pressed on the local machine
     * @see sendSignalValue()
     */
    void sendDataToClient(char *buttonNum);

    /**
     * @brief WebSocket event callback handler
     * 
     * Routes WebSocket events to appropriate handlers:
     * @li WS_EVT_CONNECT - Log client connection with IP and ID
     * @li WS_EVT_DISCONNECT - Log client disconnection
     * @li WS_EVT_DATA - Route to handleWebSocketMessage()
     * @li WS_EVT_PONG - PONG response (reserved for future use)
     * @li WS_EVT_ERROR - Error handling (reserved for future use)
     * 
     * @param server AsyncWebSocket server instance
     * @param client Connected AsyncWebSocketClient
     * @param type AwsEventType indicating the event type
     * @param arg Event-specific arguments (AwsFrameInfo for WS_EVT_DATA)
     * @param data Event data payload
     * @param len Length of event data in bytes
     */
    void onEvent(AsyncWebSocket *server, AsyncWebSocketClient *client,
                 AwsEventType type, void *arg, uint8_t *data, size_t len);

    /**
     * @brief Mount web files from LittleFS filesystem
     * 
     * Initializes LittleFS and makes web files available for serving.
     * Checks filesystem mount status and logs result to serial console.
     * 
     * @note Must be called before createWebServer()
     * @note Logs error if mount fails
     */
    void mountWebFiles();

    /**
     * @brief Control server status LED indicator
     * 
     * Toggles the BOTTOM on-board LED based on client connection status:
     * @li Connected clients (>0) - Blinks every 500ms
     * @li No clients - LED off (indicates idle/waiting state)
     * 
     * @note Part of the server status feedback system
     */
    void runningServerLED();

    /**
     * @brief Broadcast button press to connected WebSocket clients
     * 
     * Monitors button input from HumanInterface and sends button press events
     * to all connected clients when a new button press is detected.
     * Uses lastSignal to track state changes and avoid duplicate sends.
     * 
     * @note Called during runServer() loop
     * @see sendDataToClient()
     */
    void sendSignalValue();
    
    /**
     * @brief Manage loading animation based on client connection state
     * 
     * Transitions between loading animation (no clients) and normal operation (clients connected):
     * @li No clients connected - Display loading animation loop
     * @li First client connects - Stop animation, clear display
     * @li Last client disconnects - Resume loading animation
     * 
     * @note Tracks state transitions via loading and lastClientCount flags
     */
    void handleLoadingAnimation();

    /**
     * @brief Process color update message from WebSocket client
     * 
     * Updates display element colors based on received configuration:
     * 
     * **Color type options:**
     * @li "signal_line_hex" - Update signal line color (stores in filesystem)
     * @li "display_hex" - Update digit display color (stores in filesystem)
     * @li "bus_hex" - Update bus line color (stores in filesystem)
     * 
     * All colors are stored as hex strings (#RRGGBB format) in the configuration.
     * 
     * @param doc StaticJsonDocument with structure:
     *            - "data.colorType": Type of element to update
     *            - "data.hex": Hex color string (e.g., "#FF0000")
     * 
     * @note Persists color changes to LittleFS for retention after reboot
     * @note Logs all color updates to serial console
     */
    void updateColors(StaticJsonDocument<512> doc);

public:
    /**
     * @brief Construct a new W_Server object
     * 
     * Initializes the web server with provided manager and interface pointers.
     * Automatically calls initServer() to set up all components:
     * @li Creates WiFi Access Point
     * @li Initializes DNS and WebSocket servers
     * @li Mounts web files and sets up HTTP routes
     * @li Clears display to indicate server startup
     * 
     * @param dispMan Pointer to DisplayManager for display control. Must not be nullptr.
     * @param humInter Pointer to HumanInterface for input handling. Must not be nullptr.
     * @param fileSystem Pointer to FileSystem for configuration storage. Must not be nullptr.
     * 
     * @note Server is immediately ready to accept client connections after construction
     */
    W_Server(DisplayManager *dispMan, HumanInterface *humInter, FileSystem *fileSystem);

    /**
     * @brief Destructor for the W_Server class
     * 
     * Performs complete cleanup and teardown:
     * @li Cleans up all WebSocket clients
     * @li Stops DNS server
     * @li Ends HTTP server
     * @li Disconnects WiFi Access Point
     * @li Deletes dynamically allocated objects (WebSocket, DNS server)
     * @li Sets all pointers to null
     * @li Turns off on-board status LED
     * @li Logs destruction progress to serial console
     * 
     * @note Safe to call even if server was not fully initialized
     */
    ~W_Server();
    
    /**
     * @brief Main server operation loop
     * 
     * Executes core server functions in sequence:
     * @li Process DNS requests (x3 for responsive redirection)
     * @li Report connected client count changes
     * @li Handle loading animation state
     * @li Broadcast button press events to clients
     * @li Update server status LED
     * @li Refresh display with current state
     * 
     * **Timing:**
     * - 5ms delay between DNS processing batches
     * - Called repeatedly from main loop during WiFi server mode
     * - LED blinks at 500ms interval when clients connected
     * 
     * @note Should be called continuously when WiFiEnabled() returns true
     * @note Includes appropriate delays to prevent system overload
     * 
     * @see handleLoadingAnimation()
     * @see sendSignalValue()
     * @see runningServerLED()
     */
    void runServer();
};