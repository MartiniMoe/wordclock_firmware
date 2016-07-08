#include "Wordclock.hpp"

Wordclock wordclock;

String ip;

void setup(void){
    Serial.begin(9600);
    delay(2000);
    
    Serial.println("This is Wordclock speaking.");

    ip = WiFi.localIP().toString();

    wordclock.setupWebserver();
    wordclock.setupAccessPoint("Wordclock Dana", "wordclockdana");
    wordclock.setupNtp();

    wordclock.begin();
}

void loop(void){
    wordclock.loop();
    delay(10);
}
