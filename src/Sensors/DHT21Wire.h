#pragma once
#include "Base/DHTxxSensorBase.h"
#include <Wire.h>
#include <Adafruit_Sensor.h>

/**
 * @brief Implementation for an DHT21 Sensor
 */
class DHT21Wire : public DHTxxSensorBase {
public:
    DHT21Wire(GlobalDataController *globalDataController, DebugController *debugController, JsonRequestClient *jsonRequestClient);
    void initialize(TwoWire *i2cInterface, SPIClass *spiInterface, uint8_t spiCsPin, uint8_t oneWirePin) override;
};
