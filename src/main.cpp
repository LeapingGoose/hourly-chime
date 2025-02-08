#include <Arduino.h>
#include <Wire.h>
#include <pitches.h>

const int PIN_LED = 2;
const int PIN_BUZZ = 21;
const int INTERVAL_SECS = 60 * 60;     // 1 hour
const int MIN_DISTANCE_SECS = 15 * 60; // 15 minutes

int pauseDurationSecs = 0;
int currentSecs = 0;

// notes in the melody:
int melody[] = {
    NOTE_E5, NOTE_E5, NOTE_E5,
    NOTE_E5, NOTE_E5, NOTE_E5,
    NOTE_E5, NOTE_G5, NOTE_C5, NOTE_D5,
    NOTE_E5,
    NOTE_F5, NOTE_F5, NOTE_F5, NOTE_F5,
    NOTE_F5, NOTE_E5, NOTE_E5, NOTE_E5, NOTE_E5,
    NOTE_E5, NOTE_D5, NOTE_D5, NOTE_E5,
    NOTE_D5, NOTE_G5};

// note durations: 4 = quarter note, 8 = eighth note, etc, also called tempo:
int noteDurations[] = {
    8, 8, 4,
    8, 8, 4,
    8, 8, 8, 8,
    2,
    8, 8, 8, 8,
    8, 8, 8, 16, 16,
    8, 8, 8, 8,
    4, 4};

const int ledChannel = 0;
const int frequency = 2000; // Frequency in Hz
const int resolution = 8;   // Resolution of PWM (8 bits)

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
  digitalWrite(PIN_LED, HIGH);
  delay(500);
  digitalWrite(PIN_LED, LOW);
}

void playTone(int frequency, int duration)
{
  // Set the frequency for the tone
  ledcWriteTone(ledChannel, frequency);
  delay(duration);
  // Stop the tone
  ledcWriteTone(ledChannel, 0);
}

void setup()
{
  pinMode(PIN_LED, OUTPUT);
  // pinMode(PIN_BUZZ, OUTPUT);

  // Set up the PWM channel for the buzzer
  ledcSetup(ledChannel, frequency, resolution);
  ledcAttachPin(PIN_BUZZ, ledChannel);

  Serial.begin(115200);
  randomSeed(analogRead(A0));
  pauseDurationSecs = constrainedWeightedRandom(INTERVAL_SECS, MIN_DISTANCE_SECS);

  printf("Interval........... %d seconds.\n", INTERVAL_SECS);
  printf("Min Distance....... %d seconds.\n", MIN_DISTANCE_SECS);
  printf("Next flash in...... %d seconds.\n\n", pauseDurationSecs);
}

void buzzer()
{
  // iterate over the notes of the melody:
  int size = sizeof(noteDurations) / sizeof(int);

  for (int thisNote = 0; thisNote < size; thisNote++)
  {
    // to calculate the note duration, take one second divided by the note type.
    // e.g. quarter note = 1000 / 4, eighth note = 1000/8, etc.
    int noteDuration = 1000 / noteDurations[thisNote];
    playTone(melody[thisNote], noteDuration);

    // to distinguish the notes, set a minimum time between them.
    // the note's duration + 30% seems to work well:
    int pauseBetweenNotes = noteDuration * 1.30;
    delay(pauseBetweenNotes);
    // stop the tone playing:
    // noTone(PIN_BUZZ);
    // }
  }
}

void loop()
{
  printf("currentSecs........ %d\n", currentSecs);
  printf("pauseDurationSecs.. %d\n\n", pauseDurationSecs);

  buzzer();

  // Example melody
  // playTone(1000, 500); // Play tone at 1000 Hz for 500 ms
  // delay(500);          // Pause for 500 ms
  // playTone(1500, 500); // Play tone at 1500 Hz for 500 ms
  // delay(500);

  digitalWrite(PIN_LED, HIGH);
  delay(50);
  digitalWrite(PIN_LED, LOW);
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