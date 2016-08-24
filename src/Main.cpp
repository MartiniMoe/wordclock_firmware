#include "Wordclock.hpp"

extern "C" {
#include "user_interface.h"
}

Wordclock wordclock;

String ip;

void setup(void) {
    system_update_cpu_freq(160);

    Serial.begin(9600);
    delay(4000);

    ip = WiFi.localIP().toString();

    wordclock.setupWebserver();
    wordclock.setupNtp();

    wordclock.begin();
}

void loop(void){
    wordclock.loop();
}
