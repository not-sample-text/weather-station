#include "DisplayManager.h"
#include "SensorManager.h"

DisplayManager display;
SensorManager sensors;

unsigned long lastUpdateTime = 0;
#define UPDATE_INTERVAL 2500

void setup() {
  Serial.begin(9600);
  
  display.begin();
  
  if (!sensors.begin()) {
    display.drawError(F("BMP280 not found!"));
    while (1) delay(10);
  }
  
  display.drawStaticElements();
  sensors.readSensors();
}

void loop() {
  if (millis() - lastUpdateTime >= UPDATE_INTERVAL) {
    sensors.readSensors();
    
    display.updateDisplay(
      sensors.getCurrentTemperature(),
      sensors.getCurrentPressure(),
      sensors.getCurrentAltitude(),
      sensors.getTemperatureHistory(),
      sensors.getPressureHistory(),
      sensors.getDataIndex(),
      MAX_DATA_POINTS
    );
    
    lastUpdateTime = millis();
  }
}
