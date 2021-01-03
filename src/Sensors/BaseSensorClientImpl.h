#pragma once
#include "BaseSensorClient.h"
#include "../Global/GlobalDataController.h"

/**
 * @brief Basic implementations for an sensor client with needed data
 */
class BaseSensorClientImpl : public BaseSensorClient {
protected:
    GlobalDataController *globalDataController;
    DebugController *debugController;
    JsonRequestClient *jsonRequestClient;
    String clientType = "BME680";
    
public:
    BaseSensorClientImpl(String clientType, GlobalDataController *globalDataController, DebugController *debugController, JsonRequestClient *jsonRequestClient);
    String getType();
    void initialize(TwoWire *i2cInterface, SPIClass *spiInterface, uint8_t spiCsPin, uint8_t oneWirePin) {};
    boolean startSensor(SensorDataStruct *sensorData) { return false; };
    void endSensor() {};
    void updateSensor(SensorDataStruct *sensorData) {};
    
    boolean hasTemperature() { return false; };
    boolean hasHumidity() { return false; };
    boolean hasPressure() { return false; };
    boolean hasAirQuality() { return false; };
    boolean hasGasSensor() { return false; };
    boolean hasAltitude() { return false; };
    String airQualityAsString(SensorDataStruct *sensorData) { return ""; };

    String getTemperatureRounded(SensorDataStruct *sensorData) { return roundValue(sensorData->temperature); };
    String getHumidityRounded(SensorDataStruct *sensorData) { return roundValue(sensorData->humidity); };

protected:
    float pressureToAltitude(float pressure, float temp);
    String roundValue(float value) {
        int rounded = (int)(value+0.5f);
        return String(rounded);
    };
};