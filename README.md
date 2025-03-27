# Maszyna W - ESP32 Server

This project is a software implementation for hosting an Access Point and Web Server on ESP32 microcontroller. It features WebSocket communication, DNS Capture Portal, and LED control.

### Prerequisites

- ESP32 microcontroller
- ESP32 development environment (e.g., Arduino IDE, PlatformIO)
- Web browser for accessing the user interface

### Installation

1. Clone the repository:
    ```bash
    git clone <repository-url>
    ```
2. Set up the ESP32 development environment (PlatformIO recomended).
3. Upload the website build to the ESP32.
    ```bash
    pio run --target uploadfs
    ```
4. Upload the main script to the ESP32
    ```bash
    pio run --target upload
    ```
5. Run the program