#pragma once
#include "../BaseSensorClientImpl.h"
#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <DHT_U.h>

/**
 * @brief Basic implementation for an DHT Sensor
 */
class DHTxxSensorBase : public BaseSensorClientImpl {
protected:
    DHT_Unified *dht;

public:
    DHTxxSensorBase(String clientType, GlobalDataController *globalDataController, DebugController *debugController, JsonRequestClient *jsonRequestClient);
    boolean startSensor(SensorDataStruct *sensorData) override;
    void endSensor() override;
    void updateSensor(SensorDataStruct *sensorData) override;

    boolean hasTemperature() override;
    boolean hasHumidity() override;
};
