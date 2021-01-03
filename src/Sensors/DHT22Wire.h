#pragma once
#include "Base/DHTxxSensorBase.h"
#include <Wire.h>
#include <Adafruit_Sensor.h>

/**
 * @brief Implementation for an DHT22 Sensor
 */
class DHT22Wire : public DHTxxSensorBase {
public:
    DHT22Wire(GlobalDataController *globalDataController, DebugController *debugController, JsonRequestClient *jsonRequestClient);
    void initialize(TwoWire *i2cInterface, SPIClass *spiInterface, uint8_t spiCsPin, uint8_t oneWirePin) override;
};
