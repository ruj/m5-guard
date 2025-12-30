#ifndef UI_H
#define UI_H

#include "battery.h"
#include "utilities.h"
#include "../config.h"

inline void drawTopBar() {
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

#endif