#pragma once
#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <base64.h>

#define SENSOR_CLIENT_BME280_I2C       (int)0
#define SENSOR_CLIENT_BME280_SPI       (int)1
#define SENSOR_CLIENT_BME680_I2C       (int)2
#define SENSOR_CLIENT_BME680_SPI       (int)3
#define SENSOR_CLIENT_HTU21D_I2C       (int)4
#define SENSOR_CLIENT_BMP085_180_I2C   (int)5
#define SENSOR_CLIENT_DHT11_WIRE       (int)6
#define SENSOR_CLIENT_DHT21_WIRE       (int)7
#define SENSOR_CLIENT_DHT22_WIRE       (int)8

typedef struct {
    bool    activated;
    bool    showOnDisplay;
    int     sensType;
    char    error[120];
    bool    sensorIsRuning;
    long    lastSyncEpoch;
    float   temperature;
    float   pressure;
    float   humidity;
    float   airQuality;
    float   gasResistance;
    float   altitude;
} SensorDataStruct;
