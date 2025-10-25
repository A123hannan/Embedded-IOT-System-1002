#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <DHT.h>

// --- Pin configuration ---
#define LDR_PIN 34        // Analog pin for LDR
#define DHTPIN 14         // DHT22 data pin
#define DHTTYPE DHT22     // or DHT11 if using DHT11
#define SDA_PIN 21        // I2C SDA
#define SCL_PIN 22        // I2C SCL

// --- OLED setup ---
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

// --- DHT sensor setup ---
DHT dht(DHTPIN, DHTTYPE);

void setup() {
  Serial.begin(115200);
  Serial.println("Initializing ESP32 Sensors...");

  // Initialize I2C on custom pins
  Wire.begin(SDA_PIN, SCL_PIN);

  // Initialize OLED
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println("❌ SSD1306 allocation failed");
    for (;;); // Stop if OLED fails
  }

  display.clearDisplay();
  display.setTextColor(SSD1306_WHITE);
  display.setTextSize(1);
  display.setCursor(0, 0);
  display.println("Initializing...");
  display.display();

  // Initialize DHT sensor
  dht.begin();
  delay(1000);

  Serial.println("✅ Setup Complete!");
}

void loop() {
  // --- LDR Reading ---
  int adcValue = analogRead(LDR_PIN);
  float voltage = (adcValue / 4095.0) * 3.3;

  // --- DHT Reading ---
  float temperature = dht.readTemperature();
  float humidity = dht.readHumidity();

  // --- Check sensor validity ---
  if (isnan(temperature) || isnan(humidity)) {
    Serial.println("⚠️ Error reading DHT22 sensor!");
    return;
  }

  // --- Serial Monitor Output ---
  Serial.println("==============================");
  Serial.printf("LDR ADC: %d | Voltage: %.2f V\n", adcValue, voltage);
  Serial.printf("Temp: %.2f °C | Humidity: %.2f %%\n", temperature, humidity);

  // --- OLED Display Output ---
  display.clearDisplay();
  display.setTextSize(1);
  display.setCursor(0, 0);
  display.println("🌡️ ESP32 Sensor Monitor");
  
  display.setCursor(0, 16);
  display.setTextSize(1);
  display.printf("LDR: %d  (%.2fV)", adcValue, voltage);

  display.setCursor(0, 28);
  display.setTextSize(1);
  display.print("Temp: ");
  display.setTextSize(2);
  display.print(temperature);
  display.setTextSize(1);
  display.println(" C");

  display.setCursor(0, 48);
  display.setTextSize(1);
  display.print("Humidity: ");
  display.setTextSize(2);
  display.print(humidity);
  display.setTextSize(1);
  display.println(" %");

  display.display();

  delay(2000); // Update every 2 seconds
}
