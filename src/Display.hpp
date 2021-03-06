#ifndef DISPLAY_HPP
#define DISPLAY_HPP

#include <APA102.h>

#define PIN_CLOCK 13
#define PIN_DATA  12

#define DISPLAY_WIDTH  11
#define DISPLAY_HEIGHT 10
#define LED_COUNT      DISPLAY_WIDTH * DISPLAY_HEIGHT

class Display {
private:
    rgb_color _colorBlank;
    rgb_color _color1;
    rgb_color _color2;
    rgb_color _ledColors[LED_COUNT];
    rgb_color _ledColorsNew[LED_COUNT];
    int _ledBrightness;

    APA102<PIN_DATA, PIN_CLOCK> _ledStrip;

    int convertCoordsToArrayPos(int x, int y);

public:
    Display();

    void enablePixel(int x, int y);
    void clearPixels();
    void clearPixels(rgb_color col);
    void writePixels();
    void directlyFlush();

    int getBrightness();
    void setBrightness(int br);

    rgb_color getColor1();
    void setColor1(rgb_color col1);
    rgb_color getColor2();
    void setColor2(rgb_color col2);

    void setPixelColor(int x, int y, rgb_color col);
    bool getEnabled(int x, int y);
    bool getEnabled(int i);
    int getNumEnabledPixels();
    void highlightProgress(int nominator, int denominator);

    void displayEs();
    void displayIst();
    void displayPrefixFuenf();
    void displayPrefixZehn();
    void displayZwanzig();
    void displayDreiviertel();
    void displayViertel();
    void displayNach();
    void displayVor();
    void displayHalb();
    void displayZwoelf();
    void displayZwei();
    void displayEin();
    void displayEins();
    void displaySieben();
    void displayDrei();
    void displayFuenf();
    void displayElf();
    void displayNeun();
    void displayVier();
    void displayAcht();
    void displayZehn();
    void displaySechs();
    void displayUhr();
};

#endif
