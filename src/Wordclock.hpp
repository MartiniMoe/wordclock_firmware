#ifndef WORDCLOCK_HPP
#define WORDCLOCK_HPP

#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include <DNSServer.h>
#include <TimeLib.h>
#include <NtpClientLib.h>
#include "Display.hpp"
#include <ArduinoOTA.h>
#include <EEPROM.h>

#define ESSID_LENGTH    32
#define PASSWORD_LENGTH 96

#define SWITCH_TO_AP_COUNTER_LIMIT 10

#define CONFIG_PAGE_CSS "html{font-family:sans-serif;-ms-text-size-adjust:100%;-webkit-text-size-adjust:100%}body{margin:0}h1{font-size:2em;margin:.67em 0}button,input{color:inherit;font:inherit;margin:0}.pure-button,input{line-height:normal}button{overflow:visible;text-transform:none}button,html input[type=button],input[type=submit]{-webkit-appearance:button;cursor:pointer}button::-moz-focus-inner,input::-moz-focus-inner{border:0;padding:0}input[type=checkbox]{box-sizing:border-box;padding:0}input[type=number]::-webkit-inner-spin-button,input[type=number]::-webkit-outer-spin-button{height:auto}fieldset{border:1px solid silver;margin:0 2px;padding:.35em .625em .75em}.pure-g{letter-spacing:-.31em;text-rendering:optimizespeed;font-family:FreeSans,Arimo,'Droid Sans',Helvetica,Arial,sans-serif;-webkit-flex-flow:row wrap;display:-ms-flexbox;-ms-flex-flow:row wrap;-ms-align-content:flex-start;-webkit-align-content:flex-start;align-content:flex-start}.pure-u-1-1,.pure-u-1-2,.pure-u-23-24{display:inline-block;zoom:1;letter-spacing:normal;word-spacing:normal;vertical-align:top;text-rendering:auto}.pure-u-23-24{width:95.8333%}.pure-u-1-2{width:50%}.pure-u-1-1{width:100%}.pure-button{display:inline-block;zoom:1;white-space:nowrap;vertical-align:middle;text-align:center;cursor:pointer;-webkit-user-drag:none;-webkit-user-select:none;-moz-user-select:none;-ms-user-select:none;user-select:none;-webkit-box-sizing:border-box;-moz-box-sizing:border-box;box-sizing:border-box;font-family:inherit;font-size:100%;padding:.5em 1em;color:#444;color:rgba(0,0,0,.8);border:1px solid #999;border:0 transparent;background-color:#E6E6E6;text-decoration:none;border-radius:2px}.pure-button-primary,a.pure-button-primary{background-color:#0078e7;color:#fff}.pure-button:focus,.pure-button:hover{filter:progid: DXImageTransform.Microsoft.gradient(startColorstr='#00000000', endColorstr='#1a000000', GradientType=0);background-image:-webkit-gradient(linear,0 0,0 100%,from(transparent),color-stop(40%,rgba(0,0,0,.05)),to(rgba(0,0,0,.1)));background-image:-webkit-linear-gradient(transparent,rgba(0,0,0,.05) 40%,rgba(0,0,0,.1));background-image:-moz-linear-gradient(top,rgba(0,0,0,.05) 0,rgba(0,0,0,.1));background-image:-o-linear-gradient(transparent,rgba(0,0,0,.05) 40%,rgba(0,0,0,.1));background-image:linear-gradient(transparent,rgba(0,0,0,.05) 40%,rgba(0,0,0,.1))}.pure-button:focus{outline:0}.pure-button:active{box-shadow:0 0 0 1px rgba(0,0,0,.15) inset,0 0 6px rgba(0,0,0,.2) inset;border-color:#000\\9}.pure-button::-moz-focus-inner{padding:0;border:0}.pure-form input[type=color],.pure-form input[type=number],.pure-form input[type=password],.pure-form input[type=text]{padding:.5em .6em;display:inline-block;border:1px solid #ccc;box-shadow:inset 0 1px 3px #ddd;border-radius:4px;vertical-align:middle;-webkit-box-sizing:border-box;-moz-box-sizing:border-box;box-sizing:border-box}.pure-form input:not([type]){padding:.5em .6em;display:inline-block;border:1px solid #ccc;box-shadow:inset 0 1px 3px #ddd;border-radius:4px;-webkit-box-sizing:border-box;-moz-box-sizing:border-box;box-sizing:border-box}.pure-form input[type=color]{padding:.2em .5em}.pure-form input:not([type]):focus,.pure-form input[type=color]:focus,.pure-form input[type=number]:focus,.pure-form input[type=password]:focus,.pure-form input[type=text]:focus{outline:0;border-color:#129FEA}.pure-form input[type=checkbox]:focus{outline:#129FEA auto 1px}.pure-form .pure-checkbox{margin:.5em 0;display:block}.pure-form fieldset{margin:0;padding:.35em 0 .75em;border:0}.pure-form-stacked input[type=color],.pure-form-stacked input[type=file],.pure-form-stacked input[type=number],.pure-form-stacked input[type=password],.pure-form-stacked input[type=text],.pure-form-stacked label{display:block;margin:.25em 0}"
#define DNS_PORT 53

class Wordclock {
private:
    ESP8266WebServer _server;
    ntpClient *_ntp;
    Display _display;
    DNSServer _dnsServer;
    bool _showEsIst;
    bool _enableAutoBrighness;

    String _hostname;
    String _ssid;
    String _password;
    String _apSsid;
    String _apPassword;

    void readWirelessConfig();
    void writeWirelessConfig(const char* ssid, const char* password);

    void handleRootGet();
    void handleRootPost();

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
