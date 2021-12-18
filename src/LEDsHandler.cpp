#include <Adafruit_NeoPixel.h>

class LedStrip
{
    Adafruit_NeoPixel Strip;
    byte red = 0;
    byte green = 0;
    byte blue = 0;
    byte mode = 0;
    byte speed = 0;
    bool state = false;

public:
    LedStrip(byte NUM_LEDS, byte LED_PIN)
    {
        Strip = Adafruit_NeoPixel(NUM_LEDS, LED_PIN, NEO_GRB + NEO_KHZ800);
        Strip.begin();
        Strip.fill(Strip.Color(0, 0, 0));
        Strip.show();
    }

    void setColorFill(byte r, byte g, byte b)
    {
        red = r;
        green = g;
        blue = b;
        state = true;
        Strip.fill(Strip.Color(red, green, blue));
        Strip.show();
    }

    void setSpecialMode(byte me, byte sd)
    {
        mode = me;
        speed = sd;
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
    byte getMode()
    {
        return mode;
    }
    byte getSpeed()
    {
        return speed;
    }
    bool getState()
    {
        return state;
    }
};
