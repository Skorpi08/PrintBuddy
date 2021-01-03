#include "BME680SensorBase.h"

/**
 * @brief Construct a new BME680SensorBase::BME680SensorBase object
 * 
 * @param clientType            Sensor name
 * @param globalDataController  Handle to global data controller
 * @param debugController       Handle to debug controller
 * @param jsonRequestClient     Handle to json request instance
 */
BME680SensorBase::BME680SensorBase(String clientType, GlobalDataController *globalDataController, DebugController *debugController, JsonRequestClient *jsonRequestClient)
: BaseSensorClientImpl(clientType, globalDataController, debugController, jsonRequestClient) {
}

/**
 * @brief start the sensor
 * @param sensorData            Handle to sensor data struct
 * @return True if started
 */
boolean BME680SensorBase::startSensor(SensorDataStruct *sensorData) {
    if (!this->bme->begin()) {
        MemoryHelper::stringToChar("Could not find a valid BME680 sensor, check wiring!", sensorData->error, 120);
        sensorData->sensorIsRuning = false;
        return false;
    }
    sensorData->sensorIsRuning = true;

    // Set up oversampling and filter initialization
    this->bme->setTemperatureOversampling(BME680_OS_4X);
    this->bme->setHumidityOversampling(BME680_OS_4X);
    this->bme->setPressureOversampling(BME680_OS_4X);
    this->bme->setIIRFilterSize(BME680_FILTER_SIZE_3);
    this->bme->setGasHeater(320, 150); // 320*C for 150 ms

    this->getGasReference();
    return true;
}

/**
 * @brief stop the sensor
 */
void BME680SensorBase::endSensor() {
}

/**
 * @brief Read sensor data
 * @param sensorData            Handle to sensor data struct
 */
void BME680SensorBase::updateSensor(SensorDataStruct *sensorData) {
    if (!sensorData->sensorIsRuning) {
        return;
    }
    if (!this->bme->performReading()) {
        MemoryHelper::stringToChar("Failed to perform reading BME680", sensorData->error, 120);
        return;
    }
    if ((getgasreference_count++)%10==0) this->getGasReference(); 

    sensorData->temperature = this->bme->temperature;
    sensorData->pressure = this->bme->pressure / 100.0;
    sensorData->humidity = this->bme->humidity;
    sensorData->airQuality = this->calculateIAQScore(sensorData);
    sensorData->gasResistance = this->bme->gas_resistance / 1000.0;
    sensorData->altitude = this->pressureToAltitude(sensorData->pressure, sensorData->temperature);

    // Show some values
    this->debugController->printLn("Temp: " + String(sensorData->temperature));
    this->debugController->printLn("Humidity: " + String(sensorData->humidity));
    this->debugController->printLn("Pressure: " + String(sensorData->pressure));
    this->debugController->printLn("Altitude: " + String(sensorData->altitude));
    this->debugController->printLn("AIR Q..." + this->airQualityAsString(sensorData));
}

/**
 * @brief Indicates if sensor measure temperature
 * @return boolean 
 */
boolean BME680SensorBase::hasTemperature() {
    return true;
}

/**
 * @brief Indicates if sensor measure humidity
 * @return boolean 
 */
boolean BME680SensorBase::hasHumidity() {
    return true;
}

/**
 * @brief Indicates if sensor measure pressure
 * @return boolean 
 */
boolean BME680SensorBase::hasPressure() {
    return true;
}

/**
 * @brief Indicates if sensor measure gas
 * @return boolean 
 */
boolean BME680SensorBase::hasGasSensor() {
    return true;
}

/**
 * @brief Indicates if sensor measure air quality
 * @return boolean 
 */
boolean BME680SensorBase::hasAirQuality() {
    return true;
}

/**
 * @brief Indicates if sensor measure altitude
 * @return boolean 
 */
boolean BME680SensorBase::hasAltitude() {
    return true;
}

/**
 * @brief Calculating humidity and gas sensor value to an air quality index value
 * @param sensorData            Handle to sensor data struct
 * @return float 
 */
float BME680SensorBase::calculateIAQScore(SensorDataStruct *sensorData) {
    // Calculate humidity contribution to IAQ index
    float current_humidity = sensorData->humidity;
    if (current_humidity >= 38 && current_humidity <= 42) {
        // Humidity +/-5% around optimum 
        this->hum_score = 0.25*100; 
    } else { 
        // sub-optimal
        if (current_humidity < 38) {
            this->hum_score = 0.25/this->hum_reference*current_humidity*100;
        } else {
            this->hum_score = ((-0.25/(100-this->hum_reference)*current_humidity)+0.416666)*100;
        }
    }
  
    // Calculate gas contribution to IAQ index
    float gas_lower_limit = 5000;   // Bad air quality limit
    float gas_upper_limit = 50000;  // Good air quality limit 
    if (this->gas_reference > gas_upper_limit) this->gas_reference = gas_upper_limit; 
    if (this->gas_reference < gas_lower_limit) this->gas_reference = gas_lower_limit;
    this->gas_score = (0.75/(gas_upper_limit-gas_lower_limit)*this->gas_reference -(gas_lower_limit*(0.75/(gas_upper_limit-gas_lower_limit))))*100;
  
    // Combine results for the final IAQ index value (0-100% where 100% is good quality air)
    return this->hum_score + this->gas_score;    
}

/**
 * @brief Run the sensor for a burn-in period, then use combination of relative humidity and gas resistance to estimate indoor air quality as a percentage.
 */
void BME680SensorBase::getGasReference() {
    int readings = 10;
    for (int i = 1; i <= readings; i++) { // read gas for 10 x 0.150mS = 1.5secs
        this->gas_reference += this->bme->readGas();
    }
    this->gas_reference = this->gas_reference / readings;
}

/**
 * @brief Return air qualitiy as string
 * @param sensorData            Handle to sensor data struct
 * @return String 
 */
String BME680SensorBase::airQualityAsString(SensorDataStruct *sensorData) {
    float score = this->calculateIAQScore(sensorData);
    score = (100-score)*5;
    if      (score >= 301)                  return "Hazardous";
    else if (score >= 201 && score <= 300 ) return "Very Unhealthy";
    else if (score >= 176 && score <= 200 ) return "Unhealthy";
    else if (score >= 151 && score <= 175 ) return "Unhealthy for Sensitive Groups";
    else if (score >=  51 && score <= 150 ) return "Moderate";
    return "Good";
}
