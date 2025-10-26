#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET    -1
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// LEDs
#define LED_YELLOW 18
#define LED_GREEN 17
#define LED_RED 16

// Buttons
#define BTN_CYCLE 25
#define BTN_RESET 26

// PWM fade
int fadeValue = 0;
int fadeDirection = 1;

// Button debounce
const unsigned long DEBOUNCE_MS = 30;
int lastReadingCycle = HIGH, stableCycle = HIGH, lastStableCycle = HIGH;
unsigned long lastDebounceCycle = 0;

int lastReadingReset = HIGH, stableReset = HIGH, lastStableReset = HIGH;
unsigned long lastDebounceReset = 0;

// LED mode: 0=OFF, 1=ALT BLINK, 2=ALL ON, 3=PWM FADE
int ledMode = 0;
bool blinkState = false;
unsigned long lastBlinkTime = 0;
const unsigned long BLINK_INTERVAL = 500;
const unsigned long FADE_INTERVAL = 10;
unsigned long lastFadeTime = 0;

void setup() {
  pinMode(BTN_CYCLE, INPUT_PULLUP);
  pinMode(BTN_RESET, INPUT_PULLUP);

  pinMode(LED_YELLOW, OUTPUT);
  pinMode(LED_GREEN, OUTPUT);
  pinMode(LED_RED, OUTPUT);

  // OLED init
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { for(;;); }
  display.clearDisplay();
  display.setTextSize(2);
  display.setTextColor(SSD1306_WHITE);

  // Initial LED off
  analogWrite(LED_YELLOW, 0);
  analogWrite(LED_GREEN, 0);
  analogWrite(LED_RED, 0);
}

void loop() {
  unsigned long currentMillis = millis();

  // -------- Handle Cycle Button --------
  int readingCycle = digitalRead(BTN_CYCLE);
  if (readingCycle != lastReadingCycle) lastDebounceCycle = currentMillis;
  if (currentMillis - lastDebounceCycle >= DEBOUNCE_MS) {
    if (stableCycle != readingCycle && lastStableCycle == HIGH && readingCycle == LOW) {
      ledMode++;
      if (ledMode > 3) ledMode = 0;
      fadeValue = 0; fadeDirection = 1; blinkState = false;
      lastBlinkTime = currentMillis; lastFadeTime = currentMillis;
    }
    stableCycle = readingCycle;
  }
  lastReadingCycle = readingCycle;
  lastStableCycle = stableCycle;

  // -------- Handle Reset Button --------
  int readingReset = digitalRead(BTN_RESET);
  if (readingReset != lastReadingReset) lastDebounceReset = currentMillis;
  if (currentMillis - lastDebounceReset >= DEBOUNCE_MS) {
    if (stableReset != readingReset && lastStableReset == HIGH && readingReset == LOW) {
      ledMode = 0;
      analogWrite(LED_YELLOW, 0);
      analogWrite(LED_GREEN, 0);
      analogWrite(LED_RED, 0);
      fadeValue = 0; fadeDirection = 1; blinkState = false;
      lastBlinkTime = currentMillis; lastFadeTime = currentMillis;
    }
    stableReset = readingReset;
  }
  lastReadingReset = readingReset;
  lastStableReset = stableReset;

  // -------- LED modes --------
  switch(ledMode){
    case 0: // All OFF
      analogWrite(LED_YELLOW, 0);
      analogWrite(LED_GREEN, 0);
      analogWrite(LED_RED, 0);
      break;
    case 1: // Alternate blink
      if (currentMillis - lastBlinkTime >= BLINK_INTERVAL) {
        blinkState = !blinkState;
        lastBlinkTime = currentMillis;
      }
      analogWrite(LED_YELLOW, blinkState ? 255 : 0);
      analogWrite(LED_GREEN, blinkState ? 0 : 255);
      analogWrite(LED_RED, blinkState ? 255 : 0);
      break;
    case 2: // All ON
      analogWrite(LED_YELLOW, 255);
      analogWrite(LED_GREEN, 255);
      analogWrite(LED_RED, 255);
      break;
    case 3: // PWM fade
      if (currentMillis - lastFadeTime >= FADE_INTERVAL) {
        fadeValue += fadeDirection * 5;
        if (fadeValue >= 255) { fadeValue = 255; fadeDirection = -1; }
        if (fadeValue <= 0)   { fadeValue = 0; fadeDirection = 1; }
        analogWrite(LED_YELLOW, fadeValue);
        analogWrite(LED_GREEN, fadeValue);
        analogWrite(LED_RED, fadeValue);
        lastFadeTime = currentMillis;
      }
      break;
  }

  // -------- OLED display --------
  display.clearDisplay();
  display.setCursor(0,20);
  switch(ledMode){
    case 0: display.println("ALL OFF"); break;
    case 1: display.println("BLINK"); break;
    case 2: display.println("ALL ON"); break;
    case 3: display.println("PWM FADE"); break;
  }
  display.display();
}
