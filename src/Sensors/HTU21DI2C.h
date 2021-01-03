#pragma once
#include "BaseSensorClientImpl.h"
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_HTU21DF.h>

/**
 * @brief Implementation for an HTU21D Sensor (I2C)
 */
class HTU21DI2C : public BaseSensorClientImpl {
protected:
    Adafruit_HTU21DF *htu;

public:
    HTU21DI2C(GlobalDataController *globalDataController, DebugController *debugController, JsonRequestClient *jsonRequestClient);
    void initialize(TwoWire *i2cInterface, SPIClass *spiInterface, uint8_t spiCsPin, uint8_t oneWirePin) override;
    boolean startSensor(SensorDataStruct *sensorData) override;
    void endSensor() override;
    void updateSensor(SensorDataStruct *sensorData) override;

    boolean hasTemperature() override;
    boolean hasHumidity() override;
};
