#pragma once
#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <base64.h>

typedef struct {
    int     utcOffset;
    String  timezoneHash;
    bool    show;
    bool    is24h;
} ClockDataStruct;
