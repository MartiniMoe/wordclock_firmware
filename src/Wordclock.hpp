#ifndef WORDCLOCK_HPP
#define WORDCLOCK_HPP

#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include <TimeLib.h>
#include <NtpClientLib.h>
#include "Display.hpp"
#include <ArduinoOTA.h>
#include <EEPROM.h>

#define ESSID_LENGTH    32
#define PASSWORD_LENGTH 96

class Wordclock {
private:
    ESP8266WebServer _server;
    ntpClient *_ntp;
    Display _display;
    bool _showEsIst;

    String _hostname;
    String _ssid;
    String _password;
    String _apSsid;
    String _apPassword;

    void readWirelessConfig();
    void writeWirelessConfig(const char* ssid, const char* password);

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

    void begin();

    void connectWiFi(String ssid, String password, bool forceConnect);
    void setupAccessPoint(String apSsid, String apPassword);
    void loop();
    void setupNtp();
    void setupWebserver();
    rgb_color parseRGB(String hexstring);
    String generateRGB(rgb_color color);
};

#endif
