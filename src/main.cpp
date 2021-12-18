#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <WiFiUdp.h>
#include <ArduinoJson.h>
#include <ArduinoOTA.h>
#include <NTPClient.h>
#include "LEDsHandler.cpp"

#define LED_PIN1 2
#define LED_PIN2 0
#define NUM_LEDS1 36
#define NUM_LEDS2 0
#define LED_TYPE WS2812B

#define Day 0
#define Hour 1
#define Minute 2
#define Second 3

const char SSID[] = "Klaffenboeck_Wifi";
const char PWD[] = "31862025034211431218";

IPAddress local_IP(192, 168, 188, 130);
IPAddress gateway(192, 168, 188, 1);
IPAddress subnet(255, 255, 255, 0);
IPAddress primaryDNS(192, 168, 188, 103);

LedStrip Strip1(NUM_LEDS1, LED_PIN1);

ESP8266WebServer server(80);

StaticJsonDocument<384> jsonDocument;
char buffer[384];

struct DevideData
{
    IPAddress ipAddress;
    byte sysTime[4] = {0, 0, 0, 0};
    byte timeStart[4] = {0, 0, 0, 0};
    byte timeStop[4] = {0, 0, 0, 0};
} Device;

void connectToWiFi()
{
    if (!WiFi.config(local_IP, gateway, subnet, primaryDNS))
    {
        Serial.println("STA Failed to configure");
    }

    Serial.print("Connecting to ");
    Serial.println(SSID);

    WiFi.begin(SSID, PWD);
    while (WiFi.status() != WL_CONNECTED)
    {
        Serial.print(".");
        delay(100);
    }
    Serial.println("WiFi connected.");
    Serial.println("IP address: ");
    Serial.println(WiFi.localIP());
}

void handleAutomaticTurnOnOf()
{
    // TODO automaticly turn of the leds on a time
}
void loadEEprom()
{
    // TODO Load wifi data from eeprom
    // TODO Load automatic start and stop time
}
void writeEEprom()
{
    // TODO Write wifi data to eeprom
    // TODO Write automatic start and stop time
}

void add_json_object_Array(const char *tag, byte sice, byte *value)
{
    JsonArray arr = jsonDocument.createNestedArray(tag);

    for (byte i = 0; i < sice; i++)
    {
        arr.add(value[i]);
    }
}

void setStrip1()
{
    String body = server.arg("plain");
    deserializeJson(jsonDocument, body);
    byte red = jsonDocument["red"];
    byte green = jsonDocument["green"];
    byte blue = jsonDocument["blue"];

    Strip1.setColorFill(red, green, blue);
    server.send(200, "application/json", "{}");
}
void setStrip1SpecialMode()
{
    String body = server.arg("plain");
    deserializeJson(jsonDocument, body);
    byte mode = jsonDocument["mode"];
    byte speed = jsonDocument["speed"];
    byte state = jsonDocument["state"];

    Strip1.setSpecialMode(mode, speed);
    server.send(200, "application/json", "{}");
}
void setStrip1OffOn()
{
    String body = server.arg("plain");
    deserializeJson(jsonDocument, body);
    bool state = jsonDocument["state"];

    Strip1.turnOffOn(state);
    server.send(200, "application/json", "{}");
}

void getStrip1()
{
    jsonDocument.clear();
    jsonDocument["red"] = Strip1.getRed();
    jsonDocument["green"] = Strip1.getGreen();
    jsonDocument["blue"] = Strip1.getBlue();
    jsonDocument["mode"] = Strip1.getMode();
    jsonDocument["speed"] = Strip1.getSpeed();
    jsonDocument["state"] = Strip1.getState();

    serializeJson(jsonDocument, buffer);
    server.send(200, "application/json", buffer);
}

void setDevideData()
{
    String body = server.arg("plain");
    deserializeJson(jsonDocument, body);

    Device.timeStart[Hour] = jsonDocument["TimeStart"][0];
    Device.timeStart[Minute] = jsonDocument["TimeStart"][1];
    Device.timeStart[Second] = jsonDocument["TimeStart"][2];
    Device.timeStop[Hour] = jsonDocument["TimeStop"][0];
    Device.timeStop[Minute] = jsonDocument["TimeStop"][1];
    Device.timeStop[Second] = jsonDocument["TimeStop"][2];
    // Respond to the client
    server.send(200, "application/json", "{}");
}
void getDevideData()
{
    jsonDocument.clear();
    jsonDocument["IP"] = WiFi.localIP();
    jsonDocument["WifiSSID"] = SSID;
    jsonDocument["WifiPWD"] = PWD;

    add_json_object_Array("SysTime", 4, Device.sysTime);
    add_json_object_Array("TimeStart", 4, Device.timeStart);
    add_json_object_Array("TimeStop", 4, Device.timeStop);
    serializeJson(jsonDocument, buffer);
    server.send(200, "application/json", buffer);
}

void setup_routing()
{
    server.on("/Strip1/color", HTTP_POST, setStrip1);
    server.on("/Strip1/SpecialMode", HTTP_POST, setStrip1SpecialMode);
    server.on("/Strip1/OffOn", HTTP_POST, setStrip1OffOn);
    server.on("/Strip1", HTTP_GET, getStrip1);
    server.on("/DevideData", HTTP_POST, setDevideData);
    server.on("/DevideData", HTTP_GET, getDevideData);
    server.begin();
}

void setup()
{
    Serial.begin(9600);
    connectToWiFi();
    setup_routing();

    ArduinoOTA.begin();
}

void loop()
{
    server.handleClient();
    ArduinoOTA.handle();
}