#include <Arduino.h>
#include <ArduinoJson.h>
#include <ESP8266WebServer.h>
#include <WiFiUdp.h>
#include <defines.h>


ESP8266WebServer server(80);
WiFiClient espClient;
StaticJsonDocument<384> jsonDocument;
char buffer[384];

void RestHandler()
{
    server.handleClient();
}

void add_json_object_Array(const char *tag, byte sice, byte *value)
{
    JsonArray arr = jsonDocument.createNestedArray(tag);

    for (byte i = 0; i < sice; i++)
    {
        arr.add(value[i]);
    }
}

void setStrip()
{
    String body = server.arg("plain");
    deserializeJson(jsonDocument, body);
    byte red = jsonDocument["red"];
    byte green = jsonDocument["green"];
    byte blue = jsonDocument["blue"];

    Strip.setColorFill(red, green, blue);
    server.send(200, "application/json", "{}");
}
void setStripOffOn()
{
    String body = server.arg("plain");
    deserializeJson(jsonDocument, body);
    bool state = jsonDocument["state"];

    Strip.turnOffOn(state);
    server.send(200, "application/json", "{}");
}
void getStrip()
{
    jsonDocument.clear();
    jsonDocument["red"] = Strip.getRed();
    jsonDocument["green"] = Strip.getGreen();
    jsonDocument["blue"] = Strip.getBlue();
    jsonDocument["brightness"] = Strip.getBrightness();
    jsonDocument["state"] = Strip.getState();

    serializeJson(jsonDocument, buffer);
    server.send(200, "application/json", buffer);
}
void getDevideData()
{
    jsonDocument.clear();
    jsonDocument["IP"] = WiFi.localIP();
    jsonDocument["WifiSSID"] = SSID;
    jsonDocument["WifiPWD"] = PWD;

    serializeJson(jsonDocument, buffer);
    server.send(200, "application/json", buffer);
}
void setbrightness()
{
    String body = server.arg("plain");
    deserializeJson(jsonDocument, body);
    byte brightness = jsonDocument["brightness"];

    Strip.setBrightness(brightness);
    server.send(200, "application/json", "{}");
}
void setDevideData()
{
    String body = server.arg("plain");
    deserializeJson(jsonDocument, body);

    // Respond to the client
    server.send(200, "application/json", "{}");
}
void setup_routing()
{
    server.on("/Strip1/color", HTTP_POST, setStrip);
    server.on("/Strip1/OffOn", HTTP_POST, setStripOffOn);
    server.on("/Strip1/brightness", HTTP_POST, setbrightness);
    server.on("/Strip1", HTTP_GET, getStrip);
    server.on("/DevideData", HTTP_POST, setDevideData);
    server.on("/DevideData", HTTP_GET, getDevideData);
    server.begin();
}
