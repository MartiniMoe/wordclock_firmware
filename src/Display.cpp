#include "Display.hpp"

Display::Display()
    : _ledColors{ { .red = 0, .green = 0, .blue = 0 }, },
      _ledColorsMask{ false, },
      _ledBrightness( 31 ),
      _ledStrip() {
}

void Display::enablePixel(int x, int y) {
    int xOffset = (y % 2 == 1 ? DISPLAY_WIDTH - 1 - x : x);
    _ledColorsMask[y * DISPLAY_WIDTH + xOffset] = true;
}

void Display::writePixels() {
    rgb_color newLedColors[LED_COUNT] = { { .red = 0, .green = 0, .blue = 0 }, };

    for ( int i = 0; i < LED_COUNT; i++ ) {
        if ( _ledColorsMask[i]) {
            newLedColors[i] = _ledColors[i];
        }
    }

    _ledStrip.write(newLedColors, LED_COUNT, _ledBrightness);
}
