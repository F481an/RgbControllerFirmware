#include <Arduino.h>
#include <PubSubClient.h>
#include <WifiHandler.h>

// MQTT Broker
const char *mqttServer = "192.168.188.103";
const int mqttPort = 1883;
const char *mqttUser = "";
const char *mqttPassword = "";
PubSubClient client(espClient);

void callback(char *topic, byte *payload, unsigned int length)
{
    String topicextract = topic;
    String payloadextract = "";
    int pos = topicextract.lastIndexOf("/") + 1;
    topicextract = topicextract.substring(pos, topicextract.length());

    for (int i = 0; i < length; i++)
    {
        payloadextract += (char)payload[i];
    }

    if (topicextract.equals("switch"))
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
    else if (topicextract.equals("brightness"))
    {
        Strip.setBrightness((byte)payloadextract.toInt());
    }
    else if (topicextract.equals("rgb"))
    {
        byte r = payloadextract.substring(0, payloadextract.indexOf(',')).toInt();
        byte g = payloadextract.substring(payloadextract.indexOf(',') + 1, payloadextract.lastIndexOf(',')).toInt();
        byte b = payloadextract.substring(payloadextract.lastIndexOf(',') + 1, payloadextract.length()).toInt();
        Strip.setColorFill(r, g, b);
    }

    Serial.print("Topic: ");
    Serial.println(topicextract);
    Serial.print("Message: ");
    Serial.println(payloadextract);
}

void connectToMqttServer()
{
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
            delay(2000);
        }
    }

    client.subscribe("bedroom/UnderBedLight/switch");
    client.subscribe("bedroom/UnderBedLight/brightness");
    client.subscribe("bedroom/UnderBedLight/rgb");
}

void MqttHandle()
{
    if (!client.connected())
    {
        connectToMqttServer();
    }
    client.loop();
}