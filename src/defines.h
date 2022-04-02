#include <Arduino.h>
#include <LEDsHandler.h>

#define LED_PIN1 2
#define LED_PIN2 0
#define NUM_LEDS1 36
#define NUM_LEDS2 0
#define LED_TYPE WS2812B
#define ONOFTopic "bedroom/UnderBedLight/switch"
#define BrightnessTopic "bedroom/UnderBedLight/brightness"
#define rgbTopic "bedroom/UnderBedLight/rgb"

const char SSID[] = "Klaffenboeck_Wifi";
const char PWD[] = "31862025034211431218";

LedStrip Strip(NUM_LEDS1, LED_PIN1);