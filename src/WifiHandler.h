#include <Arduino.h>
#include <ESP8266WiFi.h>

IPAddress local_IP(192, 168, 188, 130);
IPAddress gateway(192, 168, 188, 1);
IPAddress subnet(255, 255, 255, 0);
IPAddress primaryDNS(192, 168, 188, 101);

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
