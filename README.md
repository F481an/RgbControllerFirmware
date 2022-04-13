The Set and respons Topics can be set with the defines in the defiens.h File


```c++
#define statusTopic "bedroom/UnderBedLight/light/set"
#define BrightnessTopic "bedroom/UnderBedLight/brightness/set"
#define rgbTopic "bedroom/UnderBedLight/rgb/set"

#define statusResponseTopic "bedroom/UnderBedLight/light/status"
#define BrightnessResponseTopic "bedroom/UnderBedLight/brightness/status"
#define rgbResponseTopic "bedroom/UnderBedLight/rgb/status"
```


This is a working example for the configguration in HomeAssistant
```c++
light:
  - platform: mqtt
    name: "UnderBedLight"
    command_topic: "bedroom/UnderBedLight/light/set"
    brightness_command_topic: "bedroom/UnderBedLight/brightness/set"
    rgb_command_topic: "bedroom/UnderBedLight/rgb/set"
    state_topic: "bedroom/UnderBedLight/light/status"
    brightness_state_topic: "bedroom/UnderBedLight/brightness/status"
    rgb_state_topic: "bedroom/UnderBedLight/rgb/status"
    payload_on: "ON"
    payload_off: "OFF"
    optimistic: false
```