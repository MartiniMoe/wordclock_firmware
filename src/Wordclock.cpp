#include "Wordclock.hpp"

Wordclock::Wordclock()
    : _server(80),
      _display() {
        MDNS.begin("wordclock");
}

void Wordclock::loop() {
    _server.handleClient();
    _display.clearPixels( rgb_color { .red = 0, .green = 255, .blue = 0 });
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

String Wordclock::connectWiFi(String ssid, String password) {
    WiFi.begin(ssid.c_str(), password.c_str());

    while (WiFi.status() != WL_CONNECTED) {
      delay(500);
      Serial.print(".");
    }
    Serial.println();

    return WiFi.localIP().toString();
}

void Wordclock::setupNtp() {
    _ntp = ntpClient::getInstance("pool.ntp.org", 1);
    _ntp->setInterval(15, 1800);
    _ntp->begin();
}

void Wordclock::handleTime(int hour, int minute) {
    _display.displayEs();
    _display.displayIst();
    if (minute < 30) {
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

void Wordclock::handleRoot() {
    _server.send(200, "text/plain", "hello from esp8266!");
}

void Wordclock::handleColors() {
    _server.send(200, "text/html", "<html><head><title>Colors</title></head>"
        "<body><form action='' method='get'>"
        "red: <input type='text' name='red'><br/>"
        "green: <input type='text' name='green'><br/>"
        "blue: <input type='text' name='blue'><br/>"
        "<input type='submit' value='Set'>"
        "</form></body></html>");
    //rgb_color newCol;
    //for (int i = 0; i < _server.args(); i++) {
    //    if (server.argName(i) == "red") {
    //        newCol.red = atoi(_server.arg(i).c_str());
    //    } else if (server.argName(i) == "green") {
    //        newCol.green = atoi(_server.arg(i).c_str());
    //    } else if (server.argName(i) == "blue") {
    //        newCol.blue = atoi(_server.arg(i).c_str());
    //    }
    //}
    //fillMonoColor(newCol);
}

void Wordclock::handleNotFound() {
    String message = "File Not Found";
    _server.send(404, "text/plain", message);
}

void Wordclock::setupWebserver() {
    _server.on("/", [this]() {
        _server.send(200, "text/plain", "hello from esp8266!");
    });
    //_server.on("/color", handleColors);
    //_server.onNotFound(handleNotFound);
    _server.begin();
}
