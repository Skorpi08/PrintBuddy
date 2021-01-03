#include "BMP180I2C.h"

/**
 * @brief Construct a new BMP180I2C::BMP180I2C object
 * 
 * @param globalDataController  Handle to global data controller
 * @param debugController       Handle to debug controller
 * @param jsonRequestClient     Handle to json request instance
 */
BMP180I2C::BMP180I2C(GlobalDataController *globalDataController, DebugController *debugController, JsonRequestClient *jsonRequestClient)
: BaseSensorClientImpl("BMP085/BMP180 - Mode I2C", globalDataController, debugController, jsonRequestClient) {
}

/**
 * @brief Initialize the sensor
 * 
 * @param i2cInterface          Handle to I2C Interface
 * @param spiInterface          Handle to SPI Interface
 * @param spiCsPin              SPI chip select pin to use
 * @param oneWirePin            Pin to use for one wire communication
 */
void BMP180I2C::initialize(TwoWire *i2cInterface, SPIClass *spiInterface, uint8_t spiCsPin, uint8_t oneWirePin) {
    this->bmp = new Adafruit_BMP085_Unified(10085);
    this->i2cInterface = i2cInterface;
}

/**
 * @brief start the sensor
 * @param sensorData            Handle to sensor data struct
 * @return True if started
 */
boolean BMP180I2C::startSensor(SensorDataStruct *sensorData) {
    if (!this->bmp->begin(BMP085_MODE_STANDARD)) {
        MemoryHelper::stringToChar("Could not find a valid BMP085/BMP180 sensor, check wiring!", sensorData->error, 120);
        sensorData->sensorIsRuning = false;
        return false;
    }
    sensorData->sensorIsRuning = true;
    return true;
}

/**
 * @brief stop the sensor
 */
void BMP180I2C::endSensor() {
}

/**
 * @brief Read sensor data
 * @param sensorData            Handle to sensor data struct
 */
void BMP180I2C::updateSensor(SensorDataStruct *sensorData) {
    if (!sensorData->sensorIsRuning) {
        return;
    }

    sensors_event_t event;
    this->bmp->getEvent(&event);
    if (event.pressure)
    {
        this->bmp->getTemperature(&sensorData->temperature);
        sensorData->pressure = event.pressure;
        sensorData->altitude = this->pressureToAltitude(sensorData->pressure, sensorData->temperature);

        // Show some values
        this->debugController->printLn("Temp: " + String(sensorData->temperature));
        this->debugController->printLn("Pressure: " + String(sensorData->pressure));
        this->debugController->printLn("Altitude: " + String(sensorData->altitude));
    }
}

/**
 * @brief Indicates if sensor measure temperature
 * @return boolean 
 */
boolean BMP180I2C::hasTemperature() {
    return true;
}

/**
 * @brief Indicates if sensor measure pressure
 * @return boolean 
 */
boolean BMP180I2C::hasPressure() {
    return true;
}

/**
 * @brief Indicates if sensor measure altitude
 * @return boolean 
 */
boolean BMP180I2C::hasAltitude() {
    return true;
}
