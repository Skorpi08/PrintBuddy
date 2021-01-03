#pragma once
#include "Base/BME280SensorBase.h"
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>

/**
 * @brief Implementation for an BME280 Sensor
 * We are using SPI mode for this sensor
 */
class BME280SensorSPI : public BME280SensorBase {   
public:
    BME280SensorSPI(GlobalDataController *globalDataController, DebugController *debugController, JsonRequestClient *jsonRequestClient);
    void initialize(TwoWire *i2cInterface, SPIClass *spiInterface, uint8_t spiCsPin, uint8_t oneWirePin) override;
    boolean startSensor(SensorDataStruct *sensorData) override;
};
