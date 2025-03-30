#include <M5StickCPlus.h>
#include <WiFi.h>
#include "config.h"
#include "tools/battery.h"
#include "tools/guard.h"

int lastSyncSecond = LAST_SYNC_SECOND;

void setup() {
    M5.begin();
    M5.Lcd.setRotation(1);
    M5.Lcd.fillScreen(BLACK);
    M5.Lcd.setTextSize(1);
    M5.Lcd.setTextColor(WHITE, BLACK);
    M5.Lcd.println("Initializing");
    
    Serial.begin(115200);
    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

    while (WiFi.status() != WL_CONNECTED) {
        delay(1000);
        M5.Lcd.println("Connecting to WiFi");
    }

    M5.Lcd.println("Connected to WiFi");

    syncTime();
    showBattery();
    showGuardCode();
}

void loop() {
    M5.update();

    showBattery();

    if (M5.BtnA.wasPressed()) {
        M5.Lcd.fillScreen(BLACK);
        M5.Lcd.setCursor(0, 0);
        M5.Lcd.setTextSize(1);
        M5.Lcd.println("Refreshing code");

        showGuardCode();
    }

    struct tm timeinfo;

    if (getLocalTime(&timeinfo)) {
        int currentSecond = timeinfo.tm_sec;

        if ((currentSecond == 0 || currentSecond == 30) && currentSecond != lastSyncSecond) {
            showGuardCode();
            lastSyncSecond = currentSecond;
        }
    }
}