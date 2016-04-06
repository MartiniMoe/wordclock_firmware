#include "Wordclock.hpp"

Wordclock wordclock;

void setup(void){
    Serial.begin(9600);
    Serial.println("This is Wordclock speaking.");

    String ip = wordclock.connectWiFi("HaSi-Kein-Internet-Legacy", "bugsbunny");
    wordclock.setupWebserver();
    wordclock.setupNtp();
    Serial.println(ip);
}

void loop(void){
    wordclock.loop();
    delay(1000);
}
