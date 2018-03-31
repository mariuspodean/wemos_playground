#include "WiFiMQTT.h"
#include "MIRremote.h"

// ****** Network settings ******
const char* mqtt_server_ip = "192.168.100.10";
const char* ssid = "ssid";
const char* password = "password";

// ****** Power Sockets ******
#define SOCKET1 D5
#define SOCKET2 D6
const int RECV_PIN = D4;

// ****** MQTT Queues ******
char* SOCKET1Queue = "/SOCKET1/command";
char* SOCKET1StatusQueue = "/SOCKET1/status";
char* SOCKET2Queue = "/SOCKET2/command";
char* SOCKET2StatusQueue = "/SOCKET2/status";

int socket_1_status = 0;
int socket_2_status = 0;

class HomeRemote : public MIRremote
{
public:
  HomeRemote(int pin, long reader_delay) : MIRremote(pin, reader_delay) {}

  void key_1_pressed();
  void key_2_pressed();
};

long ir_read_delay = 250;
MIRremote *ir_remote = new HomeRemote(RECV_PIN, ir_read_delay);

long server_poke_delay = 1000;
WiFiMQTT mqtt_server(mqtt_server_ip, ssid, password, server_poke_delay);

void HomeRemote::key_1_pressed()
{
  Serial.print("Toggle socket 1");
  Serial.println();
  Serial.print(socket_1_status);

  if(socket_1_status == 0) {
    mqtt_server.publish(SOCKET1Queue, "1");
  } else {
    mqtt_server.publish(SOCKET1Queue, "0");
  }
}

void HomeRemote::key_2_pressed()
{
  Serial.print("Toggle socket 2");
  Serial.println();
  Serial.print(socket_2_status);

  if(socket_2_status == 0) {
    mqtt_server.publish(SOCKET2Queue, "1");
  } else {
    mqtt_server.publish(SOCKET2Queue, "0");
  }
}

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

  if(strcmp(topic, SOCKET1StatusQueue) == 0) {
    if(command == '1') { //ON
      socket_1_status = 1;
    } else {
      socket_1_status = 0;
    }
  }

  if(strcmp(topic, SOCKET2StatusQueue) == 0) {
    if(command == '1') { //ON
      socket_2_status = 1;
    } else {
      socket_2_status = 0;
    }
  }
}

void setup() {
  //start the serial line for debugging
  Serial.begin(9600);
  delay(100);

  ir_remote->setup_ir();

  mqtt_server.setup(callback);
  mqtt_server.subscribe(SOCKET1Queue);
  mqtt_server.subscribe(SOCKET2Queue);
  mqtt_server.subscribe(SOCKET1StatusQueue);
  mqtt_server.subscribe(SOCKET2StatusQueue);

  pinMode(SOCKET1, OUTPUT);
  digitalWrite(SOCKET1, HIGH); // setting to HIGH (1) opens the relay, therefore no current is flowing, so the socket has no power
  pinMode(SOCKET2, OUTPUT); 
  digitalWrite(SOCKET2, HIGH);

  //wait a bit before starting the main loop
  delay(2000);
}

void loop(){
  ir_remote->read_ir();
  mqtt_server.poke_server();
}
