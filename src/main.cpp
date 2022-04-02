#include <Arduino.h>
#include <ArduinoOTA.h>
#include <RestHandler.h>
#include <MqttHandler.h>


void handleEEprom()
{
    // TODO Write wifi data to eeprom
    // TODO Write automatic start and stop time
}


void publischCurrantState() {}

void setup()
{
    Serial.begin(9600);
    connectToWiFi();
    setup_routing();
    connectToMqttServer();
    ArduinoOTA.begin();
}

void loop()
{
    MqttHandle();
    RestHandler();
    ArduinoOTA.handle();
}