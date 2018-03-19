#ifndef MIRremote_h
#define MIRremote_h

#include "Arduino.h"
#include <IRrecv.h>

class MIRremote
{
public:
  MIRremote(int pin, long reader_delay);

  long _reader_delay;
  void setup_ir();
  void read_ir();

  virtual void key_ok_pressed(void);
  virtual void key_up_pressed(void);
  virtual void key_down_pressed(void);
  virtual void key_left_pressed(void);
  virtual void key_right_pressed(void);
  virtual void key_star_pressed(void);
  virtual void key_hashtag_pressed(void);
  virtual void key_0_pressed(void);
  virtual void key_1_pressed(void);
  virtual void key_2_pressed(void);
  virtual void key_3_pressed(void);
  virtual void key_4_pressed(void);
  virtual void key_5_pressed(void);
  virtual void key_6_pressed(void);
  virtual void key_7_pressed(void);
  virtual void key_8_pressed(void);
  virtual void key_9_pressed(void);

private:
  int _pin;
  IRrecv *_irrecv;
  decode_results _reading_results;
  long _reader_delay_timer;

  void _decodeIR();
};

#endif
