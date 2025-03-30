#ifndef UTILITIES_H
#define UTILITIES_H

String getTimeString() {
    struct tm timeinfo;

    if (!getLocalTime(&timeinfo)) {
        M5.Lcd.println("Failed to obtain time");

        return "ERROR";
    }

    char time[9];

    snprintf(time, sizeof(time), "%02d:%02d:%02d", timeinfo.tm_hour, timeinfo.tm_min, timeinfo.tm_sec);

    return String(time);
}

#endif
