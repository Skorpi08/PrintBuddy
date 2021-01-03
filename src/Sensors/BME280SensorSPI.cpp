#include "BME280SensorSPI.h"

/**
 * @brief Construct a new BME280SensorSPI::BME280SensorSPI object
 * 
 * @param globalDataController  Handle to global data controller
 * @param debugController       Handle to debug controller
 * @param jsonRequestClient     Handle to json request instance
 */
BME280SensorSPI::BME280SensorSPI(GlobalDataController *globalDataController, DebugController *debugController, JsonRequestClient *jsonRequestClient)
: BME280SensorBase("BME280 - Mode SPI", globalDataController, debugController, jsonRequestClient) {
}

/**
 * @brief Initialize the sensor
 * 
 * @param i2cInterface          Handle to I2C Interface
 * @param spiInterface          Handle to SPI Interface
 * @param spiCsPin              SPI chip select pin to use
 * @param oneWirePin            Pin to use for one wire communication
 */
void BME280SensorSPI::initialize(TwoWire *i2cInterface, SPIClass *spiInterface, uint8_t spiCsPin, uint8_t oneWirePin) {
    this->bme = new Adafruit_BME280(spiCsPin, spiInterface); 
}

/**
 * @brief start the sensor
 * @param sensorData            Handle to sensor data struct
 * @return True if started
 */
boolean BME280SensorSPI::startSensor(SensorDataStruct *sensorData) {
    if (!this->bme->begin()) {
        MemoryHelper::stringToChar("Could not find a valid BME280 sensor, check wiring!", sensorData->error, 120);
        sensorData->sensorIsRuning = false;
        return false;
    }
    sensorData->sensorIsRuning = true;

    this->bme->setSampling(Adafruit_BME280::MODE_FORCED,
        Adafruit_BME280::SAMPLING_X4, // temperature
        Adafruit_BME280::SAMPLING_X4, // pressure
        Adafruit_BME280::SAMPLING_X4, // humidity
        Adafruit_BME280::FILTER_OFF
    );
    return true;
}