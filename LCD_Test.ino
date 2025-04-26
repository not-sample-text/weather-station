#include <Adafruit_GFX.h>
#include <Adafruit_ILI9341.h>

#define SPI_SCK   13
#define SPI_SDO   12
#define SPI_SDI   11
#define DISP_CS   10
#define DISP_RST  9
#define DISP_DC   8

Adafruit_ILI9341 tft = Adafruit_ILI9341(DISP_CS, DISP_DC, DISP_RST);

void setup() {
  Serial.begin(9600);
  Serial.println("Starting display test...");

  tft.begin();
  Serial.println("TFT initialized");

  tft.setRotation(1); // Landscape mode
  Serial.println("Rotation set to landscape");

  tft.fillScreen(ILI9341_BLACK);
  Serial.println("Screen cleared");

  tft.setTextColor(ILI9341_WHITE);
  tft.setTextSize(2);
  tft.setCursor(10, 10);
  tft.println("Display Test");
  Serial.println("Text printed on screen");

  delay(1000);

  // Draw some basic shapes
  tft.drawCircle(60, 80, 30, ILI9341_GREEN);
  Serial.println("Circle drawn");

  tft.drawRect(120, 60, 60, 40, ILI9341_RED);
  Serial.println("Rectangle drawn");

  tft.fillTriangle(200, 100, 220, 60, 240, 100, ILI9341_BLUE);
  Serial.println("Triangle drawn");

  delay(1000);

  // Draw lines from corners
  tft.fillScreen(ILI9341_BLACK);
  Serial.println("Screen cleared for line test");

  for (int x = 0; x < tft.width(); x += 10) {
    tft.drawLine(0, 0, x, tft.height(), ILI9341_YELLOW);
  }
  Serial.println("Line pattern drawn");

  delay(2000);
}

void loop() {
  // Nothing in loop for this test
}
