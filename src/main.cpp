#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <WiFiUdp.h>
#include <ArduinoJson.h>
#include <Adafruit_NeoPixel.h>
#include <ArduinoOTA.h>
#include <NTPClient.h>

#define LED_PIN1 2
#define LED_PIN2 0
#define NUM_LEDS1 4
#define NUM_LEDS2 0
#define LED_TYPE WS2812B

const char SSID[] = "A1-104259";
const char PWD[] = "A46942J6JB";
const long utcOffsetInSeconds = 3600;

ESP8266WebServer server(80);

WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org", utcOffsetInSeconds);

StaticJsonDocument<384> jsonDocument;
char buffer[384];

Adafruit_NeoPixel Strip_1(NUM_LEDS1, LED_PIN1, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel Strip_2(NUM_LEDS2, LED_PIN2, NEO_GRB + NEO_KHZ800);

struct color
{
    byte red = 0;
    byte green = 0;
    byte blue = 0;
    byte mode = 0;
    byte speed = 0;
    bool state = false;
} Strip_1_color, Strip_2_color;

struct DevideData
{
    IPAddress ipAddress;
    byte sysTime[4] = {0, 0, 0, 0};
    byte timeStart[3] = {0, 0, 0};
    byte timeStop[3] = {0, 0, 0};
} Device;

void connectToWiFi()
{
    Serial.print("Connecting to ");
    Serial.println(SSID);

    WiFi.begin(SSID, PWD);
    while (WiFi.status() != WL_CONNECTED)
    {
        Serial.print(".");
        delay(100);
    }
    Serial.print("Connected. IP: ");
    Serial.println(WiFi.localIP());
}

void add_json_object(char *tag, byte value)
{
    jsonDocument[tag] = value;
}
void add_json_object(char *tag, IPAddress value)
{
    jsonDocument[tag] = value;
}
void add_json_object(char *tag, const char *value)
{
    jsonDocument[tag] = value;
}
void add_json_object_Array(char *tag, byte sice, byte *value)
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
    // Get RGB components
    if (jsonDocument.containsKey("red"))
    {
        Strip_1_color.red = jsonDocument["red"];
    }
    if (jsonDocument.containsKey("green"))
    {
        Strip_1_color.green = jsonDocument["green"];
    }
    if (jsonDocument.containsKey("blue"))
    {
        Strip_1_color.blue = jsonDocument["blue"];
    }
    if (jsonDocument.containsKey("mode"))
    {
        Strip_1_color.mode = jsonDocument["mode"];
    }
    if (jsonDocument.containsKey("speed"))
    {
        Strip_1_color.speed = jsonDocument["speed"];
    }
    if (jsonDocument.containsKey("state"))
    {
        Strip_1_color.state = jsonDocument["state"];
    }
    // Respond to the client
    server.send(200, "application/json", "{}");
}
void setStrip2()
{
    String body = server.arg("plain");
    deserializeJson(jsonDocument, body);
    // Get RGB components
    if (jsonDocument.containsKey("red"))
    {
        Strip_2_color.red = jsonDocument["red"];
    }
    if (jsonDocument.containsKey("green"))
    {
        Strip_2_color.green = jsonDocument["green"];
    }
    if (jsonDocument.containsKey("blue"))
    {
        Strip_2_color.blue = jsonDocument["blue"];
    }
    if (jsonDocument.containsKey("mode"))
    {
        Strip_2_color.mode = jsonDocument["mode"];
    }
    if (jsonDocument.containsKey("speed"))
    {
        Strip_2_color.speed = jsonDocument["speed"];
    }
    if (jsonDocument.containsKey("state"))
    {
        Strip_2_color.state = jsonDocument["state"];
    }
    // Respond to the client
    server.send(200, "application/json", "{}");
}
void setDevideData()
{
    String body = server.arg("plain");
    deserializeJson(jsonDocument, body);

    Device.timeStart[0] = jsonDocument["TimeStart"][0];
    Device.timeStart[1] = jsonDocument["TimeStart"][1];
    Device.timeStart[2] = jsonDocument["TimeStart"][2];
    Device.timeStop[0] = jsonDocument["TimeStop"][0];
    Device.timeStop[1] = jsonDocument["TimeStop"][1];
    Device.timeStop[2] = jsonDocument["TimeStop"][2];
    // Respond to the client
    server.send(200, "application/json", "{}");
}

void getStrip1()
{
    jsonDocument.clear();
    add_json_object("red", Strip_1_color.red);
    add_json_object("green", Strip_1_color.green);
    add_json_object("blue", Strip_1_color.blue);
    add_json_object("mode", Strip_1_color.mode);
    add_json_object("speed", Strip_1_color.speed);
    add_json_object("state", Strip_1_color.state);
    serializeJson(jsonDocument, buffer);
    server.send(200, "application/json", buffer);
}
void getStrip2()
{
    jsonDocument.clear();
    add_json_object("red", Strip_2_color.red);
    add_json_object("green", Strip_2_color.green);
    add_json_object("blue", Strip_2_color.blue);
    add_json_object("mode", Strip_2_color.mode);
    add_json_object("speed", Strip_2_color.speed);
    add_json_object("state", Strip_2_color.state);
    serializeJson(jsonDocument, buffer);
    server.send(200, "application/json", buffer);
}
void getDevideData()
{
    jsonDocument.clear();
    add_json_object("IP", WiFi.localIP());
    add_json_object("WifiSSID", SSID);
    add_json_object("WifiPWD", PWD);

    add_json_object_Array("SysTime", 4, Device.sysTime);
    add_json_object_Array("TimeStart", 3, Device.timeStart);
    add_json_object_Array("TimeStop", 3, Device.timeStop);
    serializeJson(jsonDocument, buffer);
    server.send(200, "application/json", buffer);
}

void setup_routing()
{
    server.on("/SETStrip1", HTTP_POST, setStrip1);
    server.on("/SETStrip2", HTTP_POST, setStrip2);
    server.on("/SETDevideData", HTTP_POST, setDevideData);
    server.on("/GETStrip1", HTTP_GET, getStrip1);
    server.on("/GETStrip2", HTTP_GET, getStrip2);
    server.on("/GETDevideData", HTTP_GET, getDevideData);
    server.begin();
}

void handleOnOff()
{
    if (Strip_1_color.state)
    {
        Strip_1.fill(Strip_1.Color(Strip_1_color.red, Strip_1_color.green, Strip_1_color.blue));
        Strip_1.show();
    }
    else
    {
        Strip_1.fill(Strip_1.Color(0, 0, 0));
        Strip_1.show();
    }

    if (Strip_2_color.state)
    {
        Strip_2.fill(Strip_2.Color(Strip_2_color.red, Strip_2_color.green, Strip_2_color.blue));
        Strip_2.show();
    }
    else
    {
        Strip_2.fill(Strip_2.Color(0, 0, 0));
        Strip_2.show();
    }
}

void handleAutomaticTurnOnOf()
{
    // TODO start and stop on time
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

void setup()
{
    Serial.begin(9600);
    connectToWiFi();
    setup_routing();

    ArduinoOTA.begin();

    Strip_1.begin();
    Strip_2.begin();
    Strip_1.fill(Strip_1.Color(0, 0, 0));
    Strip_1.show();
    Strip_2.fill(Strip_2.Color(0, 0, 0));
    Strip_2.show();
}

void loop()
{
    server.handleClient();
    ArduinoOTA.handle();
    timeClient.update();
    handleAutomaticTurnOnOf();
    handleOnOff();
    Device.sysTime[0] = timeClient.getDay();
    Device.sysTime[1] = timeClient.getHours();
    Device.sysTime[2] = timeClient.getMinutes();
    Device.sysTime[3] = timeClient.getSeconds();
}