#ifndef BATTERY_H
#define BATTERY_H

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

#endif
