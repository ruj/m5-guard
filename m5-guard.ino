#include <M5StickCPlus.h>
#include <mbedtls/md.h>
#include <mbedtls/base64.h>
#include <WiFi.h>
#include <time.h>
#include "config.h"
#include "tools/battery.h"
#include "tools/guard.h"

unsigned long LAST_TIME = 0;
const long INTERVAL = 30000;

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
    showCode();
}

void loop() {
    M5.update();

    showBattery();

    if (M5.BtnA.wasPressed()) {
        M5.Lcd.fillScreen(BLACK);
        M5.Lcd.setCursor(0, 0);
        M5.Lcd.setTextSize(1);
        M5.Lcd.println("Refreshing code");

        showCode();

        LAST_TIME = millis();
    }

    unsigned long currentMillis = millis();

    if (currentMillis - LAST_TIME >= INTERVAL) {
        showCode();

        LAST_TIME = currentMillis;
    }
}
