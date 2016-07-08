#include "Display.hpp"

Display::Display()
    : _colorBlank( { .red = 0, .green = 0, .blue = 0} ),
      _color1( { .red = 255, .green = 255, .blue = 0} ),
      _color2( { .red = 255, .green = 63, .blue = 0} ),
      _ledColors{ _color1, },
      _ledColorsNew{ _color1, },
      _ledBrightness( 16 ),
      _ledStrip() {
    clearPixels();
}

int Display::convertCoordsToArrayPos(int x, int y) {
    int xOffset = (y % 2 == 1 ? DISPLAY_WIDTH - 1 - x : x);
    return y * DISPLAY_WIDTH + xOffset;
}

void Display::enablePixel(int x, int y) {
    _ledColorsNew[convertCoordsToArrayPos(x, y)] = _color1;
}

void Display::clearPixels() {
    clearPixels(_colorBlank);
}

void Display::clearPixels(rgb_color col) {
    for (int i = 0; i < LED_COUNT; i++) {
        _ledColorsNew[i] = col;
    }
}

void Display::writePixels() {
    rgb_color fadeLedColors[LED_COUNT] = { { .red = 0, .green = 0, .blue = 0 }, };

    for (int i = 0; i < 255; i++) {
        // Fade amount
        float t = float(i) / 255.0;
        for (int l = 0; l < LED_COUNT; l++) {
            fadeLedColors[l].red = int(((1.0-t) * _ledColors[l].red + t * _ledColorsNew[l].red) + 0.5);
            fadeLedColors[l].green = int(((1.0-t) * _ledColors[l].green + t * _ledColorsNew[l].green) + 0.5);
            fadeLedColors[l].blue = int(((1.0-t) * _ledColors[l].blue + t * _ledColorsNew[l].blue) + 0.5);
        }
        _ledStrip.write(fadeLedColors, LED_COUNT, _ledBrightness);
        delay(3);
    }
    memcpy(_ledColors, _ledColorsNew, sizeof(_ledColorsNew));
}

int Display::getBrightness() {
    return _ledBrightness;
}

void Display::setBrightness(int br) {
    if (br <= 31 && br >= 0) {
        _ledBrightness = br;
    }
}

rgb_color Display::getColor1() {
    return _color1;
}

void Display::setColor1(rgb_color col1) {
    _color1 = col1;
}

rgb_color Display::getColor2() {
    return _color2;
}

void Display::setColor2(rgb_color col2) {
    _color2 = col2;
}

void Display::setPixelColor(int x, int y, rgb_color col) {
    _ledColorsNew[convertCoordsToArrayPos(x, y)] = col;
}

bool Display::getEnabled(int x, int y) {
    if (_ledColorsNew[convertCoordsToArrayPos(x, y)].red != _colorBlank.red
        || _ledColorsNew[convertCoordsToArrayPos(x, y)].green != _colorBlank.green
        || _ledColorsNew[convertCoordsToArrayPos(x, y)].blue != _colorBlank.blue) {
            return true;
    } else {
        return false;
    }
}

bool Display::getEnabled(int i) {
    if (_ledColorsNew[i].red != _colorBlank.red
        || _ledColorsNew[i].green != _colorBlank.green
        || _ledColorsNew[i].blue != _colorBlank.blue) {
            return true;
    } else {
        return false;
    }
}

int Display::getNumEnabledPixels() {
    int c = 0;
    for (int i = 0; i < LED_COUNT; i++) {
        if (getEnabled(i)) {
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
                //Serial.printf("Setting Pixel %d : %d to color 2.", x, y);
                setPixelColor(x, DISPLAY_HEIGHT - y - 1, _color2);
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
