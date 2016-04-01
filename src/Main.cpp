#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include <ArduinoOTA.h>
#include <APA102.h>
#include "Display.hpp"

#define SSID "HaSi-Kein-Internet-Legacy"
#define PASSWORD "bugsbunny"

ESP8266WebServer server(80);

const int led = 13;

const uint8_t dataPin = 12;
const uint8_t clockPin = 13;
APA102<dataPin, clockPin> ledStrip;
const uint16_t ledCount = 110;
const uint16_t ledBrightness = 5;
rgb_color color {
  .red = 255,
  .green = 0,
  .blue = 0
};
rgb_color colors[ledCount];

void fillMonoColor(rgb_color col) {
  for(int i = 0; i < ledCount; i++) {
    colors[i] = col;
  }
}

void clear() {
  rgb_color clearCol {
    .red = 0,
    .green = 0,
    .blue = 0
  };
  for(int i = 0; i < ledCount; i++) {
    colors[i] = clearCol;
  }
}

void setPixel(int x, int y, rgb_color col) {
  int xOffset = (y % 2 == 1 ? 10 - x : x);
  colors[y * 11 + xOffset] = col;
}

void displayEs(rgb_color col) {
  setPixel(0, 9, col);
  setPixel(1, 9, col);
}

void displayIst(rgb_color col) {
  setPixel(3, 9, col);
  setPixel(4, 9, col);
  setPixel(5, 9, col);
}

void displayViertel(rgb_color col) {
  setPixel(4, 7, col);
  setPixel(5, 7, col);
  setPixel(6, 7, col);
  setPixel(7, 7, col);
  setPixel(8, 7, col);
  setPixel(9, 7, col);
  setPixel(10, 7, col);
}

void displayVor(rgb_color col) {
  setPixel(6, 6, col);
  setPixel(7, 6, col);
  setPixel(8, 6, col);
}

void displayZwei(rgb_color col) {
  setPixel(0, 4, col);
  setPixel(1, 4, col);
  setPixel(2, 4, col);
  setPixel(3, 4, col);
}

void showDemoTime() {
  clear();
  displayEs(color);
  displayIst(color);
  displayViertel(color);
  displayVor(color);
  displayZwei(color);
}

void handleRoot() {
  digitalWrite(led, 1);
  server.send(200, "text/plain", "hello from esp8266!");
  digitalWrite(led, 0);
}

void handleColors() {
  server.send(200, "text/html", "<html><head><title>Colors</title></head>"
  "<body><form action='' method='get'>"
    "red: <input type='text' name='red'><br/>"
    "green: <input type='text' name='green'><br/>"
    "blue: <input type='text' name='blue'><br/>"
    "<input type='submit' value='Set'>"
  "</form></body></html>");
  rgb_color newCol;
  for (int i = 0; i < server.args(); i++) {
    if (server.argName(i) == "red") {
      newCol.red = atoi(server.arg(i).c_str());
    } else if (server.argName(i) == "green") {
      newCol.green = atoi(server.arg(i).c_str());
    } else if (server.argName(i) == "blue") {
      newCol.blue = atoi(server.arg(i).c_str());
    }
  }
  fillMonoColor(newCol);
}

void handleNotFound() {
  digitalWrite(led, 1);
  String message = "File Not Found\n\n";
  message += "URI: ";
  message += server.uri();
  message += "\nMethod: ";
  message += (server.method() == HTTP_GET)?"GET":"POST";
  message += "\nArguments: ";
  message += server.args();
  message += "\n";
  for (uint8_t i=0; i<server.args(); i++){
    message += " " + server.argName(i) + ": " + server.arg(i) + "\n";
  }
  server.send(404, "text/plain", message);
  digitalWrite(led, 0);
}

void setup(void){
  pinMode(led, OUTPUT);
  digitalWrite(led, 0);
  Serial.begin(115200);
  WiFi.begin(SSID, PASSWORD);
  Serial.println("");

  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(SSID);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
  Serial.print("Number of LEDs: ");
  Serial.println(ledCount);

  if (MDNS.begin("esp8266")) {
    Serial.println("MDNS responder started");
  }

  server.on("/", handleRoot);

  server.on("/color", handleColors);

  server.onNotFound(handleNotFound);

  server.begin();
  Serial.println("HTTP server started");

  rgb_color start;
  start.red = 255;
  start.green = 255;
  start.blue = 255;
  //fillMonoColor(start);
  //fillRainbow();
  showDemoTime();
}

void loop(void){
  server.handleClient();
  ledStrip.write(colors, ledCount, ledBrightness);
}
