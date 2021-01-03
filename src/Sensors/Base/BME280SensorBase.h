#pragma once
#include "../BaseSensorClientImpl.h"
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>

/**
 * @brief Basic implementation for an BME280 Sensor (I2C and SPI)
 */
class BME280SensorBase : public BaseSensorClientImpl {
protected:
    Adafruit_BME280 *bme; 

public:
    BME280SensorBase(String clientType, GlobalDataController *globalDataController, DebugController *debugController, JsonRequestClient *jsonRequestClient);
    void endSensor() override;
    void updateSensor(SensorDataStruct *sensorData) override;

    boolean hasTemperature() override;
    boolean hasHumidity() override;
    boolean hasPressure() override;
    boolean hasAltitude() override;
};
