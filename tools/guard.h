#ifndef GUARD_H
#define GUARD_H

#include <M5StickCPlus.h>
#include <mbedtls/md.h>
#include <mbedtls/base64.h>
#include <time.h>
#include "../config.h"

const char *NTP_SERVER = "pool.ntp.org";
const long GMT_OFFSET = 0;
const int DAYLIGHT_OFFSET = 0;
const char *CHARSET = "23456789BCDFGHJKMNPQRTVWXY";

void syncTime() {
    configTime(GMT_OFFSET, DAYLIGHT_OFFSET, NTP_SERVER);

    struct tm timeinfo;
    while (!getLocalTime(&timeinfo)) {
        M5.Lcd.println("Waiting for pool.ntp.org synchronization");
        delay(1000);
    }
    M5.Lcd.println("Synchronized time");
    delay(1000);
}

uint32_t getUnixTime() {
    struct tm timeinfo;
    if (!getLocalTime(&timeinfo)) {
        M5.Lcd.println("Error getting time");
        return 0;
    }
    return mktime(&timeinfo);
}

void hmacSha1(const uint8_t *key, size_t keySize, const uint8_t *data, size_t dataSize, uint8_t *output) {
    mbedtls_md_context_t ctx;
    mbedtls_md_type_t mdType = MBEDTLS_MD_SHA1;

    mbedtls_md_init(&ctx);
    mbedtls_md_setup(&ctx, mbedtls_md_info_from_type(mdType), 1);
    mbedtls_md_hmac_starts(&ctx, key, keySize);
    mbedtls_md_hmac_update(&ctx, data, dataSize);
    mbedtls_md_hmac_finish(&ctx, output);
    mbedtls_md_free(&ctx);
}

String generateSteamCode(const char *secret) {
    uint8_t key[32];
    size_t keySize = 0;

    if (mbedtls_base64_decode(NULL, 0, &keySize, (const unsigned char *)secret, strlen(secret)) != MBEDTLS_ERR_BASE64_BUFFER_TOO_SMALL) {
        M5.Lcd.println("Error calculating Base64 key size");
        return "ERROR";
    }

    if (mbedtls_base64_decode(key, sizeof(key), &keySize, (const unsigned char *)secret, strlen(secret)) != 0) {
        M5.Lcd.println("Error decoding Base64 key");
        return "ERROR";
    }

    uint32_t time = getUnixTime() / 30;
    uint8_t timeBytes[8];
    for (int i = 7; i >= 0; i--) {
        timeBytes[i] = time & 0xFF;
        time >>= 8;
    }

    uint8_t hash[20];
    hmacSha1(key, keySize, timeBytes, sizeof(timeBytes), hash);

    int offset = hash[19] & 0x0F;
    uint32_t fullCode = ((hash[offset] & 0x7F) << 24) | (hash[offset + 1] << 16) | (hash[offset + 2] << 8) | hash[offset + 3];

    String code = "";
    for (int i = 0; i < 5; i++) {
        code += CHARSET[fullCode % strlen(CHARSET)];
        fullCode /= strlen(CHARSET);
    }

    return code;
}

void showCode() {
    M5.Lcd.fillScreen(BLACK);

    String code = generateSteamCode(SECRET);

    M5.Lcd.setTextSize(4);
    M5.Lcd.setTextDatum(CC_DATUM);
    M5.Lcd.drawString(code, M5.Lcd.width() / 2, M5.Lcd.height() / 2, 2);
}

#endif
