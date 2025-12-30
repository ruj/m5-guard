#ifndef CONFIG_H
#define CONFIG_H

struct WifiCredential {
    const char* SSID;
    const char* PASSWORD;
};

extern const WifiCredential WIFI_NETWORKS[];
extern const int WIFI_NETWORK_SIZE;

extern const char* SECRET;

#define SCREEN_WIDTH 240
#define SCREEN_HEIGHT 135

#endif
