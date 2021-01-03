#include "BME280SensorBase.h"

/**
 * @brief Construct a new BME280SensorBase::BME280SensorBase object
 * 
 * @param clientType            Sensor name
 * @param globalDataController  Handle to global data controller
 * @param debugController       Handle to debug controller
 * @param jsonRequestClient     Handle to json request instance
 */
BME280SensorBase::BME280SensorBase(String clientType, GlobalDataController *globalDataController, DebugController *debugController, JsonRequestClient *jsonRequestClient)
: BaseSensorClientImpl(clientType, globalDataController, debugController, jsonRequestClient) {
}

/**
 * @brief stop the sensor
 */
void BME280SensorBase::endSensor() {
}

/**
 * @brief Read sensor data
 * @param sensorData            Handle to sensor data struct
 */
void BME280SensorBase::updateSensor(SensorDataStruct *sensorData) {
    if (!sensorData->sensorIsRuning) {
        return;
    }
    this->bme->takeForcedMeasurement();
    sensorData->temperature = this->bme->readTemperature();
    sensorData->pressure = this->bme->readPressure() / 100.0;
    sensorData->humidity = this->bme->readHumidity();
    sensorData->altitude = this->pressureToAltitude(sensorData->pressure, sensorData->temperature);

    // Show some values
    this->debugController->printLn("Temp: " + String(sensorData->temperature));
    this->debugController->printLn("Humidity: " + String(sensorData->humidity));
    this->debugController->printLn("Pressure: " + String(sensorData->pressure));
    this->debugController->printLn("Altitude: " + String(sensorData->altitude));
}

/**
 * @brief Indicates if sensor measure temperature
 * @return boolean 
 */
boolean BME280SensorBase::hasTemperature() {
    return true;
}

/**
 * @brief Indicates if sensor measure humidity
 * @return boolean 
 */
boolean BME280SensorBase::hasHumidity() {
    return true;
}

/**
 * @brief Indicates if sensor measure pressure
 * @return boolean 
 */
boolean BME280SensorBase::hasPressure() {
    return true;
}

/**
 * @brief Indicates if sensor measure altitude
 * @return boolean 
 */
boolean BME280SensorBase::hasAltitude() {
    return true;
}
