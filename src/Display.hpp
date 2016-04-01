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
    rgb_color _ledColors[LED_COUNT];
    bool _ledColorsMask[LED_COUNT];
    int _ledBrightness;

    APA102<PIN_DATA, PIN_CLOCK> _ledStrip;

    void enablePixel(int x, int y);

public:
    Display();

    void writePixels();

    void displayEs();
    void displayIst();
    void displayPrefixFuenf();
    void displayPrefixZehn();
    void displayZwanzig();
    void displayDreiviertel();
    void displayNach();
    void displayVor();
    void displayHalb();
    void displayZwoelf();
    void displayZwei();
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
