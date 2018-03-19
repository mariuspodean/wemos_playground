#include "Arduino.h"
#include "WiFiMQTT.h"

//generate unique name from MAC addr
String macToStr(const uint8_t *mac)
{
    String result;

    for (int i = 0; i < 6; ++i)
    {
        result += String(mac[i], 16);

        if (i < 5)
        {
            result += ':';
        }
    }

    return result;
}

WiFiMQTT::WiFiMQTT(const char* mqtt_server_ip, const char* ssid, const char* password, long poke_delay)
{
    _mqtt_server_ip = mqtt_server_ip;
    _ssid = ssid;
    _password = password;
    _poke_delay = poke_delay;

    _mqtt_client_name = "esp8266-";

    _wifi_client = new WiFiClient();
    mqtt_client = new PubSubClient(*_wifi_client);
}

void WiFiMQTT::setup(MQTT_CALLBACK_SIGNATURE)
{
    _poke_delay_timer = millis();

    mqtt_client->setServer(_mqtt_server_ip, 1883);
    mqtt_client->setCallback(callback);

    //start wifi subsystem
    WiFi.begin(_ssid, _password);
    delay(500);

    // Generate client name based on MAC address and last 8 bits of microsecond counter
    uint8_t mac[6];
    WiFi.macAddress(mac);
    _mqtt_client_name += macToStr(mac);

    //attempt to connect to the WIFI network and then connect to the MQTT server
    _keep_alive();
}

void WiFiMQTT::_keep_alive()
{
    //attempt to connect to the wifi if connection is lost
    if (WiFi.status() != WL_CONNECTED)
    {
        //debug printing
        Serial.print("Connecting to ");
        Serial.println(_ssid);

        //loop while we wait for connection
        while (WiFi.status() != WL_CONNECTED)
        {
            delay(500);
            Serial.print(".");
        }

        //print out some more debug once connected
        Serial.println("");
        Serial.println("WiFi connected");
        Serial.println("IP address: ");
        Serial.println(WiFi.localIP());
    }

    //make sure we are connected to WIFI before attemping to reconnect to MQTT
    if (WiFi.status() == WL_CONNECTED)
    {
        // Loop until we're reconnected to the MQTT server
        while (!mqtt_client->connected())
        {
            Serial.print("Attempting MQTT connection... @ ");
            Serial.println(_mqtt_server_ip);
            
            //if connected, subscribe to the topic(s) we want to be notified about
            if (mqtt_client->connect((char*) _mqtt_client_name.c_str())) {
              Serial.print("MTQQ Connected");
            }
            //otherwise print failed for debugging
            else
            {
                Serial.print("failed, rc=");
                Serial.print(mqtt_client->state());
                Serial.println(" try again in 5 seconds");
                // Wait 3 seconds before retrying
                // delay(3000);
            }
        }
        Serial.println("WIFi connected");
    }
}

void WiFiMQTT::poke_server()
{
   if (millis() - _poke_delay_timer > _poke_delay)
   {
        _poke_delay_timer = millis();

        //reconnect if connection is lost
        if (!mqtt_client->connected())
        {
            Serial.println("resuscitating ... ");
            _keep_alive();
        }
        else
        {
            //maintain MQTT connection
            mqtt_client->loop();
        }
   }
}

boolean WiFiMQTT::publish(const char* topic, const char* payload)
{
    return mqtt_client->publish(topic, payload);
}

boolean WiFiMQTT::subscribe(const char* topic)
{
    return mqtt_client->subscribe(topic);
}
