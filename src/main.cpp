#include <Arduino.h>
#include <EEPROM.h>
#include <ArduinoOTA.h>
#include <RestHandler.h>
#include <MqttHandler.h>

#define RedAddr 0
#define GreenAddr 1
#define BlueAddr 2
#define BrightnessAddr 3
#define StateAddr 4

void handleEEprom()
{
    if (Strip.getUpdated())
    {
        EEPROM.write(RedAddr, Strip.getRed());
        EEPROM.write(GreenAddr, Strip.getGreen());
        EEPROM.write(BlueAddr, Strip.getBlue());
        EEPROM.write(BrightnessAddr, Strip.getBrightness());
        EEPROM.write(StateAddr, Strip.getState());
        EEPROM.commit();
        Strip.setUpdated(false);
    }
}

void publischCurrantState() {}

void setup()
{
    Serial.begin(9600);
    EEPROM.begin(12);
    connectToWiFi();
    setup_routing();
    connectToMqttServer();
    ArduinoOTA.begin();

    byte red = EEPROM.read(RedAddr);
    byte green = EEPROM.read(GreenAddr);
    byte blue = EEPROM.read(BlueAddr);
    byte brightness = EEPROM.read(BrightnessAddr);
    byte state = EEPROM.read(StateAddr);

    Strip.setColorFill(red, green, blue);
    Strip.setBrightness(brightness);
    Strip.turnOffOn(state);
}

void loop()
{
    MqttHandle();
    RestHandler();
    handleEEprom();
    ArduinoOTA.handle();
}