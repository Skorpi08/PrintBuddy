#pragma once
#include "Base/BME680SensorBase.h"
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BME680.h>

/**
 * @brief Implementation for an BME680 Sensor
 * We are using I2C mode for this sensor
 */
class BME680SensorI2C : public BME680SensorBase {
public:
    BME680SensorI2C(GlobalDataController *globalDataController, DebugController *debugController, JsonRequestClient *jsonRequestClient);
    void initialize(TwoWire *i2cInterface, SPIClass *spiInterface, uint8_t spiCsPin, uint8_t oneWirePin) override;
};
