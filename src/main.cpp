// TODO: Make it only chime once an hour. When chiming, figure out how much
// longer until the end of the hour and then start again from there.

#include <Arduino.h>
#include <Wire.h>
#include <pitches.h>

const int PIN_LED = 2;
const int PIN_BUZZ = 21;
const int INTERVAL_SECS = 60 * 60;     // 1 hour
const int MIN_DISTANCE_SECS = 15 * 60; // 15 minutes
// const int INTERVAL_SECS = 60;     // 1 minute
// const int MIN_DISTANCE_SECS = 15; // 15 seconds

int secondsUntilChime = 0;
int intervalCounter = 0;
bool chimedInInterval = false;

const int buzzerChannel = 0;
const int pwmFrequency = 1000; // Frequency in Hz
const int pwmResolution = 8;   // Resolution of PWM (8 bits)

int midWeightedRandom(int intervalSecs)
{
  int rand1 = random(intervalSecs);
  int rand2 = random(intervalSecs);
  return (rand1 + rand2) / 2;
}

int constrainedWeightedRandom(int intervalSecs, int minDistanceSecs)
{
  int weightedRandomSecs = midWeightedRandom(intervalSecs - minDistanceSecs);
  return weightedRandomSecs + minDistanceSecs;
}

// Play a single, fading 'ding' sound.
void playChime(int frequency, int duration)
{
  // Set the frequency for the tone
  ledcWriteTone(buzzerChannel, frequency);

  // Gradually reduce the volume to create a fade-out effect
  for (int dutyCycle = 150; dutyCycle >= 0; dutyCycle -= 5)
  {
    ledcWrite(buzzerChannel, dutyCycle);
    // Adjust delay to control fade-out speed
    delay(duration / 51);
  }

  // Stop the tone
  ledcWriteTone(buzzerChannel, 0);
}

void triggerChime()
{
  playChime(NOTE_C4, 1500);
}

void setup()
{
  pinMode(PIN_LED, OUTPUT);
  // Set up the PWM channel for the buzzer
  ledcSetup(buzzerChannel, pwmFrequency, pwmResolution);
  ledcAttachPin(PIN_BUZZ, buzzerChannel);

  Serial.begin(115200);
  randomSeed(analogRead(A0));
  secondsUntilChime = constrainedWeightedRandom(INTERVAL_SECS, MIN_DISTANCE_SECS);

  printf("\n\nInterval........... %d seconds.\n", INTERVAL_SECS);
  printf("Min distance....... %d seconds.\n", MIN_DISTANCE_SECS);
  printf("Next chime in...... %d seconds.\n\n", secondsUntilChime);
}

void loop()
{
  // Indicates seconds using onboard LED
  digitalWrite(PIN_LED, HIGH);
  delay(50);
  digitalWrite(PIN_LED, LOW);
  delay(950);

  intervalCounter++;

  if (chimedInInterval)
  {
    printf("Counting to end of interval... %d seconds.\n\n",
           INTERVAL_SECS - intervalCounter);
  }
  else
  {
    printf("secondsUntilChime............. %d\n\n", secondsUntilChime);
  }
  printf("intervalCounter............... %d seconds\n", intervalCounter);

  if (intervalCounter >= secondsUntilChime && chimedInInterval == false)
  {
    chimedInInterval = true;
    triggerChime();
    printf("Chimed at %d seconds.\n", intervalCounter);
    printf("End of interval in %d seconds.\n\n", INTERVAL_SECS - intervalCounter);
    secondsUntilChime = -1;
  }

  if (intervalCounter >= INTERVAL_SECS)
  {
    intervalCounter = 0;
    chimedInInterval = false;
    secondsUntilChime = constrainedWeightedRandom(INTERVAL_SECS, MIN_DISTANCE_SECS);
    printf("End of interval.\nNext chime in................. %d seconds.\n\n",
           secondsUntilChime);
  }
}