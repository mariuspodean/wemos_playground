#include "Arduino.h"
#include "MIRremote.h"
#include <IRremoteESP8266.h>
#include <IRrecv.h>
#include <IRutils.h>

MIRremote::MIRremote(int pin, long reader_delay)
{
  _pin = pin;
  _irrecv = new IRrecv(pin);
  _reader_delay = reader_delay;
}

void MIRremote::setup_ir()
{
  _irrecv->enableIRIn();
  _reader_delay_timer = millis();

  Serial.println();
  Serial.print("Waiting for IR messages on pin ");
  Serial.println(_pin);
}

void MIRremote::key_ok_pressed()
{
  Serial.println("OK");
}

void MIRremote::key_up_pressed()
{
  Serial.println("Up Arrow");
}

void MIRremote::key_left_pressed()
{
  Serial.println("Left Arrow");
}

void MIRremote::key_right_pressed()
{
  Serial.println("Right Arrow");
}

void MIRremote::key_down_pressed()
{
  Serial.println("Down Arrow");
}

void MIRremote::key_1_pressed()
{
  Serial.println("1");
}

void MIRremote::key_2_pressed()
{
  Serial.println("2");
}

void MIRremote::key_3_pressed()
{
  Serial.println("3");
}

void MIRremote::key_4_pressed()
{
  Serial.println("4");
}

void MIRremote::key_5_pressed()
{
  Serial.println("5");
}

void MIRremote::key_6_pressed()
{
  Serial.println("6");
}

void MIRremote::key_7_pressed()
{
  Serial.println("7");
}

void MIRremote::key_8_pressed()
{
  Serial.println("8");
}
void MIRremote::key_9_pressed()
{
  Serial.println("9");
}

void MIRremote::key_0_pressed()
{
  Serial.println("0");
}

void MIRremote::key_star_pressed()
{
  Serial.println("*");
}

void MIRremote::key_hashtag_pressed()
{
  Serial.println("#");
}

void MIRremote::_decodeIR()
{
  switch (_reading_results.value)
  {

  case 0xFF629D:
    key_up_pressed();
    break;

  case 0xFF22DD:
    key_left_pressed();
    break;

  case 0xFF02FD:
    key_ok_pressed();
    break;

  case 0xFFC23D:
    key_right_pressed();
    break;

  case 0xFFA857:
    key_down_pressed();
    break;

  case 0xFF6897:
    key_1_pressed();
    break;

  case 0xFF9867:
    key_2_pressed();
    break;

  case 0xFFB04F:
    key_3_pressed();
    break;

  case 0xFF30CF:
    key_4_pressed();
    break;

  case 0xFF18E7:
    key_5_pressed();
    break;

  case 0xFF7A85:
    key_6_pressed();
    break;

  case 0xFF10EF:
    key_7_pressed();
    break;

  case 0xFF38C7:
    key_8_pressed();
    break;

  case 0xFF5AA5:
    key_9_pressed();
    break;

  case 0xFF42BD:
    key_star_pressed();
    break;

  case 0xFF4AB5:
    key_0_pressed();
    break;

  case 0xFF52AD:
    key_hashtag_pressed();
    break;

  default:
    break;
  }
}

void MIRremote::read_ir()
{
  if (millis() - _reader_delay_timer > _reader_delay)
  {
    _reader_delay_timer = millis();

    if (_irrecv->decode(&_reading_results))
    {
      _decodeIR();

      // Receive the next value
      _irrecv->resume();
    }
  }
}
