#include <Arduino.h>
#include <Wire.h>

const int LED_PIN = 2;
// const int INTERVAL_SECS = 60;
const int INTERVAL_SECS = 60 * 60; // 1 hour
// const int MIN_DISTANCE_SECS = 15;
const int MIN_DISTANCE_SECS = 15 * 60; // 15 minutes

int pauseDurationSecs = 0;
int currentSecs = 0;

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
  Serial.println("Flashing LED.\n");
  digitalWrite(LED_PIN, HIGH);
  delay(500);
  digitalWrite(LED_PIN, LOW);
}

void setup()
{
  pinMode(LED_PIN, OUTPUT);
  Serial.begin(115200);
  randomSeed(analogRead(A0));
  pauseDurationSecs = constrainedWeightedRandom(INTERVAL_SECS, MIN_DISTANCE_SECS);

  printf("Interval........... %d seconds.\n", INTERVAL_SECS);
  printf("Min Distance....... %d seconds.\n", MIN_DISTANCE_SECS);
  printf("Next flash in...... %d seconds.\n\n", pauseDurationSecs);
}

void loop()
{
  printf("currentSecs........ %d\n", currentSecs);
  printf("pauseDurationSecs.. %d\n\n", pauseDurationSecs);

  digitalWrite(LED_PIN, HIGH);
  delay(50);
  digitalWrite(LED_PIN, LOW);
  delay(950);

  currentSecs++;

  if (currentSecs >= pauseDurationSecs)
  {
    currentSecs = 0;
    pauseDurationSecs = constrainedWeightedRandom(INTERVAL_SECS, MIN_DISTANCE_SECS);
    printf("Next flash in...... %d seconds.\n", pauseDurationSecs);
    triggerChime();
  }
}