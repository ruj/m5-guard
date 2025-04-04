#include <M5StickCPlus.h>
#include <WiFi.h>
#include "config.h"
#include "tools/battery.h"
#include "tools/guard.h"
#include "tools/utilities.h"

unsigned long lastUpdate = 0;
int lastSyncSecond = -1;
int timeLeft = 30; 
int lastTimeLeft = 30;

void setup() {
    M5.begin();
    M5.Lcd.setRotation(1);
    M5.Lcd.fillScreen(BLACK);
    M5.Lcd.setTextSize(1);
    M5.Lcd.setTextColor(WHITE, BLACK);
    M5.Lcd.println("Initializing");

    Serial.begin(115200);

    connectToWifi();
    syncTime();
    topBar();
    showGuardCode(true);
}

void loop() {
    M5.update();

    topBar();

    if (M5.BtnA.wasPressed()) {
        M5.Lcd.fillScreen(BLACK);
        M5.Lcd.setCursor(0, 0);
        M5.Lcd.setTextSize(1);
        M5.Lcd.println("Refreshing code");

        showGuardCode(true);
    }

    struct tm timeinfo;

    if (getLocalTime(&timeinfo)) {
        int currentSecond = timeinfo.tm_sec;

        if ((currentSecond == 0 || currentSecond == 30) && currentSecond != lastSyncSecond) {
            showGuardCode(true);

            lastSyncSecond = currentSecond;
            timeLeft = 30;
        }

        if (millis() - lastUpdate >= 1000) {
            lastUpdate = millis();

            if (timeLeft > 0) {
                timeLeft--;
            }

            if (timeLeft != lastTimeLeft) {
                showGuardCode(false);

                lastTimeLeft = timeLeft;
            }
        }
    }
}

void connectToWifi() {
    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

    while (WiFi.status() != WL_CONNECTED) {
        delay(1000);
        M5.Lcd.println("Connecting to WiFi");
    }

    M5.Lcd.println("Connected to WiFi");
}

void topBar() {
    M5.Lcd.setTextColor(WHITE, BLACK);
    M5.Lcd.setCursor(10, 10);
    M5.Lcd.setTextSize(1);
    M5.Lcd.println(getTimeString());
    M5.Lcd.setCursor(SCREEN_WIDTH / 2 - 5, 10);

    int battery = getBattery();

    if (battery > 50) {
        M5.Lcd.setTextColor(GREEN, BLACK);
    } else if (battery > 25) {
        M5.Lcd.setTextColor(YELLOW, BLACK);
    } else {
        M5.Lcd.setTextColor(RED, BLACK);
    }

    M5.Lcd.setTextSize(1);
    M5.Lcd.setCursor(SCREEN_WIDTH - 30, 10);
    M5.Lcd.print(getBatteryString(battery));
    M5.Lcd.setTextColor(WHITE, BLACK);
}
