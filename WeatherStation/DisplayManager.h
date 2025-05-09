#ifndef DISPLAY_MANAGER_H
#define DISPLAY_MANAGER_H

#include <Adafruit_GFX.h>
#include <Adafruit_ILI9341.h>
#include <avr/pgmspace.h>  // Include PROGMEM support

// Display color definitions
#define BACKGROUND   0x2104
#define TEXT_COLOR   0xEF7D
#define TEMP_COLOR   0x639D
#define PRESS_COLOR  0xF20B
#define GRID_COLOR   0x3186
#define ALT_COLOR    0xE581

// Graph definitions
#define GRAPH_X      20
#define GRAPH_Y      200
#define GRAPH_W      200
#define GRAPH_H      100
#define GRAPH_PAD    5

// Pin definitions for display
#define DISP_CS   8
#define DISP_RST  9
#define DISP_DC   10

class DisplayManager {
public:
    DisplayManager();
    void begin();
    void drawStaticElements();
    void updateDisplay(float currentTemp, float currentPress, float altitude, 
                       float* tempHistory, float* pressHistory, int dataIndex, int maxDataPoints);
    void drawError(const char* message);
    void drawError(const __FlashStringHelper* message); // Add overload for F() strings

private:
    Adafruit_ILI9341 tft;
    void updateAltitudeText(float alt);
    void drawGraphs(float* tempHistory, float* pressHistory, int dataIndex, int maxDataPoints);
};

// Store strings in program memory
extern const char TITLE[] PROGMEM;
extern const char AUTHOR1[] PROGMEM;
extern const char AUTHOR2[] PROGMEM;
extern const char TEMP_LABEL[] PROGMEM;
extern const char PRESS_LABEL[] PROGMEM;
extern const char ALT_LABEL[] PROGMEM;
extern const char TEMP_UNIT[] PROGMEM;
extern const char PRESS_UNIT[] PROGMEM;
extern const char ALT_UNIT[] PROGMEM;
extern const char SEA_LEVEL_TEXT[] PROGMEM;
extern const char LOW_ALT_TEXT[] PROGMEM;
extern const char HIGH_ALT_TEXT[] PROGMEM;

#endif // DISPLAY_MANAGER_H
