#include "SensorManager.h"

SensorManager::SensorManager() : dataIndex(0), sealevelPressure(1013.25) {
    // Initialize history arrays
    for (int i = 0; i < MAX_DATA_POINTS; i++) {
        tempHistory[i] = 0;
        pressHistory[i] = 0;
    }
}

bool SensorManager::begin() {
    return bmp.begin(0x76);  // Returns true if sensor is found and initialized
}

void SensorManager::readSensors() {
    float temperature = bmp.readTemperature();
    float pressure = bmp.readPressure() / 100.0F;  // Convert Pa to hPa

    tempHistory[dataIndex] = temperature;
    pressHistory[dataIndex] = pressure;

    dataIndex = (dataIndex + 1) % MAX_DATA_POINTS;

    // Debug output with F() macro to save RAM
    Serial.print(F("Temp: ")); 
    Serial.print(temperature);
    Serial.print(F(" C, Pressure: ")); 
    Serial.print(pressure); 
    Serial.println(F(" hPa"));
}
float SensorManager::getCurrentTemperature() {
    int lastIndex = (dataIndex + MAX_DATA_POINTS - 1) % MAX_DATA_POINTS;
    return tempHistory[lastIndex];
}

float SensorManager::getCurrentPressure() {
    int lastIndex = (dataIndex + MAX_DATA_POINTS - 1) % MAX_DATA_POINTS;
    return pressHistory[lastIndex];
}

float SensorManager::getCurrentAltitude() {
    return bmp.readAltitude(sealevelPressure);
}

float* SensorManager::getTemperatureHistory() {
    return tempHistory;
}

float* SensorManager::getPressureHistory() {
    return pressHistory;
}

int SensorManager::getDataIndex() {
    return dataIndex;
}
