#include <Arduino.h>
#include <Wire.h>
#include "Buzzer.h"

// #define DEBUG

const int PIN_SECONDS_INDICATOR_LED = 2;

#ifdef DEBUG
const int INTERVAL_SECS = 60;     // 1 minute
const int MIN_DISTANCE_SECS = 15; // 15 seconds
#else
const int INTERVAL_SECS = 60 * 60;     // 1 hour
const int MIN_DISTANCE_SECS = 15 * 60; // 15 minutes
#endif

int secondsUntilChime = 0;
int intervalCounter = 0;
bool chimedInInterval = false;

Buzzer buzzer(
    21,   // PIN
    0,    // Channel
    1000, // PWM Frequency
    8     // PWM Resolution
);

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

void triggerChime()
{
  buzzer.playChime(NOTE_C4, 1500);
}

void setup()
{
  pinMode(PIN_SECONDS_INDICATOR_LED, OUTPUT);
  Serial.begin(115200);
  randomSeed(analogRead(A0));
  secondsUntilChime = constrainedWeightedRandom(INTERVAL_SECS, MIN_DISTANCE_SECS);

  printf("\n\nInterval........... %d seconds.\n", INTERVAL_SECS);
  printf("Min distance....... %d seconds.\n", MIN_DISTANCE_SECS);
  printf("Next chime in...... %d seconds.\n\n", secondsUntilChime);
}

void loop()
{
  digitalWrite(PIN_SECONDS_INDICATOR_LED, HIGH);
  delay(50);
  digitalWrite(PIN_SECONDS_INDICATOR_LED, LOW);
  delay(950);

  intervalCounter++;

  if (chimedInInterval)
  {
    printf("Counting to end of interval... %d seconds.\n",
           INTERVAL_SECS - intervalCounter);
  }
  else
  {
    printf("secondsUntilChime............. %d\n", secondsUntilChime);
  }
  printf("intervalCounter............... %d seconds\n\n", intervalCounter);

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