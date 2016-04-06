#ifndef WORDCLOCK_HPP
#define WORDCLOCK_HPP

#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include <TimeLib.h>
#include <NtpClientLib.h>
#include "Display.hpp"

class Wordclock {
private:
    ESP8266WebServer _server;
    ntpClient *_ntp;
    Display _display;

    void handleRoot();
    void handleColors();
    void handleNotFound();
    void handleWebserver();

    void handleTime(int hour, int minute);
    void handleHour(int hour, int minute);
    void handleMinute(int minute);
    void handleProgress(int minute, int second);
public:
    Wordclock();

    String connectWiFi(String ssid, String password);
    void loop();
    void setupNtp();
    void setupWebserver();
};

#endif
