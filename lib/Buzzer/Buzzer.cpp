#include "Buzzer.h"

Buzzer::Buzzer(int pin, int channel, int frequency, int resolution)
    : pin(pin), channel(channel), frequency(frequency), resolution(resolution)
{
  ledcSetup(channel, frequency, resolution);
  ledcAttachPin(pin, channel);
}

void Buzzer::playChime(int frequency, int duration)
{
  ledcWriteTone(channel, frequency);

  for (int dutyCycle = 150; dutyCycle >= 0; dutyCycle -= 5)
  {
    ledcWrite(channel, dutyCycle);
    delay(duration / 51);
  }

  ledcWriteTone(channel, 0);
}