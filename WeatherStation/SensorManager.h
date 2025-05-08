#ifndef SENSOR_MANAGER_H
#define SENSOR_MANAGER_H

#include <Adafruit_Sensor.h>
#include <Adafruit_BMP280.h>
#include <Wire.h>
#include <SPI.h>

#define MAX_DATA_POINTS 50

class SensorManager {
public:
    SensorManager();
    bool begin();
    void readSensors();
    float getCurrentTemperature();
    float getCurrentPressure();
    float getCurrentAltitude();
    float* getTemperatureHistory();
    float* getPressureHistory();
    int getDataIndex();

private:
    Adafruit_BMP280 bmp;
    float tempHistory[MAX_DATA_POINTS];
    float pressHistory[MAX_DATA_POINTS];
    int dataIndex;
    float sealevelPressure;
};

#endif // SENSOR_MANAGER_H
