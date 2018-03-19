#ifndef WifiMQTT_h
#define WifiMQTT_h

#include "Arduino.h"
#include <PubSubClient.h>
#include <ESP8266WiFi.h>

class WiFiMQTT
{
  public:
    WiFiMQTT(const char* mqtt_server_ip, const char* ssid, const char* password, long poke_delay);

    PubSubClient *mqtt_client;

    void setup(MQTT_CALLBACK_SIGNATURE);
    void poke_server();
    boolean publish(const char* topic, const char* payload);
    boolean subscribe(const char* topic);

  private:
    WiFiClient *_wifi_client;

    const char *_mqtt_server_ip;
    const char *_ssid;
    const char *_password;
    long _poke_delay_timer;
    long _poke_delay;
    String _mqtt_client_name;

    void _keep_alive();
};

#endif
