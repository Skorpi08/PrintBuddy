#pragma once
#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <base64.h>
#include <SPI.h>
#include <Wire.h>
#include "Debug.h"
#include "../Network/JsonRequestClient.h"
#include "../DataStructs/SensorDataStruct.h"
#include "../../include/MemoryHelper.h"

/**
 * @brief Basic function definitions for an sensor client like an interface
 */
class BaseSensorClient {
public:
    virtual String getType() = 0;
    virtual void initialize(TwoWire *i2cInterface, SPIClass *spiInterface, uint8_t spiCsPin, uint8_t oneWirePin) = 0;
    virtual boolean startSensor(SensorDataStruct *sensorData) = 0;
    virtual void endSensor() = 0;
    virtual void updateSensor(SensorDataStruct *sensorData) = 0;

    virtual boolean hasTemperature() = 0;
    virtual boolean hasHumidity() = 0;
    virtual boolean hasPressure() = 0;
    virtual boolean hasGasSensor() = 0;
    virtual boolean hasAirQuality() = 0;
    virtual boolean hasAltitude() = 0;
    virtual String airQualityAsString(SensorDataStruct *sensorData) = 0;

    virtual String getTemperatureRounded(SensorDataStruct *sensorData) = 0;
    virtual String getHumidityRounded(SensorDataStruct *sensorData) = 0;

    /**
     * @brief Reset all dynamic variables for sensor
     * @param sensorData        Handle to sensor struct
     */
    static void resetData(SensorDataStruct *sensorData) {
        sensorData->activated = false;
        sensorData->showOnDisplay = false;
        sensorData->pressure = 0;
        sensorData->humidity = 0;
        sensorData->gasResistance = 0;
        sensorData->airQuality = 0;
        sensorData->temperature = 0;
        sensorData->sensType = 0;
        sensorData->altitude = 0;
        MemoryHelper::stringToChar("", sensorData->error, 120);
    }
};