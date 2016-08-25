#include "Wordclock.hpp"
#include "fablab.h"
#include "hasi.h"

Wordclock::Wordclock()
    : _server(80),
      _display(),
      _showEsIst(true),
      _hostname("wordclock"),
      _ssid(""),
      _password(""),
      _apSsid(""),
      _apPassword("") {
}

void Wordclock::begin() {
    _display.clearPixels();

    for (int y = 0; y < 10; y++) {
        for (int x = 0; x < 11; x++) {

            rgb_color col = { .red = bootLogo_FABLAB[y][x][0],
                              .green = bootLogo_FABLAB[y][x][1],
                              .blue = bootLogo_FABLAB[y][x][2] };

            _display.setPixelColor(x, y, col);
        }
    }

    _display.directlyFlush();
    delay(7000);

    for (int i = 0; i < 21 * 10 + 4; i++) {
        for (int y = 0; y < 10; y++) {
            for (int x = 0; x < 11; x++) {
                int ry = (y + i) % 10;

                rgb_color col = { .red = bootLogo_HASI[ry][x][0],
                                  .green = bootLogo_HASI[ry][x][1],
                                  .blue = bootLogo_HASI[ry][x][2] };

                _display.setPixelColor(x, y, col);
            }
        }

        _display.directlyFlush();

        delayMicroseconds(100 * i / 4);
    }

    delay(5000);

    EEPROM.begin(512);
    //writeWirelessConfig("HaSi-Kein-Internet-Legacy", "bugsbunny");
    readWirelessConfig();

    Serial.println("Connecting to:");
    Serial.println("SSID: " + _ssid);
    Serial.println("Pass: " + _password);

    connectWiFi(_ssid, _password, true);
}

void Wordclock::loop() {
    connectWiFi(_ssid, _password, false);

    _server.handleClient();
    //_display.clearPixels( rgb_color { .red = 0, .green = 255, .blue = 0 });
    _display.clearPixels();
    time_t now_time = now();
    int now_hour = hour(now_time);
    int now_minute = minute(now_time);
    int now_second = second(now_time);
    if ((timeStatus() != timeNotSet) || (now != 0)) {
        handleTime(now_hour, now_minute);
    }
    handleProgress(now_minute, now_second);
    _display.writePixels();
}

void Wordclock::connectWiFi(String ssid, String password, bool forceConnect) {
    static bool connecting = false;
    static int counter = 0;
    static bool apMode = false;

    if (forceConnect) {
        counter = 0;
        apMode = false;
    }

    if (apMode) {
        return;
    }

    if (counter > SWITCH_TO_AP_COUNTER_LIMIT && !apMode) {
        Serial.println("Switching to AP mode, could not connect to the WiFi!");

        WiFi.mode(WIFI_AP);
        setupAccessPoint("Wordclock", "fablab1337");

        apMode = true;
    }

    if ((!connecting && WiFi.status() != WL_CONNECTED) || forceConnect) {
        connecting = true;

        _ssid = ssid.c_str();
        _password = password.c_str();

        WiFi.mode(WIFI_STA);
        WiFi.hostname(_hostname);
        WiFi.begin(_ssid.c_str(), _password.c_str());
    } else if (WiFi.status() == WL_CONNECTED) {
        connecting = false;
        counter = 0;
    } else if (connecting && counter <= SWITCH_TO_AP_COUNTER_LIMIT) {
        counter++;
    }
}

void Wordclock::setupAccessPoint(String apSsid, String apPassword) {
    _apSsid = apSsid;
    _apPassword = apPassword;
    IPAddress apIp, apGw, apSn;
    apIp.fromString("192.168.0.1");
    apGw.fromString("192.168.0.1");
    apSn.fromString("255.255.255.0");
    WiFi.softAPConfig(apIp, apGw, apSn);
    WiFi.softAP(_apSsid.c_str(), _apPassword.c_str());
}

void Wordclock::setupNtp() {
    _ntp = ntpClient::getInstance("de.pool.ntp.org", 1);
    _ntp->setInterval(15, 1800);
    _ntp->begin();
}

void Wordclock::readWirelessConfig() {
    char tmpSsid[ESSID_LENGTH];
    char tmpPassword[PASSWORD_LENGTH];

    for (int i = 0; i < ESSID_LENGTH; i++) {
        tmpSsid[i] = char(EEPROM.read(i));
    }

    for (int i = 0; i < PASSWORD_LENGTH; i++) {
        tmpPassword[i] = char(EEPROM.read(ESSID_LENGTH + i));
    }

    _ssid = String(tmpSsid);
    _password = String(tmpPassword);
}

void Wordclock::writeWirelessConfig(const char* ssid, const char* password) {
    for (int i = 0; i < ESSID_LENGTH - 1; i++) {
        EEPROM.write(i, ssid[i]);

        if (ssid[i] == 0) {
            break;
        }
    }

    EEPROM.write(ESSID_LENGTH - 1, 0);

    for (int i = 0; i < PASSWORD_LENGTH - 1; i++) {
        EEPROM.write(ESSID_LENGTH + i, password[i]);

        if (password[i] == 0) {
            break;
        }
    }

    EEPROM.write(ESSID_LENGTH + PASSWORD_LENGTH - 1, 0);

    EEPROM.commit();
}

void Wordclock::handleTime(int hour, int minute) {
    if (_showEsIst) {
        _display.displayEs();
        _display.displayIst();
    }
    if (minute < 25) {
        handleHour(hour, minute);
    } else {
        handleHour(hour + 1, minute);
    }
    handleMinute(minute);
}

void Wordclock::handleHour(int hour, int minute) {
    hour = hour % 12;
    switch (hour) {
        case 0:
            _display.displayZwoelf();
            break;
        case 1:
            if (minute / 5 == 0) {
                _display.displayEin();
            } else {
                _display.displayEins();
            }
            break;
        case 2:
            _display.displayZwei();
            break;
        case 3:
            _display.displayDrei();
            break;
        case 4:
            _display.displayVier();
            break;
        case 5:
            _display.displayFuenf();
            break;
        case 6:
            _display.displaySechs();
            break;
        case 7:
            _display.displaySieben();
            break;
        case 8:
            _display.displayAcht();
            break;
        case 9:
            _display.displayNeun();
            break;
        case 10:
            _display.displayZehn();
            break;
        case 11:
            _display.displayElf();
            break;
        default:
            break;
    }
}

void Wordclock::handleMinute(int minute) {
    minute = minute / 5;
    switch (minute) {
        case 0:
            _display.displayUhr();
            break;
        case 1:
            _display.displayPrefixFuenf();
            _display.displayNach();
            break;
        case 2:
            _display.displayPrefixZehn();
            _display.displayNach();
            break;
        case 3:
            _display.displayViertel();
            _display.displayNach();
            break;
        case 4:
            _display.displayZwanzig();
            _display.displayNach();
            break;
        case 5:
            _display.displayPrefixFuenf();
            _display.displayVor();
            _display.displayHalb();
            break;
        case 6:
            _display.displayHalb();
            break;
        case 7:
            _display.displayPrefixFuenf();
            _display.displayNach();
            _display.displayHalb();
            break;
        case 8:
            _display.displayZwanzig();
            _display.displayVor();
            break;
        case 9:
            _display.displayViertel();
            _display.displayVor();
            break;
        case 10:
            _display.displayPrefixZehn();
            _display.displayVor();
            break;
        case 11:
            _display.displayPrefixFuenf();
            _display.displayVor();
            break;
    }
}

void Wordclock::handleProgress(int minute, int second) {
    int nominator = (minute % 5) * 60 + second;
    _display.highlightProgress(nominator, 300);
}

rgb_color Wordclock::parseRGB(String hexstring) {
    long number = (long) strtol(&hexstring[1], NULL, 16);
    int r = number >> 16;
    int g = number >> 8 & 0xFF;
    int b = number & 0xFF;

    return rgb_color{.red = (uint8) r, .green = (uint8) g, .blue = (uint8) b};
}

String Wordclock::generateRGB(rgb_color color) {
    char hex[7] = {0};
    sprintf(hex, "%02X%02X%02X", color.red, color.green, color.blue);
    return String("#" + String(hex));
}

void Wordclock::setupWebserver() {
    _server.on("/pure-min-reduced.css", HTTP_GET, [this]() {
        _server.send(200, "text/css", F(CONFIG_PAGE_CSS));

        Serial.println("Delivered /pure-min-reduced.css");
    });

    _server.on("/", HTTP_GET, [this]() {
        String stateEsIstOn = "";

        if (_showEsIst) {
            stateEsIstOn = "checked";
        } else {
            stateEsIstOn = "";
        }

        String stateBrightness = String(_display.getBrightness());

        rgb_color stateCol1 = rgb_color(_display.getColor1());
        rgb_color stateCol2 = rgb_color(_display.getColor2());

        String stateCol1_str = generateRGB(stateCol1);
        String stateCol2_str = generateRGB(stateCol2);

        _server.send(200, "text/html",
            "<html>"
            "<head>"
            "<meta charset='UTF-8'>"
            "<meta name='viewport' content='width=device-width, initial-scale=1'>"
            "<title>Wordclock Configuration</title>"
            "<link rel='stylesheet' type='text/css' href='/pure-min-reduced.css'>"
            "<style>"
            "h1 { text-align: center; }"
            ".wordclock-content { background: #F0F0F0; border: #F0F0F0 4px solud; border-radius: 8px; width: 40%; min-width: 200pt; margin: 0 auto; padding: 20px; }"
            "</style>"
            "</head>"
            "<body>"
            "<h1>Wordclock</h1>"
            "<div class='wordclock-content'>"
            "<form action='/' method='POST' class='pure-form'>"
            "<label>Wordclock Einstellungen:</label>"
            "<fieldset>"
            "<div class='pure-g' style='margin-bottom: 10pt;'>"
            "<div class='pure-control-group pure-u-1-2'>"
            "<label>Farbe 1:</label><br>"
            "<input type='color' name='col1' class='pure-input-1 pure-u-23-24' style='height: 32px;' value='" + stateCol1_str + "'>"
            "</div>"
            "<div class='pure-control-group pure-u-1-2'>"
            "<label>Farbe 2:</label><br>"
            "<input type='color' name='col2' class='pure-input-1 pure-u-23-24' style='height: 32px;' value='" + stateCol2_str + "'>"
            "</div>"
            "</div>"
            "<div class='pure-control-group' style='margin-bottom: 10pt;'>"
            "<label>Helligkeit:</label><br>"
            "<input type='number' class='pure-input-1 pure-u-1-1' min='0' max='31' name='brightness' value='" + stateBrightness + "'>"
            "</div>"
            "<label class='pure-checkbox'>"
            "<input name='showEsIst' type='checkbox' " + stateEsIstOn + "> „ES IST” anzeigen"
            "</label>"
            "</fieldset>"
            "<label>Wireless Einstellungen:</label>"
            "<fieldset class='pure-group'>"
            "<input type='text' name='wifiSsid' class='pure-input-1 pure-u-1-1' placeholder='WLAN-Name' value='" + _ssid + "'>"
            "<input type='password' name='wifiPassword' class='pure-input-1 pure-u-1-1' placeholder='Passwort' value='" + _password + "'>"
            "</fieldset>"
            "<button type='submit' class='pure-button pure-input-1 pure-u-1-1 pure-button-primary'>Speichern</button>"
            "</form>"
            "</div>"
            "</body>"
            "</html>"
        );

        Serial.println("Delivered /");
    });

    _server.on("/", HTTP_POST, [this]() {
        bool wifiChanged = false;

        _showEsIst = false;

        for (int i = 0; i < _server.args(); i++) {
            if (_server.argName(i) == "showEsIst") {
                if (_server.arg(i) == "on") {
                    _showEsIst = true;
                }
            } else if (_server.argName(i) == "brightness") {
                _display.setBrightness(_server.arg(i).toInt());
            } else if (_server.argName(i) == "col1") {
                _display.setColor1(parseRGB(_server.arg(i)));
            } else if (_server.argName(i) == "col2") {
                _display.setColor2(parseRGB(_server.arg(i)));
            } else if (_server.argName(i) == "wifiSsid") {
                if (!_ssid.equals(_server.arg(i))) {
                    wifiChanged = true;
                }

                _ssid = _server.arg(i);
            } else if (_server.argName(i) == "wifiPassword") {
                if (!_password.equals(_server.arg(i))) {
                    wifiChanged = true;
                }

                _password = _server.arg(i);
            }
        }

        _server.send(200, "text/html", "<html><head><meta charset=\"UTF-8\"><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\"><title>Wordclock Configuration</title></head><body>Einstellungen wurden übernommen!</body></html>");

        if (wifiChanged) {
            writeWirelessConfig(_ssid.c_str(), _password.c_str());
            connectWiFi(_ssid, _password, true);

            Serial.println("Wrote wireless config:");
            Serial.println("New SSID: " + _ssid);
            Serial.println("New Pass: " + _password);
        }
    });
    //_server.on("/color", handleColors);
    //_server.onNotFound(handleNotFound);
    _server.begin();
}
