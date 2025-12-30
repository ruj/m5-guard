#include <M5StickCPlus.h>
#include "config.h"
#include "tools/guard.h"
#include "tools/wifi.h"
#include "tools/ui.h"

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
    drawTopBar();
    showGuardCode(true);
}

void loop() {
    M5.update();

    drawTopBar();

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


