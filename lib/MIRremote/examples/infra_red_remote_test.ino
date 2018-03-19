#include "MIRremote.h"

class HomeRemote : public MIRremote
{
public:
  HomeRemote(int pin, long reader_delay) : MIRremote(pin, reader_delay) {}

  void key_ok_pressed();
};

void HomeRemote::key_ok_pressed()
{
  Serial.print("WOWOWOWOWO");
  Serial.println();
}

const int RECV_PIN = D4;
long ir_read_delay = 250;
MIRremote *ir_remote = new HomeRemote(RECV_PIN, ir_read_delay);

void setup()
{
  Serial.begin(115200);

  ir_remote->setup_ir();
}

void loop()
{
  ir_remote->read_ir();
}
