#include "DisplayManager.h"

DisplayManager::DisplayManager() : tft(DISP_CS, DISP_DC, DISP_RST) {
}

void DisplayManager::begin() {
    tft.begin();
    tft.setRotation(2); // Portrait
    tft.fillScreen(BACKGROUND);
}

void DisplayManager::drawStaticElements() {
    tft.setTextSize(2);
    tft.setTextColor(TEXT_COLOR);
    tft.setCursor(30, 10);
    tft.print("Weather Station");
    
    // Add names under the title
    tft.setTextSize(1);
    tft.setCursor(10, 30);
    tft.print("Pal Antonio");
    
    tft.setCursor(145, 30);
    tft.print("Hausi Alexandru");
    
    // Reset text size for the rest of the elements
    tft.setTextSize(2);
    tft.setTextColor(TEMP_COLOR);
    tft.setCursor(10, 50);
    tft.print("Temp:");

    tft.setTextColor(PRESS_COLOR);
    tft.setCursor(10, 80);
    tft.print("Pres:");

    tft.setTextColor(ALT_COLOR);
    tft.setCursor(10, 110);
    tft.print("Alt:");

    tft.drawRect(GRAPH_X - GRAPH_PAD, GRAPH_Y - GRAPH_PAD,
               GRAPH_W + GRAPH_PAD * 2, GRAPH_H + GRAPH_PAD * 2, GRID_COLOR);
}

void DisplayManager::updateDisplay(float currentTemp, float currentPress, float altitude, 
                                    float* tempHistory, float* pressHistory, int dataIndex, int maxDataPoints) {
    tft.fillRect(80, 40, 150, 100, BACKGROUND);
    
    tft.setTextSize(2);
    tft.setTextColor(TEMP_COLOR);
    tft.setCursor(80, 50);
    tft.print(currentTemp, 1); tft.print(" C");

    tft.setTextColor(PRESS_COLOR);
    tft.setCursor(80, 80);
    tft.print(currentPress, 1); tft.print(" hPa");

    tft.setTextColor(ALT_COLOR);
    tft.setCursor(80, 110);
    tft.print(altitude, 1); tft.print(" m");

    updateAltitudeText(altitude);
    drawGraphs(tempHistory, pressHistory, dataIndex, maxDataPoints);
}

void DisplayManager::updateAltitudeText(float alt) {
    tft.setTextSize(1);
    tft.setTextColor(ALT_COLOR);
    tft.fillRect(10, 135, 200, 10, BACKGROUND);
    tft.setCursor(10, 135);
    if (alt < 100) {
        tft.print("SEA LEVEL");
    } else if (alt < 1000) {
        tft.print("LOW ALT");
    } else {
        tft.print("HIGH ALT");
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

void DisplayManager::drawError(const char* message) {
    tft.fillScreen(BACKGROUND);
    tft.setTextColor(0xF800);
    tft.setTextSize(2);
    tft.setCursor(20, 160);
    tft.print(message);
}
