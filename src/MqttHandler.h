#include <Arduino.h>
#include <PubSubClient.h>
#include <WifiHandler.h>

#define emergCuttofTime 30000

unsigned long UpdateTimeing = 5000;
unsigned long UpdateTimeLast = 0;

// MQTT Broker
const char *mqttServer = "mqtt01.klaffenboeck.eu";
const int mqttPort = 1883;
const char *mqttUser = "";
const char *mqttPassword = "";
WiFiClient espClient;
PubSubClient client(espClient);

void PublishStatus()
{
    if (Strip.getState())
    {
        client.publish(statusResponseTopic, "ON");
    }
    else
    {
        client.publish(statusResponseTopic, "OFF");
    }

    client.beginPublish(BrightnessResponseTopic, 3, true);
    client.write((Strip.getBrightness() / 100 % 10) + 48);
    client.write((Strip.getBrightness() / 10 % 10) + 48);
    client.write((Strip.getBrightness() % 10) + 48);
    client.endPublish();

    client.beginPublish(rgbResponseTopic, 11, true);
    client.write((Strip.getRed() / 100 % 10) + 48);
    client.write((Strip.getRed() / 10 % 10) + 48);
    client.write((Strip.getRed() % 10) + 48);
    client.write(44);
    client.write((Strip.getGreen() / 100 % 10) + 48);
    client.write((Strip.getGreen() / 10 % 10) + 48);
    client.write((Strip.getGreen() % 10) + 48);
    client.write(44);
    client.write((Strip.getBlue() / 100 % 10) + 48);
    client.write((Strip.getBlue() / 10 % 10) + 48);
    client.write((Strip.getBlue() % 10) + 48);
    client.endPublish();
}

void callback(char *topic, byte *payload, unsigned int length)
{
    String topicextract = topic;
    String payloadextract = "";

    for (int i = 0; i < length; i++)
    {
        payloadextract += (char)payload[i];
    }

    if (topicextract.equals(statusTopic))
    {

        if (payloadextract.equals("ON"))
        {
            Strip.turnOffOn(true);
        }
        else if (payloadextract.equals("OFF"))
        {
            Strip.turnOffOn(false);
        }
    }
    else if (topicextract.equals(BrightnessTopic))
    {
        Strip.setBrightness((byte)payloadextract.toInt());
    }
    else if (topicextract.equals(rgbTopic))
    {
        byte r = payloadextract.substring(0, payloadextract.indexOf(',')).toInt();
        byte g = payloadextract.substring(payloadextract.indexOf(',') + 1, payloadextract.lastIndexOf(',')).toInt();
        byte b = payloadextract.substring(payloadextract.lastIndexOf(',') + 1, payloadextract.length()).toInt();
        Strip.setColorFill(r, g, b);
    }
    PublishStatus();
}

bool connectToMqttServer()
{
    unsigned long ConnTryStartTime = millis();

    client.setServer(mqttServer, mqttPort);
    client.setCallback(callback);

    while (!client.connected())
    {
        Serial.println("Connecting to MQTT...");

        if (client.connect("UnderBedLightController", mqttUser, mqttPassword))
        {
            Serial.println("connected");
        }
        else
        {
            Serial.print("failed with state ");
            Serial.print(client.state());
            delay(1000);
        }
        if (millis() - ConnTryStartTime > emergCuttofTime)
        {
            return false;
        }
    }

    client.subscribe(statusTopic);
    client.subscribe(BrightnessTopic);
    client.subscribe(rgbTopic);
    return true;
}

void MqttHandle()
{
    if (!client.connected())
    {
        connectToMqttServer();
    }
    client.loop();

    if (millis() - UpdateTimeLast > UpdateTimeing)
    {
        UpdateTimeLast = millis();
        PublishStatus();
    }
}