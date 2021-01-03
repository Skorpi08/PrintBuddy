#pragma once
#include "Base/BME280SensorBase.h"
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>

/**
 * @brief Implementation for an BME280 Sensor
 * We are using I2C mode for this sensor
 */
class BME280SensorI2C : public BME280SensorBase {
private:
    TwoWire *i2cInterface;
    
public:
    BME280SensorI2C(GlobalDataController *globalDataController, DebugController *debugController, JsonRequestClient *jsonRequestClient);
    void initialize(TwoWire *i2cInterface, SPIClass *spiInterface, uint8_t spiCsPin, uint8_t oneWirePin) override;
    boolean startSensor(SensorDataStruct *sensorData) override;
};
