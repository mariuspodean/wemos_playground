#include "WiFiMQTT.h"

// ****** Network settings ******
const char* mqtt_server_ip = "192.168.100.10";
const char* ssid = "ssid";
const char* password = "password";

// ****** Power Sockets ******
#define SOCKET1 D5
#define SOCKET2 D6

// ****** MQTT Queues ******
char* SOCKET1Queue = "/SOCKET1/command";
char* SOCKET1StatusQueue = "/SOCKET1/status";
char* SOCKET2Queue = "/SOCKET2/command";
char* SOCKET2StatusQueue = "/SOCKET2/status";

long server_poke_delay = 1000;
WiFiMQTT mqtt_server(mqtt_server_ip, ssid, password, server_poke_delay);

void callback(char* topic, byte* payload, unsigned int length) {
  char command = (char)payload[0];
  
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  Serial.print((char)command);
  Serial.println();

  if(strcmp(topic, SOCKET1Queue) == 0) {
     if(command == '1') { //ON
        Serial.println("SOCKET1Queue >> ON");
        digitalWrite(SOCKET1, LOW);
        mqtt_server.publish(SOCKET1StatusQueue,"1");
     } else {
        Serial.println("SOCKET1Queue >> OFF");
        digitalWrite(SOCKET1, HIGH);
        mqtt_server.publish(SOCKET1StatusQueue,"0"); 
     }
  }
  
  if(strcmp(topic, SOCKET2Queue) == 0) {
     if(command == '1') { //ON
        Serial.println("SOCKET2Queue >> ON");
        digitalWrite(SOCKET2, LOW);
        mqtt_server.publish(SOCKET2StatusQueue,"1");
     } else {
        Serial.println("SOCKET2Queue >> OFF");
        digitalWrite(SOCKET2, HIGH);
        mqtt_server.publish(SOCKET2StatusQueue,"0"); 
     }
  }
}

void setup() {
  //start the serial line for debugging
  Serial.begin(9600);
  delay(100);

  mqtt_server.setup(callback);
  mqtt_server.subscribe(SOCKET1Queue);
  mqtt_server.subscribe(SOCKET2Queue);

  pinMode(SOCKET1, OUTPUT);
  digitalWrite(SOCKET1, HIGH); // setting to HIGH (1) opens the relay, therefore no current is flowing, so the socket has no power
  pinMode(SOCKET2, OUTPUT); 
  digitalWrite(SOCKET2, HIGH);

  //wait a bit before starting the main loop
  delay(2000);
}

void loop(){
  mqtt_server.poke_server();
}
