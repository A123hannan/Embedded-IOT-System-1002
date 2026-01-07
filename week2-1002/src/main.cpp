#include <Arduino.h>

#define LED PIN 2 // change if your board uses another built-in LED pin

void setup() {

pinMode(LED_PIN, OUTPUT);

}

void loop() {

digitalWrite(LED_PIN, HIGH);

// LED on

delay(500);

// wait 0.5s

digitalWrite(LED_PIN, LOW);

// LED off

delay(500);

}