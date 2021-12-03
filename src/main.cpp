#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <ArduinoJson.h>
#include <Adafruit_NeoPixel.h>
#include <ArduinoOTA.h>

#define LED_PIN1 2
#define LED_PIN2 0
#define NUM_LEDS1 4
#define NUM_LEDS2 0
#define LED_TYPE WS2812B

const char *SSID = "A1-104259";
const char *PWD = "A46942J6JB";

ESP8266WebServer server(80);

Adafruit_NeoPixel Strip_1(NUM_LEDS1, LED_PIN1, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel Strip_2(NUM_LEDS2, LED_PIN2, NEO_GRB + NEO_KHZ800);

struct color
{
    byte red = 0;
    byte green = 0;
    byte blue = 0;

} Strip_1_color, Strip_2_color;

StaticJsonDocument<250> jsonDocument;
char buffer[250];

void connectToWiFi()
{
    Serial.print("Connecting to ");
    Serial.println(SSID);

    WiFi.begin(SSID, PWD);

    while (WiFi.status() != WL_CONNECTED)
    {
        Serial.print(".");
        delay(500);
        // we can even make the ESP32 to sleep
    }

    Serial.print("Connected. IP: ");
    Serial.println(WiFi.localIP());
}

void add_json_object(char *tag, float value)
{
    JsonObject obj = jsonDocument.createNestedObject();
    obj["type"] = tag;
    obj["value"] = value;
}

void setStrip1()
{
    if (server.hasArg("plain") == false)
    {
        // handle error here
    }
    String body = server.arg("plain");
    deserializeJson(jsonDocument, body);

    // Get RGB components
    Strip_1_color.red = jsonDocument["red"];
    Strip_1_color.green = jsonDocument["green"];
    Strip_1_color.blue = jsonDocument["blue"];
    Strip_1.fill(Strip_1.Color(Strip_1_color.red, Strip_1_color.green, Strip_1_color.blue));
    Strip_1.show();
    // Respond to the client
    server.send(200, "application/json", "{}");
}

void setStrip2()
{
    if (server.hasArg("plain") == false)
    {
        // handle error here
    }
    String body = server.arg("plain");
    deserializeJson(jsonDocument, body);

    // Get RGB components
    Strip_2_color.red = jsonDocument["red"];
    Strip_2_color.green = jsonDocument["green"];
    Strip_2_color.blue = jsonDocument["blue"];
    Strip_2.fill(Strip_2.Color(Strip_2_color.red, Strip_2_color.green, Strip_2_color.blue));
    Strip_2.show();
    // Respond to the client
    server.send(200, "application/json", "{}");
}

void getStrip1()
{
    jsonDocument.clear();
    add_json_object("red", Strip_1_color.red);
    add_json_object("green", Strip_1_color.green);
    add_json_object("blue", Strip_1_color.blue);
    serializeJson(jsonDocument, buffer);
    server.send(200, "application/json", buffer);
}
void getStrip2()
{
    jsonDocument.clear();
    add_json_object("red", Strip_2_color.red);
    add_json_object("green", Strip_2_color.green);
    add_json_object("blue", Strip_2_color.blue);
    serializeJson(jsonDocument, buffer);
    server.send(200, "application/json", buffer);
}

void setup_routing()
{
    server.on("/GETStrip1", HTTP_GET, getStrip1);
    server.on("/SETStrip1", HTTP_POST, setStrip1);
    server.on("/GETStrip2", HTTP_GET, getStrip2);
    server.on("/SETStrip2", HTTP_POST, setStrip2);
    server.begin();
}

void setup()
{
    Serial.begin(9600);
    connectToWiFi();
    setup_routing();

    ArduinoOTA.begin();

    Strip_1.begin();
    Strip_2.begin();
}

void loop()
{
    server.handleClient();
    ArduinoOTA.handle();
}