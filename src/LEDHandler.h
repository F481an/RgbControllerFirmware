#include <Arduino.h>
#include <Adafruit_NeoPixel.h>

class LedStrip
{
    Adafruit_NeoPixel Strip;
    byte brightness = 0;
    byte red = 0;
    byte green = 0;
    byte blue = 0;
    bool state = false;
    bool updated = false;

public:
    LedStrip(byte NUM_LEDS, byte LED_PIN)
    {
        Strip = Adafruit_NeoPixel(NUM_LEDS, LED_PIN, NEO_GRB + NEO_KHZ800);
        Strip.begin();
        Strip.fill(Strip.Color(0, 0, 0));
        Strip.show();
        updated = true;
    }

    void setColorFill(byte r, byte g, byte b)
    {
        red = r;
        green = g;
        blue = b;
        state = true;
        Strip.fill(Strip.Color(red, green, blue));
        Strip.show();
        updated = true;
    }

    void setBrightness(byte br)
    {
        brightness = br;
        Strip.setBrightness(brightness);
        updated = true;
    }

    void turnOffOn(bool st)
    {
        state = st;
        if (state)
        {
            Strip.fill(Strip.Color(red, green, blue));
            Strip.show();
        }
        else
        {
            Strip.fill(Strip.Color(0, 0, 0));
            Strip.show();
        }
        updated = true;
    }

    byte getRed()
    {
        return red;
    }
    byte getGreen()
    {
        return green;
    }
    byte getBlue()
    {
        return blue;
    }
    bool getState()
    {
        return state;
    }
    byte getBrightness()
    {
        return brightness;
    }
    bool getUpdated()
    {
        return updated;
    }
    void setUpdated(bool up)
    {
        updated = up;
    }
};
