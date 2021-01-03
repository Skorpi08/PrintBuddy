#include "DHTxxSensorBase.h"

/**
 * @brief Construct a new DHTxxSensorBase::DHTxxSensorBase object
 * 
 * @param clientType            Sensor name
 * @param globalDataController  Handle to global data controller
 * @param debugController       Handle to debug controller
 * @param jsonRequestClient     Handle to json request instance
 */
DHTxxSensorBase::DHTxxSensorBase(String clientType, GlobalDataController *globalDataController, DebugController *debugController, JsonRequestClient *jsonRequestClient)
: BaseSensorClientImpl(clientType, globalDataController, debugController, jsonRequestClient) {
}

/**
 * @brief start the sensor
 * @param sensorData            Handle to sensor data struct
 * @return True if started
 */
boolean DHTxxSensorBase::startSensor(SensorDataStruct *sensorData) {
    this->dht->begin();
    sensorData->sensorIsRuning = true;
    return true;
}


/**
 * @brief stop the sensor
 */
void DHTxxSensorBase::endSensor() {
}

/**
 * @brief Read sensor data
 * @param sensorData            Handle to sensor data struct
 */
void DHTxxSensorBase::updateSensor(SensorDataStruct *sensorData) {
    if (!sensorData->sensorIsRuning) {
        return;
    }

    sensors_event_t event;
    this->dht->temperature().getEvent(&event);
    if (!isnan(event.temperature)) {
        sensorData->temperature = event.temperature;
    }

    this->dht->humidity().getEvent(&event);
    if (!isnan(event.relative_humidity)) {
        sensorData->humidity = event.relative_humidity;
    }

    // Show some values
    this->debugController->printLn("Temp: " + String(sensorData->temperature));
    this->debugController->printLn("Humidity: " + String(sensorData->humidity));
}

/**
 * @brief Indicates if sensor measure temperature
 * @return boolean 
 */
boolean DHTxxSensorBase::hasTemperature() {
    return true;
}

/**
 * @brief Indicates if sensor measure humidity
 * @return boolean 
 */
boolean DHTxxSensorBase::hasHumidity() {
    return true;
}