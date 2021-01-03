#pragma once
#include "../BaseSensorClientImpl.h"
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BME680.h>

/**
 * @brief Basic implementation for an BME680 Sensor (I2C and SPI)
 */
class BME680SensorBase : public BaseSensorClientImpl {
protected:
    Adafruit_BME680 *bme; 
    
    float hum_weighting = 0.25; // so hum effect is 25% of the total air quality score
    float gas_weighting = 0.75; // so gas effect is 75% of the total air quality score
    float hum_score, gas_score;
    float gas_reference = 250000;
    float hum_reference = 40;
    int   getgasreference_count = 0;

public:
    BME680SensorBase(String clientType, GlobalDataController *globalDataController, DebugController *debugController, JsonRequestClient *jsonRequestClient);
    boolean startSensor(SensorDataStruct *sensorData) override;
    void endSensor() override;
    void updateSensor(SensorDataStruct *sensorData) override;

    boolean hasTemperature() override;
    boolean hasHumidity() override;
    boolean hasPressure() override;
    boolean hasGasSensor() override;
    boolean hasAirQuality() override;
    boolean hasAltitude() override;
    String airQualityAsString(SensorDataStruct *sensorData) override;

protected:
    float calculateIAQScore(SensorDataStruct *sensorData);
    String calculateIAQ(float score);
    void getGasReference();
};
