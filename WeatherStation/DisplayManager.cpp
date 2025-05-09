#include "DisplayManager.h"

// Define strings in program memory
const char TITLE[] PROGMEM = "Weather Station";
const char AUTHOR1[] PROGMEM = "Pal Antonio";
const char AUTHOR2[] PROGMEM = "Hausi Alexandru";
const char TEMP_LABEL[] PROGMEM = "Temp:";
const char PRESS_LABEL[] PROGMEM = "Pres:";
const char ALT_LABEL[] PROGMEM = "Alt:";
const char TEMP_UNIT[] PROGMEM = " C";
const char PRESS_UNIT[] PROGMEM = " hPa";
const char ALT_UNIT[] PROGMEM = " m";
const char SEA_LEVEL_TEXT[] PROGMEM = "SEA LEVEL";
const char LOW_ALT_TEXT[] PROGMEM = "LOW ALT";
const char HIGH_ALT_TEXT[] PROGMEM = "HIGH ALT";

DisplayManager::DisplayManager() : tft(DISP_CS, DISP_DC, DISP_RST) {
}

void DisplayManager::begin() {
    tft.begin();
    tft.setRotation(2); // Portrait
    tft.fillScreen(BACKGROUND);
}

// Helper function to print PROGMEM strings
void printProgmemStr(Adafruit_ILI9341 &tft, const char* str) {
    char c;
    for (uint8_t i = 0; (c = pgm_read_byte(str + i)); i++) {
        tft.print(c);
    }
}

void DisplayManager::drawStaticElements() {
    tft.setTextSize(2);
    tft.setTextColor(TEXT_COLOR);
    tft.setCursor(30, 10);
    printProgmemStr(tft, TITLE);
    
    // Add names under the title
    tft.setTextSize(1);
    tft.setCursor(10, 30);
    printProgmemStr(tft, AUTHOR1);
    
    tft.setCursor(145, 30);
    printProgmemStr(tft, AUTHOR2);
    
    // Reset text size for the rest of the elements
    tft.setTextSize(2);
    tft.setTextColor(TEMP_COLOR);
    tft.setCursor(10, 50);
    printProgmemStr(tft, TEMP_LABEL);

    tft.setTextColor(PRESS_COLOR);
    tft.setCursor(10, 80);
    printProgmemStr(tft, PRESS_LABEL);

    tft.setTextColor(ALT_COLOR);
    tft.setCursor(10, 110);
    printProgmemStr(tft, ALT_LABEL);

    tft.drawRect(GRAPH_X - GRAPH_PAD, GRAPH_Y - GRAPH_PAD,
               GRAPH_W + GRAPH_PAD * 2, GRAPH_H + GRAPH_PAD * 2, GRID_COLOR);
}

void DisplayManager::updateDisplay(float currentTemp, float currentPress, float altitude, 
                                    float* tempHistory, float* pressHistory, int dataIndex, int maxDataPoints) {
    tft.fillRect(80, 40, 150, 100, BACKGROUND);
    
    tft.setTextSize(2);
    tft.setTextColor(TEMP_COLOR);
    tft.setCursor(80, 50);
    tft.print(currentTemp, 1);
    printProgmemStr(tft, TEMP_UNIT);

    tft.setTextColor(PRESS_COLOR);
    tft.setCursor(80, 80);
    tft.print(currentPress, 1);
    printProgmemStr(tft, PRESS_UNIT);

    tft.setTextColor(ALT_COLOR);
    tft.setCursor(80, 110);
    tft.print(altitude, 1);
    printProgmemStr(tft, ALT_UNIT);

    updateAltitudeText(altitude);
    drawGraphs(tempHistory, pressHistory, dataIndex, maxDataPoints);
}

void DisplayManager::updateAltitudeText(float alt) {
    tft.setTextSize(1);
    tft.setTextColor(ALT_COLOR);
    tft.fillRect(10, 135, 200, 10, BACKGROUND);
    tft.setCursor(10, 135);
    if (alt < 100) {
        printProgmemStr(tft, SEA_LEVEL_TEXT);
    } else if (alt < 1000) {
        printProgmemStr(tft, LOW_ALT_TEXT);
    } else {
        printProgmemStr(tft, HIGH_ALT_TEXT);
    }
}


void DisplayManager::drawGraphs(float* tempHistory, float* pressHistory, int dataIndex, int maxDataPoints) {
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
    float minPress = 1200, maxPress = 800;

    for (int i = 0; i < maxDataPoints; i++) {
        if (tempHistory[i] > 0) {
            minTemp = min(minTemp, tempHistory[i]);
            maxTemp = max(maxTemp, tempHistory[i]);
            minPress = min(minPress, pressHistory[i]);
            maxPress = max(maxPress, pressHistory[i]);
        }
    }

    minTemp -= 2; maxTemp += 2;
    minPress -= 5; maxPress += 5;

    // Draw the graph lines
    for (int i = 0; i < maxDataPoints - 1; i++) {
        int idx1 = (dataIndex + i) % maxDataPoints;
        int idx2 = (dataIndex + i + 1) % maxDataPoints;

        if (tempHistory[idx1] > 0 && tempHistory[idx2] > 0) {
            int x1 = GRAPH_X + (i * GRAPH_W / maxDataPoints);
            int x2 = GRAPH_X + ((i + 1) * GRAPH_W / maxDataPoints);

            int y1 = GRAPH_Y + GRAPH_H - (tempHistory[idx1] - minTemp) * GRAPH_H / (maxTemp - minTemp);
            int y2 = GRAPH_Y + GRAPH_H - (tempHistory[idx2] - minTemp) * GRAPH_H / (maxTemp - minTemp);

            tft.drawLine(x1, y1, x2, y2, TEMP_COLOR);
        }

        if (pressHistory[idx1] > 0 && pressHistory[idx2] > 0 && i % 2 == 0) {
            int x1 = GRAPH_X + (i * GRAPH_W / maxDataPoints);
            int x2 = GRAPH_X + ((i + 1) * GRAPH_W / maxDataPoints);

            int y1 = GRAPH_Y + GRAPH_H - (pressHistory[idx1] - minPress) * GRAPH_H / (maxPress - minPress);
            int y2 = GRAPH_Y + GRAPH_H - (pressHistory[idx2] - minPress) * GRAPH_H / (maxPress - minPress);

            tft.drawLine(x1, y1, x2, y2, PRESS_COLOR);
        }
    }
}
// Add new overloaded method for F() strings
void DisplayManager::drawError(const __FlashStringHelper* message) {
    tft.fillScreen(BACKGROUND);
    tft.setTextColor(0xF800);
    tft.setTextSize(2);
    tft.setCursor(20, 160);
    tft.print(message); // Adafruit_GFX can print flash strings directly
}
