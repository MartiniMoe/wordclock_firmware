#include "Wordclock.hpp"

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
    EEPROM.begin(512);
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

    if ((!connecting && WiFi.status() != WL_CONNECTED) || forceConnect) {
        connecting = true;

        _ssid = ssid.c_str();
        _password = password.c_str();
        WiFi.hostname(_hostname);
        WiFi.begin(_ssid.c_str(), _password.c_str());
    } else if (WiFi.status() == WL_CONNECTED) {
        connecting = false;
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
    _server.on("/", HTTP_GET, [this]() {
        String stateEsIstOn = "";
        String stateEsIstOff = "";
        if (_showEsIst) {
            stateEsIstOn = "checked";
            stateEsIstOff = "";
        } else {
            stateEsIstOff = "checked";
            stateEsIstOn = "";
        }
        String stateBrightness = String(_display.getBrightness());
        rgb_color stateCol1 = rgb_color(_display.getColor1());
        rgb_color stateCol2 = rgb_color(_display.getColor2());

        String stateCol1_str = generateRGB(stateCol1);
        String stateCol2_str = generateRGB(stateCol2);

        rgb_color col1;
        rgb_color col2;
        _server.send(200, "text/html", "<html><head><meta charset=\"UTF-8\"><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\"><title>Wordclock Configuration</title></head>"
            "<body><h1>Wordclock Configuration</h1>"
            "<form action='/' method='POST'><table>"
            "<tr><td>Zeige <i>ES IST</i></td><td><input type='radio' name='showEsIst' value='on' " + stateEsIstOn + ">Ein<br>"
                "<input type='radio' name='showEsIst' value='off' " + stateEsIstOff + ">Aus</td></tr>"
            "<tr><td>Helligkeit</td><td><input type='number' min='0' max='31' name='brightness' value='" + stateBrightness + "'></td></tr>"
            "<tr><td>Farbe 1</td><td><input type='color' name='col1' value='" + stateCol1_str+ "'></td></tr>"
            "<tr><td>Farbe 2</td><td><input type='color' name='col2' value='" + stateCol2_str+ "'></td></tr>"
            "<tr><td>WLAN Name</td><td><input type='text' name='wifiSsid' value='" + _ssid + "'></td></tr>"
            "<tr><td>WLAN Passwort</td><td><input type='password' name='wifiPassword' value='" + _password + "'></td></tr>"
            "<table><br><input type='submit' value='Speichern'></form></body></html>");
    });

    _server.on("/", HTTP_POST, [this]() {
        for (int i = 0; i < _server.args(); i++) {
            if (_server.argName(i) == "showEsIst") {
                if (_server.arg(i) == "on") {
                    _showEsIst = true;
                } else if (_server.arg(i) == "off") {
                    _showEsIst = false;
                }
            } else if (_server.argName(i) == "brightness") {
                _display.setBrightness(_server.arg(i).toInt());
            } else if (_server.argName(i) == "col1") {
                _display.setColor1(parseRGB(_server.arg(i)));
            } else if (_server.argName(i) == "col2") {
                _display.setColor2(parseRGB(_server.arg(i)));
            } else if (_server.argName(i) == "wifiSsid") {
                _ssid = _server.arg(i);
            } else if (_server.argName(i) == "wifiPassword") {
                _password = _server.arg(i);
            }
            writeWirelessConfig(_ssid.c_str(), _password.c_str());
            connectWiFi(_ssid, _password, true);

            Serial.println("Wrote wireless config:");
            Serial.println("New SSID: " + _ssid);
            Serial.println("New Pass: " + _password);

            readWirelessConfig();

            Serial.println("EEPROM SSID: " + _ssid);
            Serial.println("EEPROM Pass: " + _password);
        }
        _server.send(200, "text/html", "<html><head><meta charset=\"UTF-8\"><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\"><title>Wordclock Configuration</title></head><body><a href=\"/\">Zurück</a></body></html>");
    });
    //_server.on("/color", handleColors);
    //_server.onNotFound(handleNotFound);
    _server.begin();
}
