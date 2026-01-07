#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

<<<<<<< HEAD
// --- Pin Definitions ---
#define BTN_PIN   25
#define LED_PIN   18
#define BUZZER    19

// --- LED PWM Setup (ESP32) ---
#define CH_LED    0
#define PWM_FREQ  5000
#define PWM_RES   8   // 8-bit (0–255)

// --- Debounce and Press Detection ---
bool lastButtonState = HIGH;
bool buttonPressed = false;
unsigned long pressStartTime = 0;
unsigned long lastDebounceTime = 0;
const unsigned long DEBOUNCE_MS = 30;
const unsigned long LONG_PRESS_MS = 1500;

// --- LED & Buzzer states ---
bool ledState = false;
bool buzzerOn = false;
unsigned long buzzerStartTime = 0;
const unsigned long BUZZER_DURATION = 200;

void setup() {
  pinMode(BTN_PIN, INPUT_PULLUP);
  pinMode(BUZZER, OUTPUT);
  digitalWrite(BUZZER, LOW);

  // Setup LED PWM
  ledcSetup(CH_LED, PWM_FREQ, PWM_RES);
  ledcAttachPin(LED_PIN, CH_LED);
  ledcWrite(CH_LED, 0);

  // Setup OLED
=======
// LED pins
#define LED_YELLOW 18
#define LED_GREEN  17
#define LED_RED    16

// LED PWM channels (for ESP32)
#define CH_YELLOW 0
#define CH_GREEN  1
#define CH_RED    2
#define PWM_FREQ  5000
#define PWM_RES   8   // 8-bit resolution (0-255)

// Button pins
#define BTN_CYCLE 25
#define BTN_RESET 26



// Button debounce
int stableButtonState = HIGH;
int lastReading = HIGH;
int lastStableState = HIGH;
unsigned long lastDebounceTime = 0;
const unsigned long DEBOUNCE_MS = 30;

int stableResetState = HIGH;
int lastResetReading = HIGH;
int lastStableResetState = HIGH;
unsigned long lastResetDebounceTime = 0;

// LED state: 0=OFF, 1=ALL ON, 2=BLINK, 3=PWM FADE
int ledMode = 0;

// Blink timing
unsigned long lastBlinkTime = 0;
const unsigned long BLINK_INTERVAL = 500;
bool blinkState = false;

// PWM fade
int fadeValue = 0;
int fadeDirection = 1;
unsigned long lastFadeTime = 0;
const unsigned long FADE_INTERVAL = 10;

void setup() {
  pinMode(BTN_CYCLE, INPUT_PULLUP);
  pinMode(BTN_RESET, INPUT_PULLUP);

  // Initialize OLED
>>>>>>> 4c4558b (Added all remaining Embedded IoT folders)
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    for (;;);
  }
  display.clearDisplay();
  display.setTextSize(2);
  display.setTextColor(SSD1306_WHITE);
<<<<<<< HEAD
  display.setCursor(0, 20);
  display.println("Ready");
  display.display();
=======

  // Initialize button states
  lastReading = digitalRead(BTN_CYCLE);
  stableButtonState = lastReading;
  lastStableState = stableButtonState;

  lastResetReading = digitalRead(BTN_RESET);
  stableResetState = lastResetReading;
  lastStableResetState = stableResetState;

  // Attach PWM channels
  ledcSetup(CH_YELLOW, PWM_FREQ, PWM_RES);
  ledcAttachPin(LED_YELLOW, CH_YELLOW);

  ledcSetup(CH_GREEN, PWM_FREQ, PWM_RES);
  ledcAttachPin(LED_GREEN, CH_GREEN);

  ledcSetup(CH_RED, PWM_FREQ, PWM_RES);
  ledcAttachPin(LED_RED, CH_RED);

  // Initial LEDs off
  ledcWrite(CH_YELLOW, 0);
  ledcWrite(CH_GREEN, 0);
  ledcWrite(CH_RED, 0);
>>>>>>> 4c4558b (Added all remaining Embedded IoT folders)
}

void loop() {
  unsigned long currentMillis = millis();
<<<<<<< HEAD
  bool reading = digitalRead(BTN_PIN);

  // Debounce button
  if (reading != lastButtonState) {
    lastDebounceTime = currentMillis;
    lastButtonState = reading;
  }

  if ((currentMillis - lastDebounceTime) > DEBOUNCE_MS) {
    // Button pressed
    if (!buttonPressed && reading == LOW) {
      buttonPressed = true;
      pressStartTime = currentMillis;
    }

    // Button released
    if (buttonPressed && reading == HIGH) {
      unsigned long pressDuration = currentMillis - pressStartTime;
      buttonPressed = false;

      if (pressDuration >= LONG_PRESS_MS) {
        // --- Long Press ---
        digitalWrite(BUZZER, HIGH);
        buzzerOn = true;
        buzzerStartTime = currentMillis;

        display.clearDisplay();
        display.setCursor(0, 20);
        display.println("Long Press");
        display.display();
      } else {
        // --- Short Press ---
        ledState = !ledState;
        ledcWrite(CH_LED, ledState ? 255 : 0);

        display.clearDisplay();
        display.setCursor(0, 20);
        display.println(ledState ? "LED ON" : "LED OFF");
        display.display();
=======

  // -------- Handle BTN_CYCLE --------
  int reading = digitalRead(BTN_CYCLE);
  if (reading != lastReading) {
    lastDebounceTime = currentMillis;
    lastReading = reading;
  }
  if (currentMillis - lastDebounceTime >= DEBOUNCE_MS) {
    if (stableButtonState != reading) {
      stableButtonState = reading;
      if (lastStableState == HIGH && stableButtonState == LOW) {
        ledMode++;
        if (ledMode > 3) ledMode = 0;

        fadeValue = 0;
        fadeDirection = 1;
        blinkState = false;
        lastBlinkTime = currentMillis;
        lastFadeTime = currentMillis;
>>>>>>> 4c4558b (Added all remaining Embedded IoT folders)
      }
      lastStableState = stableButtonState;
    }
  }

<<<<<<< HEAD
  // Turn off buzzer after duration
  if (buzzerOn && (currentMillis - buzzerStartTime >= BUZZER_DURATION)) {
    digitalWrite(BUZZER, LOW);
    buzzerOn = false;
  }
}
=======
  // -------- Handle BTN_RESET --------
  int resetReading = digitalRead(BTN_RESET);
  if (resetReading != lastResetReading) {
    lastResetDebounceTime = currentMillis;
    lastResetReading = resetReading;
  }
  if (currentMillis - lastResetDebounceTime >= DEBOUNCE_MS) {
    if (stableResetState != resetReading) {
      stableResetState = resetReading;
      if (lastStableResetState == HIGH && stableResetState == LOW) {
        ledMode = 0;

        // Turn off all LEDs
        ledcWrite(CH_YELLOW, 0);
        ledcWrite(CH_GREEN, 0);
        ledcWrite(CH_RED, 0);

        fadeValue = 0;
        fadeDirection = 1;
        blinkState = false;
        lastBlinkTime = currentMillis;
        lastFadeTime = currentMillis;
      }
      lastStableResetState = stableResetState;
    }
  }

  // -------- Handle LED modes --------
  switch (ledMode) {
    case 0: // ALL OFF
      ledcWrite(CH_YELLOW, 0);
      ledcWrite(CH_GREEN, 0);
      ledcWrite(CH_RED, 0);
      break;
    case 1: // ALL ON
      ledcWrite(CH_YELLOW, 255);
      ledcWrite(CH_GREEN, 255);
      ledcWrite(CH_RED, 255);
      break;
    case 2: // BLINK
      if (currentMillis - lastBlinkTime >= BLINK_INTERVAL) {
        blinkState = !blinkState;
        lastBlinkTime = currentMillis;
      }
      ledcWrite(CH_YELLOW, blinkState ? 255 : 0);
      ledcWrite(CH_GREEN, blinkState ? 0 : 255);
      ledcWrite(CH_RED, blinkState ? 255 : 0);
      break;
    case 3: // PWM FADE
      if (currentMillis - lastFadeTime >= FADE_INTERVAL) {
        fadeValue += fadeDirection * 5;
        if (fadeValue >= 255) { fadeValue = 255; fadeDirection = -1; }
        if (fadeValue <= 0)   { fadeValue = 0; fadeDirection = 1; }
        ledcWrite(CH_YELLOW, fadeValue);
        ledcWrite(CH_GREEN, fadeValue);
        ledcWrite(CH_RED, fadeValue);
        lastFadeTime = currentMillis;
      }
      break;
  }

  // -------- Update OLED --------
  display.clearDisplay();
  display.setCursor(0, 20);
  switch (ledMode) {
    case 0: display.println("ALL OFF"); break;
    case 1: display.println("ALL ON"); break;
    case 2: display.println("BLINKING"); break;
    case 3: display.println("PWM FADE"); break;
  }
  display.display();
}
>>>>>>> 4c4558b (Added all remaining Embedded IoT folders)
