#include "WiFiPart.h"

static const char* ssid = "username"; // Replace with your Wi-Fi SSID
static const char* password = "password"; // Replace with your Wi-Fi password

void initWiFi() {
    WiFi.mode(WIFI_STA); // Set Wi-Fi mode to station

    // if(!WiFi.config(local_IP, gateway, subnet)) {
    //     Serial.println("STA Failed to configure");
    // }
    // else
    // {
    //     Serial.println("STA Configured");
    // }

    WiFi.begin(ssid, password); // Connect to Wi-Fi network
    WiFi.setSleep(false); // Disable Wi-Fi sleep mode for better performance
    Serial.print("Connecting to Wi-Fi");
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }

    Serial.println();
    Serial.println("Wi-Fi connected");
    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());
}

void wifiConnect() {
    if (WiFi.status() != WL_CONNECTED) {
        Serial.println("Wi-Fi not connected. Attempting to reconnect...");
        WiFi.disconnect();
        WiFi.reconnect();
    }
}

bool isWiFiConnected() {
    return WiFi.status() == WL_CONNECTED;
}