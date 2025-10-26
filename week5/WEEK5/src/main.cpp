#include <Arduino.h>

#define BUZZER_PIN  27     // GPIO connected to buzzer
#define LED1_PIN    19     // First LED
#define LED2_PIN    18     // Second LED
#define PWM_CH      0      // PWM channel (0–15)
#define FREQ        2000   // Default frequency (Hz)
#define RESOLUTION  10     // 10-bit resolution (0–1023)

void setup() {
  // --- Setup pins ---
  pinMode(LED1_PIN, OUTPUT);
  pinMode(LED2_PIN, OUTPUT);

  // --- Setup PWM channel for buzzer ---
  ledcSetup(PWM_CH, FREQ, RESOLUTION);
  ledcAttachPin(BUZZER_PIN, PWM_CH);

  // Initial OFF state
  digitalWrite(LED1_PIN, LOW);
  digitalWrite(LED2_PIN, LOW);
  ledcWrite(PWM_CH, 0);

  // --- Simple beep pattern ---
  for (int i = 0; i < 3; i++) {
    digitalWrite(LED1_PIN, HIGH);
    digitalWrite(LED2_PIN, HIGH);
    ledcWriteTone(PWM_CH, 2000 + i * 400);
    delay(150);

    digitalWrite(LED1_PIN, LOW);
    digitalWrite(LED2_PIN, LOW);
    ledcWrite(PWM_CH, 0);
    delay(150);
  }

  // --- Frequency sweep (400Hz → 3kHz) ---
  for (int f = 400; f <= 3000; f += 100) {
    digitalWrite(LED1_PIN, f % 200 == 0 ? HIGH : LOW);  // blink during sweep
    digitalWrite(LED2_PIN, f % 300 == 0 ? HIGH : LOW);
    ledcWriteTone(PWM_CH, f);
    delay(20);
  }
  ledcWrite(PWM_CH, 0);
  digitalWrite(LED1_PIN, LOW);
  digitalWrite(LED2_PIN, LOW);
  delay(500);

  // --- Short melody ---
  int melody[] = {262, 294, 330, 349, 392, 440, 494, 523};
  for (int i = 0; i < 8; i++) {
    digitalWrite(LED1_PIN, i % 2 == 0 ? HIGH : LOW);
    digitalWrite(LED2_PIN, i % 2 != 0 ? HIGH : LOW);
    ledcWriteTone(PWM_CH, melody[i]);
    delay(250);
  }

  // Stop everything
  ledcWrite(PWM_CH, 0);
  digitalWrite(LED1_PIN, LOW);
  digitalWrite(LED2_PIN, LOW);
}

void loop() {
  // Repeat the same sequence in loop

  // --- Simple beep pattern ---
  for (int i = 0; i < 3; i++) {
    digitalWrite(LED1_PIN, HIGH);
    digitalWrite(LED2_PIN, HIGH);
    ledcWriteTone(PWM_CH, 2000 + i * 400);
    delay(150);

    digitalWrite(LED1_PIN, LOW);
    digitalWrite(LED2_PIN, LOW);
    ledcWrite(PWM_CH, 0);
    delay(150);
  }

  // --- Frequency sweep ---
  for (int f = 400; f <= 3000; f += 100) {
    digitalWrite(LED1_PIN, f % 200 == 0 ? HIGH : LOW);
    digitalWrite(LED2_PIN, f % 300 == 0 ? HIGH : LOW);
    ledcWriteTone(PWM_CH, f);
    delay(20);
  }
  ledcWrite(PWM_CH, 0);
  digitalWrite(LED1_PIN, LOW);
  digitalWrite(LED2_PIN, LOW);
  delay(500);

  // --- Short melody ---
  int melody[] = {262, 294, 330, 349, 392, 440, 494, 523};
  for (int i = 0; i < 8; i++) {
    digitalWrite(LED1_PIN, i % 2 == 0 ? HIGH : LOW);
    digitalWrite(LED2_PIN, i % 2 != 0 ? HIGH : LOW);
    ledcWriteTone(PWM_CH, melody[i]);
    delay(250);
  }

  // Stop
  ledcWrite(PWM_CH, 0);
  digitalWrite(LED1_PIN, LOW);
  digitalWrite(LED2_PIN, LOW);
  delay(1000);
}
