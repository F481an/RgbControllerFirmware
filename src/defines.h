#include <Arduino.h>
#include <LEDHandler.h>

#define LED_PIN1 2
#define LED_PIN2 0
#define NUM_LEDS1 36
#define NUM_LEDS2 0
#define LED_TYPE WS2812B

#define statusTopic "bedroom/UnderBedLight/light/set"
#define BrightnessTopic "bedroom/UnderBedLight/brightness/set"
#define rgbTopic "bedroom/UnderBedLight/rgb/set"

#define statusResponseTopic "bedroom/UnderBedLight/light/status"
#define BrightnessResponseTopic "bedroom/UnderBedLight/brightness/status"
#define rgbResponseTopic "bedroom/UnderBedLight/rgb/status"

const char SSID[] = "Klaffenboeck_Wifi";
const char PWD[] = "31862025034211431218";

LedStrip Strip(NUM_LEDS1, LED_PIN1);