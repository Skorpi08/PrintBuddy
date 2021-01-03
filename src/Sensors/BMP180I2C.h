#pragma once
#include "BaseSensorClientImpl.h"
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BMP085_U.h>

/**
 * @brief Implementation for an BMP180 Sensor (I2C)
 */
class BMP180I2C : public BaseSensorClientImpl {
protected:
    Adafruit_BMP085_Unified *bmp;
    TwoWire *i2cInterface;

public:
    BMP180I2C(GlobalDataController *globalDataController, DebugController *debugController, JsonRequestClient *jsonRequestClient);
    void initialize(TwoWire *i2cInterface, SPIClass *spiInterface, uint8_t spiCsPin, uint8_t oneWirePin) override;
    boolean startSensor(SensorDataStruct *sensorData) override;
    void endSensor() override;
    void updateSensor(SensorDataStruct *sensorData) override;

    boolean hasTemperature() override;
    boolean hasPressure() override;
    boolean hasAltitude() override;
};
