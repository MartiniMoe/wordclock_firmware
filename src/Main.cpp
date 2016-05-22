#include "Wordclock.hpp"

Wordclock wordclock;

void setup(void){
    Serial.begin(9600);
    Serial.println("This is Wordclock speaking.");

    String ip = wordclock.connectWiFi("HaSi-Kein-Internet-Legacy", "bugsbunny");
    wordclock.setupWebserver();
    wordclock.setupNtp();
    Serial.println(ip);

    //ArduinoOTA.setHostname("wordclock");
    //ArduinoOTA.setPassword("bugsbunny");
    //ArduinoOTA.begin();
}

void loop(void){
    //ArduinoOTA.handle();
    wordclock.loop();
    delay(1000);
}
