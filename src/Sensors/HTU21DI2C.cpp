#include "HTU21DI2C.h"

/**
 * @brief Construct a new HTU21DI2C::HTU21DI2C object
 * 
 * @param globalDataController  Handle to global data controller
 * @param debugController       Handle to debug controller
 * @param jsonRequestClient     Handle to json request instance
 */
HTU21DI2C::HTU21DI2C(GlobalDataController *globalDataController, DebugController *debugController, JsonRequestClient *jsonRequestClient)
: BaseSensorClientImpl("HTU21D(F) - Mode I2C", globalDataController, debugController, jsonRequestClient) {
}

/**
 * @brief Initialize the sensor
 * 
 * @param i2cInterface          Handle to I2C Interface
 * @param spiInterface          Handle to SPI Interface
 * @param spiCsPin              SPI chip select pin to use
 * @param oneWirePin            Pin to use for one wire communication
 */
void HTU21DI2C::initialize(TwoWire *i2cInterface, SPIClass *spiInterface, uint8_t spiCsPin, uint8_t oneWirePin) {
    this->htu = new Adafruit_HTU21DF(); 
}

/**
 * @brief start the sensor
 * @param sensorData            Handle to sensor data struct
 * @return True if started
 */
boolean HTU21DI2C::startSensor(SensorDataStruct *sensorData) {
    if (!this->htu->begin()) {
        MemoryHelper::stringToChar("Could not find a valid HTU21D sensor, check wiring!", sensorData->error, 120);
        sensorData->sensorIsRuning = false;
        return false;
    }
    sensorData->sensorIsRuning = true;
    return true;
}

/**
 * @brief stop the sensor
 */
void HTU21DI2C::endSensor() {
}

/**
 * @brief Read sensor data
 * @param sensorData            Handle to sensor data struct
 */
void HTU21DI2C::updateSensor(SensorDataStruct *sensorData) {
    if (!sensorData->sensorIsRuning) {
        return;
    }

    sensorData->temperature = this->htu->readTemperature();
    sensorData->humidity = this->htu->readHumidity();

    // Show some values
    this->debugController->printLn("Temp: " + String(sensorData->temperature));
    this->debugController->printLn("Humidity: " + String(sensorData->humidity));
}

/**
 * @brief Indicates if sensor measure temperature
 * @return boolean 
 */
boolean HTU21DI2C::hasTemperature() {
    return true;
}

/**
 * @brief Indicates if sensor measure humidity
 * @return boolean 
 */
boolean HTU21DI2C::hasHumidity() {
    return true;
}
