#ifndef WIFI_H
#define WIFI_H

#include <WiFi.h>
#include "../config.h"

bool connectToWifi(unsigned long timeoutMs = 15000) {
    WiFi.mode(WIFI_STA);

    const int networkCount = WIFI_NETWORK_SIZE;

    M5.Lcd.println("Configured networks (" + String(networkCount) + ")");

    for (int index = 0; index < networkCount; index++) {
        const char* ssid = WIFI_NETWORKS[index].SSID;
        const char* password = WIFI_NETWORKS[index].PASSWORD;

        M5.Lcd.println("Trying to connect to " + String(ssid));

        WiFi.begin(ssid, password);

        unsigned long start = millis();

        while (WiFi.status() != WL_CONNECTED && millis() - start < timeoutMs) {
            delay(1000);
        }

        if (WiFi.status() == WL_CONNECTED) {
            M5.Lcd.println("Connected to " + String(ssid));

            return true;
        }
    }

    M5.Lcd.println("No WiFi connected");

    return false;
}

#endif