#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET    -1
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

#define BUTTON_PIN 25
#define LED_PIN    18
#define BUZZER_PIN 15

const unsigned long LONG_PRESS_MS = 1500;
const unsigned long DEBOUNCE_MS = 50;

int lastButtonState = HIGH;
int stableState = HIGH;
unsigned long lastDebounce = 0;
unsigned long pressStart = 0;

void setup() {
  pinMode(BUTTON_PIN, INPUT_PULLUP);
  pinMode(LED_PIN, OUTPUT);
  pinMode(BUZZER_PIN, OUTPUT);

  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) for(;;);
  display.clearDisplay();
  display.setTextSize(2);
  display.setTextColor(SSD1306_WHITE);

  digitalWrite(LED_PIN, LOW);
  digitalWrite(BUZZER_PIN, LOW);
}

void loop() {
  unsigned long currentMillis = millis();
  int reading = digitalRead(BUTTON_PIN);

  // Debounce
  if (reading != lastButtonState) lastDebounce = currentMillis;
  if (currentMillis - lastDebounce >= DEBOUNCE_MS) {
    if (stableState != reading) {
      stableState = reading;

      if (reading == LOW) pressStart = currentMillis;  // button pressed
      else {  // button released
        unsigned long pressDuration = currentMillis - pressStart;
        display.clearDisplay();
        display.setCursor(0,20);

        if (pressDuration < LONG_PRESS_MS) {
          // Short press → toggle LED
          digitalWrite(LED_PIN, !digitalRead(LED_PIN));
          display.println("Short Press");
        } else {
          // Long press → buzzer
          tone(BUZZER_PIN, 1000, 500);
          display.println("Long Press");
        }
        display.display();
      }
    }
  }

  lastButtonState = reading;
}
