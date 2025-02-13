#include <Arduino.h>
#include <Wire.h>
#include "Buzzer.h"

/*
Debug mode has an interval of 60 seconds and a minimum distance of 15 seconds.
Otherwise, the interval is 60 minutes and the minimum distance is 15 minutes.
*/
// #define DEBUG

const int PIN_SECS_IND_LED = 2;
const int BUZZER_DURATION = 1500;
const int BUZZER_NOTE = NOTE_C4;

#ifdef DEBUG
const int INTERVAL_SECS = 60;     // 1 minute
const int MIN_DISTANCE_SECS = 15; // 15 seconds
#else
const int INTERVAL_SECS = 60 * 60;     // 1 hour
const int MIN_DISTANCE_SECS = 15 * 60; // 15 minutes
#endif

int secsUntilChime = 0;
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
  buzzer.playChime(BUZZER_NOTE, BUZZER_DURATION);
}

void setup()
{
  pinMode(PIN_SECS_IND_LED, OUTPUT);
  randomSeed(analogRead(A0));
  secsUntilChime = constrainedWeightedRandom(INTERVAL_SECS, MIN_DISTANCE_SECS);

#ifdef DEBUG
  Serial.begin(115200);
  printf("\n\nInterval........... %d seconds.\n", INTERVAL_SECS);
  printf("Min distance....... %d seconds.\n", MIN_DISTANCE_SECS);
  printf("Next chime in...... %d seconds.\n\n", secsUntilChime);
#endif
}

void loop()
{
  digitalWrite(PIN_SECS_IND_LED, HIGH);
  delay(50);
  digitalWrite(PIN_SECS_IND_LED, LOW);
  delay(950);

  intervalCounter++;

#ifdef DEBUG
  printf("intervalCounter............... %d seconds\n", intervalCounter);
  if (chimedInInterval)
  {
    printf("Counting to end of interval... %d seconds.\n\n",
           INTERVAL_SECS - intervalCounter);
  }
  else
  {
    printf("secsUntilChime............. %d\n\n", secsUntilChime);
  }
#endif

  if (intervalCounter >= secsUntilChime && chimedInInterval == false)
  {
    chimedInInterval = true;
    triggerChime();
    secsUntilChime = -1;
#ifdef DEBUG
    printf("Chimed at %d seconds.\n", intervalCounter);
    printf("End of interval in %d seconds.\n\n", INTERVAL_SECS - intervalCounter);
#endif
  }

  if (intervalCounter >= INTERVAL_SECS)
  {
    intervalCounter = 0;
    chimedInInterval = false;
    secsUntilChime = constrainedWeightedRandom(INTERVAL_SECS, MIN_DISTANCE_SECS);
#ifdef DEBUG
    printf("End of interval.\nNext chime in................. %d seconds.\n\n",
           secsUntilChime);
#endif
  }
}