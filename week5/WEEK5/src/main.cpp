#include <Arduino.h>

// --- Pin Definitions ---
#define BUZZER_PIN  27
#define LED1_PIN    19
#define LED2_PIN    18

// --- PWM Channel Definitions ---
#define BUZZER_CH   0
#define LED1_CH     1
#define LED2_CH     2

// --- PWM Resolution ---
#define RESOLUTION  10  // 10-bit (0–1023)

// --- Frequencies ---
#define FREQ_BUZZER 10000   // 3 kHz tone
#define FREQ_LED1   2000   // LED1 PWM frequency
#define FREQ_LED2   800    // LED2 PWM frequency

void setup() {
  // --- Setup PWM channels ---
  ledcSetup(BUZZER_CH, FREQ_BUZZER, RESOLUTION);
  ledcAttachPin(BUZZER_PIN, BUZZER_CH);

  ledcSetup(LED1_CH, FREQ_LED1, RESOLUTION);
  ledcAttachPin(LED1_PIN, LED1_CH);

  ledcSetup(LED2_CH, FREQ_LED2, RESOLUTION);
  ledcAttachPin(LED2_PIN, LED2_CH);

  // --- Initial OFF State ---
  ledcWrite(BUZZER_CH, 0);
  ledcWrite(LED1_CH, 0);
  ledcWrite(LED2_CH, 0);
}

void loop() {
  // --- Step 1: Turn ON with different duty cycles ---
  ledcWrite(LED1_CH, 512);   // 50% brightness, 2 kHz
  ledcWrite(LED2_CH, 256);   // 25% brightness, 800 Hz
  ledcWriteTone(BUZZER_CH, 3000);  // 3 kHz tone
  delay(1000);

  // --- Step 2: Change buzzer frequency ---
  ledcWriteTone(BUZZER_CH, 3500);  // 3.5 kHz tone
  delay(1000);

  // --- Step 3: Blink LEDs visibly (for better effect) ---
  for (int i = 0; i < 5; i++) {
    ledcWrite(LED1_CH, 1023);  // Full brightness
    ledcWrite(LED2_CH, 512);
    delay(200);

    ledcWrite(LED1_CH, 0);
    ledcWrite(LED2_CH, 0);
    delay(200);
  }

  // --- Step 4: Turn everything OFF ---
  ledcWrite(BUZZER_CH, 0);
  ledcWrite(LED1_CH, 0);
  ledcWrite(LED2_CH, 0);
  delay(1000);
}
