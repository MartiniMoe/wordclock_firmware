#include "Display.hpp"

Display::Display()
    : _ledColors{ { .red = 0, .green = 255, .blue = 0 }, },
      _ledColorsMask{ false, },
      _ledBrightness( 16 ),
      _ledStrip() {
    //pinMode(PIN_CLOCK, OUTPUT);
    //pinMode(PIN_DATA, OUTPUT);
    for (int i = 0; i < LED_COUNT; i++) {
        _ledColors[i] = { .red = 0, .green = 255, .blue = 0 };
        _ledColorsMask[i] = false;
    }
}

int Display::convertCoordsToArrayPos(int x, int y) {
    int xOffset = (y % 2 == 1 ? DISPLAY_WIDTH - 1 - x : x);
    return y * DISPLAY_WIDTH + xOffset;
}

void Display::enablePixel(int x, int y) {
    _ledColorsMask[convertCoordsToArrayPos(x, y)] = true;
}

void Display::clearPixels() {
    clearPixels(rgb_color { .red = 0, .green = 0, .blue = 0 });
}

void Display::clearPixels(rgb_color col) {
    for (int i = 0; i < LED_COUNT; i++) {
        _ledColorsMask[i] = false;
        _ledColors[i] = col;
    }
}

void Display::writePixels() {
    rgb_color newLedColors[LED_COUNT] = { { .red = 0, .green = 0, .blue = 0 }, };

    for (int i = 0; i < LED_COUNT; i++) {
        if (_ledColorsMask[i]) {
            newLedColors[i] = _ledColors[i];
        }
    }

    _ledStrip.write(newLedColors, LED_COUNT, _ledBrightness);
}

void Display::setPixelColor(int x, int y, rgb_color col) {
    _ledColors[convertCoordsToArrayPos(x, y)] = col;
}

bool Display::getEnabled(int x, int y) {
    return _ledColorsMask[convertCoordsToArrayPos(x, y)];
}

int Display::getNumEnabledPixels() {
    int c = 0;
    for (int i = 0; i < LED_COUNT; i++) {
        if (_ledColorsMask[i]) {
            c++;
        }
    }
    return c;
}

void Display::highlightProgress(int nominator, int denominator) {
    int progress = (nominator * getNumEnabledPixels()) / denominator;
    int changedPixels = 0;
    for (int y = 0; y < DISPLAY_HEIGHT; y++) {
        for (int x = 0; x < DISPLAY_WIDTH; x++) {
            if (changedPixels >= progress) {
                return;
            }
            if (getEnabled(x, DISPLAY_HEIGHT - y - 1)) {
                changedPixels++;
                setPixelColor(x, DISPLAY_HEIGHT - y - 1, rgb_color{ .red = 0, .green = 0, .blue = 255 });
            }
        }
    }
}

void Display::displayEs() {
    enablePixel(0, 9);
    enablePixel(1, 9);
}

void Display::displayIst() {
    enablePixel(3, 9);
    enablePixel(4, 9);
    enablePixel(5, 9);
}

void Display::displayPrefixFuenf() {
    enablePixel(7, 9);
    enablePixel(8, 9);
    enablePixel(9, 9);
    enablePixel(10, 9);
}

void Display::displayPrefixZehn() {
    enablePixel(0, 8);
    enablePixel(1, 8);
    enablePixel(2, 8);
    enablePixel(3, 8);
}

void Display::displayZwanzig() {
    enablePixel(4, 8);
    enablePixel(5, 8);
    enablePixel(6, 8);
    enablePixel(7, 8);
    enablePixel(8, 8);
    enablePixel(9, 8);
    enablePixel(10, 8);
}

void Display::displayDreiviertel() {
    enablePixel(0, 7);
    enablePixel(1, 7);
    enablePixel(2, 7);
    enablePixel(3, 7);
    enablePixel(4, 7);
    enablePixel(5, 7);
    enablePixel(6, 7);
    enablePixel(7, 7);
    enablePixel(8, 7);
    enablePixel(9, 7);
    enablePixel(10, 7);
}

void Display::displayViertel() {
    enablePixel(4, 7);
    enablePixel(5, 7);
    enablePixel(6, 7);
    enablePixel(7, 7);
    enablePixel(8, 7);
    enablePixel(9, 7);
    enablePixel(10, 7);
}

void Display::displayNach() {
    enablePixel(2, 6);
    enablePixel(3, 6);
    enablePixel(4, 6);
    enablePixel(5, 6);
}

void Display::displayVor() {
    enablePixel(6, 6);
    enablePixel(7, 6);
    enablePixel(8, 6);
}

void Display::displayHalb() {
    enablePixel(0, 5);
    enablePixel(1, 5);
    enablePixel(2, 5);
    enablePixel(3, 5);
}

void Display::displayZwoelf() {
    enablePixel(5, 5);
    enablePixel(6, 5);
    enablePixel(7, 5);
    enablePixel(8, 5);
    enablePixel(9, 5);
}

void Display::displayZwei() {
    enablePixel(0, 4);
    enablePixel(1, 4);
    enablePixel(2, 4);
    enablePixel(3, 4);
}

void Display::displayEin() {
    enablePixel(2, 4);
    enablePixel(3, 4);
    enablePixel(4, 4);
}

void Display::displayEins() {
    enablePixel(2, 4);
    enablePixel(3, 4);
    enablePixel(4, 4);
    enablePixel(5, 4);
}

void Display::displaySieben() {
    enablePixel(5, 4);
    enablePixel(6, 4);
    enablePixel(7, 4);
    enablePixel(8, 4);
    enablePixel(9, 4);
    enablePixel(10, 4);
}

void Display::displayDrei() {
    enablePixel(1, 3);
    enablePixel(2, 3);
    enablePixel(3, 3);
    enablePixel(4, 3);
}

void Display::displayFuenf() {
    enablePixel(7, 3);
    enablePixel(8, 3);
    enablePixel(9, 3);
    enablePixel(10, 3);
}

void Display::displayElf() {
    enablePixel(0, 2);
    enablePixel(1, 2);
    enablePixel(2, 2);
}

void Display::displayNeun() {
    enablePixel(3, 2);
    enablePixel(4, 2);
    enablePixel(5, 2);
    enablePixel(6, 2);
}

void Display::displayVier() {
    enablePixel(7, 2);
    enablePixel(8, 2);
    enablePixel(9, 2);
    enablePixel(10, 2);
}

void Display::displayAcht() {
    enablePixel(1, 1);
    enablePixel(2, 1);
    enablePixel(3, 1);
    enablePixel(4, 1);
}

void Display::displayZehn() {
    enablePixel(5, 1);
    enablePixel(6, 1);
    enablePixel(7, 1);
    enablePixel(8, 1);
}

void Display::displaySechs() {
    enablePixel(1, 0);
    enablePixel(2, 0);
    enablePixel(3, 0);
    enablePixel(4, 0);
    enablePixel(5, 0);
}

void Display::displayUhr() {
    enablePixel(8, 0);
    enablePixel(9, 0);
    enablePixel(10, 0);
}
