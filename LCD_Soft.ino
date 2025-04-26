#include <Adafruit_GFX.h>
#include <Adafruit_ILI9341.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>
#include <Wire.h>
#include <SPI.h>

// Pin definitions
#define SPI_SCK   13
#define SPI_SDO   12
#define SPI_SDI   11
#define DISP_CS   10
#define DISP_RST  9
#define DISP_DC   8
#define DISP_LED  6
#define BME_CS    7
#define BME_SDA   A4
#define BME_SCL   A5

// Display initialization
Adafruit_ILI9341 tft = Adafruit_ILI9341(DISP_CS, DISP_DC, DISP_RST);

// BME280 I2C definition
Adafruit_BME280 bme;

// Colors
#define BACKGROUND   0x0000  // Black
#define TEXT_COLOR   0xFFFF  // White
#define TEMP_COLOR   0xF800  // Red
#define HUM_COLOR    0x07FF  // Cyan
#define PRESS_COLOR  0xFFE0  // Yellow
#define GRID_COLOR   0x4208  // Dark gray

// Data recording
#define MAX_DATA_POINTS 48
float tempHistory[MAX_DATA_POINTS];
float humHistory[MAX_DATA_POINTS];
float pressHistory[MAX_DATA_POINTS];
int dataIndex = 0;
unsigned long lastUpdateTime = 0;
#define UPDATE_INTERVAL 5000  // 5 seconds between readings

// Graphics parameters
#define GRAPH_X      40
#define GRAPH_Y      150
#define GRAPH_W      260
#define GRAPH_H      80
#define GRAPH_PAD    5

void setup() {
  Serial.begin(9600);
  
  // Initialize display
  tft.begin();
  pinMode(DISP_LED, OUTPUT);
  digitalWrite(DISP_LED, HIGH);  // Turn on backlight
  tft.setRotation(3);  // Landscape mode
  tft.fillScreen(BACKGROUND);
  
  // Initialize BME280
  if (!bme.begin(0x76)) {  // Default I2C address
    drawError("BME280 not found!");
    while (1) delay(10);
  }
  
  // Initialize data arrays
  for (int i = 0; i < MAX_DATA_POINTS; i++) {
    tempHistory[i] = 0;
    humHistory[i] = 0;
    pressHistory[i] = 0;
  }
  
  // Draw static elements
  drawStaticElements();
  
  // Get initial readings
  readSensors();
}

void loop() {
  // Read sensors at regular intervals
  if (millis() - lastUpdateTime >= UPDATE_INTERVAL) {
    readSensors();
    updateDisplay();
    lastUpdateTime = millis();
  }
}

void readSensors() {
  // Read BME280 values
  float temperature = bme.readTemperature();
  float humidity = bme.readHumidity();
  float pressure = bme.readPressure() / 100.0F; // Convert to hPa
  
  // Store in history arrays
  tempHistory[dataIndex] = temperature;
  humHistory[dataIndex] = humidity;
  pressHistory[dataIndex] = pressure;
  
  // Move to next data point
  dataIndex = (dataIndex + 1) % MAX_DATA_POINTS;
}

void updateDisplay() {
  // Clear value display areas
  tft.fillRect(120, 20, 180, 100, BACKGROUND);
  
  // Display current values
  float currentTemp = tempHistory[(dataIndex + MAX_DATA_POINTS - 1) % MAX_DATA_POINTS];
  float currentHum = humHistory[(dataIndex + MAX_DATA_POINTS - 1) % MAX_DATA_POINTS];
  float currentPress = pressHistory[(dataIndex + MAX_DATA_POINTS - 1) % MAX_DATA_POINTS];
  
  // Temperature
  tft.setTextSize(3);
  tft.setTextColor(TEMP_COLOR);
  tft.setCursor(140, 25);
  tft.print(currentTemp, 1);
  tft.print(" C");
  
  // Humidity
  tft.setTextColor(HUM_COLOR);
  tft.setCursor(140, 55);
  tft.print(currentHum, 1);
  tft.print(" %");
  
  // Pressure
  tft.setTextColor(PRESS_COLOR);
  tft.setCursor(140, 85);
  tft.print(currentPress, 1);
  tft.print(" hPa");
  
  // Update weather icon based on conditions
  updateWeatherIcon(currentTemp, currentHum);
  
  // Update graphs
  drawGraphs();
}

void drawStaticElements() {
  // Title
  tft.setTextSize(2);
  tft.setTextColor(TEXT_COLOR);
  tft.setCursor(60, 3);
  tft.print("Weather Station");
  
  // Labels
  tft.setTextSize(2);
  tft.setTextColor(TEMP_COLOR);
  tft.setCursor(10, 30);
  tft.print("Temp:");
  
  tft.setTextColor(HUM_COLOR);
  tft.setCursor(10, 60);
  tft.print("Hum:");
  
  tft.setTextColor(PRESS_COLOR);
  tft.setCursor(10, 90);
  tft.print("Press:");
  
  // Draw graph frame
  tft.drawRect(GRAPH_X - GRAPH_PAD, GRAPH_Y - GRAPH_PAD, 
               GRAPH_W + GRAPH_PAD * 2, GRAPH_H + GRAPH_PAD * 2, GRID_COLOR);
}

void drawGraphs() {
  // Clear graph area
  tft.fillRect(GRAPH_X, GRAPH_Y, GRAPH_W, GRAPH_H, BACKGROUND);
  
  // Draw grid lines
  for (int i = 0; i < 5; i++) {
    int y = GRAPH_Y + (i * GRAPH_H / 4);
    tft.drawLine(GRAPH_X, y, GRAPH_X + GRAPH_W, y, GRID_COLOR);
  }
  
  for (int i = 0; i < 7; i++) {
    int x = GRAPH_X + (i * GRAPH_W / 6);
    tft.drawLine(x, GRAPH_Y, x, GRAPH_Y + GRAPH_H, GRID_COLOR);
  }
  
  // Find min/max for scaling
  float minTemp = 100, maxTemp = -40;
  float minHum = 100, maxHum = 0;
  float minPress = 1200, maxPress = 800;
  
  // Find actual min/max values for dynamic scaling
  for (int i = 0; i < MAX_DATA_POINTS; i++) {
    if (tempHistory[i] > 0) {
      minTemp = min(minTemp, tempHistory[i]);
      maxTemp = max(maxTemp, tempHistory[i]);
      minHum = min(minHum, humHistory[i]);
      maxHum = max(maxHum, humHistory[i]);
      minPress = min(minPress, pressHistory[i]);
      maxPress = max(maxPress, pressHistory[i]);
    }
  }
  
  // Add buffer to min/max
  minTemp -= 2; maxTemp += 2;
  minHum -= 5; maxHum += 5;
  minPress -= 5; maxPress += 5;
  
  // Draw temperature graph
  for (int i = 0; i < MAX_DATA_POINTS - 1; i++) {
    int idx1 = (dataIndex + i) % MAX_DATA_POINTS;
    int idx2 = (dataIndex + i + 1) % MAX_DATA_POINTS;
    
    // Only plot if we have valid data
    if (tempHistory[idx1] > 0 && tempHistory[idx2] > 0) {
      int x1 = GRAPH_X + (i * GRAPH_W / MAX_DATA_POINTS);
      int x2 = GRAPH_X + ((i + 1) * GRAPH_W / MAX_DATA_POINTS);
      
      int y1 = GRAPH_Y + GRAPH_H - (tempHistory[idx1] - minTemp) * GRAPH_H / (maxTemp - minTemp);
      int y2 = GRAPH_Y + GRAPH_H - (tempHistory[idx2] - minTemp) * GRAPH_H / (maxTemp - minTemp);
      
      tft.drawLine(x1, y1, x2, y2, TEMP_COLOR);
    }
  }
  
  // Draw humidity graph - slightly dimmer
  for (int i = 0; i < MAX_DATA_POINTS - 1; i++) {
    int idx1 = (dataIndex + i) % MAX_DATA_POINTS;
    int idx2 = (dataIndex + i + 1) % MAX_DATA_POINTS;
    
    if (humHistory[idx1] > 0 && humHistory[idx2] > 0) {
      int x1 = GRAPH_X + (i * GRAPH_W / MAX_DATA_POINTS);
      int x2 = GRAPH_X + ((i + 1) * GRAPH_W / MAX_DATA_POINTS);
      
      int y1 = GRAPH_Y + GRAPH_H - (humHistory[idx1] - minHum) * GRAPH_H / (maxHum - minHum);
      int y2 = GRAPH_Y + GRAPH_H - (humHistory[idx2] - minHum) * GRAPH_H / (maxHum - minHum);
      
      tft.drawLine(x1, y1, x2, y2, HUM_COLOR);
    }
  }
  
  // Draw pressure graph - dash pattern
  for (int i = 0; i < MAX_DATA_POINTS - 1; i++) {
    int idx1 = (dataIndex + i) % MAX_DATA_POINTS;
    int idx2 = (dataIndex + i + 1) % MAX_DATA_POINTS;
    
    if (pressHistory[idx1] > 0 && pressHistory[idx2] > 0) {
      int x1 = GRAPH_X + (i * GRAPH_W / MAX_DATA_POINTS);
      int x2 = GRAPH_X + ((i + 1) * GRAPH_W / MAX_DATA_POINTS);
      
      int y1 = GRAPH_Y + GRAPH_H - (pressHistory[idx1] - minPress) * GRAPH_H / (maxPress - minPress);
      int y2 = GRAPH_Y + GRAPH_H - (pressHistory[idx2] - minPress) * GRAPH_H / (maxPress - minPress);
      
      if (i % 2 == 0) { // Dash pattern
        tft.drawLine(x1, y1, x2, y2, PRESS_COLOR);
      }
    }
  }
}

void updateWeatherIcon(float temp, float humidity) {
  // Clear icon area
  tft.fillRect(10, 110, 100, 30, BACKGROUND);
  
  tft.setTextSize(1);
  tft.setTextColor(TEXT_COLOR);
  tft.setCursor(10, 120);
  
  // Simple weather icon logic
  if (humidity > 80) {
    tft.print("RAINY");
  } else if (humidity > 60) {
    tft.print("CLOUDY");
  } else if (temp > 28) {
    tft.print("HOT");
  } else if (temp < 10) {
    tft.print("COLD");
  } else {
    tft.print("FAIR");
  }
}

void drawError(const char* message) {
  tft.fillScreen(BACKGROUND);
  tft.setTextColor(0xF800); // Red
  tft.setTextSize(2);
  tft.setCursor(20, 100);
  tft.print(message);
}