#pragma once
#include "Base/DHTxxSensorBase.h"
#include <Wire.h>
#include <Adafruit_Sensor.h>

/**
 * @brief Implementation for an DHT11 Sensor
 */
class DHT11Wire : public DHTxxSensorBase {
public:
    DHT11Wire(GlobalDataController *globalDataController, DebugController *debugController, JsonRequestClient *jsonRequestClient);
    void initialize(TwoWire *i2cInterface, SPIClass *spiInterface, uint8_t spiCsPin, uint8_t oneWirePin) override;
};
