#ifndef BATTERY_H
#define BATTERY_H

#include <M5StickCPlus.h>
#include "../config.h"

int getBattery() {
    float batteryValue = M5.Axp.GetVbatData() * 1.1 / 1000;
    int battery = ((batteryValue - 3.0) / 1.2) * 100;

    if (battery > 100 || battery < 0) {
        return -1;
    }

    return battery;
}

String getBatteryString(int battery) {
    if (battery == -1) {
        return "...";
    }

    return String(battery) + "%";
}

void showBattery() {
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

#endif
