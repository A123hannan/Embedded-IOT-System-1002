#include <Arduino.h>
#define BLYNK_PRINT Serial

/* ================= BLYNK ================= */
#define BLYNK_TEMPLATE_ID "TMPL6ZbL7avkz"
#define BLYNK_TEMPLATE_NAME "SAARS AIR MONITOR"
#define BLYNK_AUTH_TOKEN "LzQlnkLzisha19HtJVEAK7Iefj_sp7z3"

/* ================= LIBRARIES ================= */
#include <WiFi.h>
#include <BlynkSimpleEsp32.h>
#include <DHT.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Firebase_ESP_Client.h>
#include <addons/TokenHelper.h>
#include <time.h>
#include <math.h>

/* ================= WIFI & FIREBASE ================= */
char ssid[] = "NTU-FSD";
char pass[] = "";

#define DATABASE_URL "https://saars-air-monitor-default-rtdb.firebaseio.com/"
#define DATABASE_SECRET "Rp7Y1A3WZhF2DG7lBgJIor3WVeOa0JnUtpB9kUew"

/* ================= PINS ================= */
#define DHTPIN        4
#define DHTTYPE       DHT11
#define DUST_LED_PIN  25
#define DUST_VO_PIN   35
#define MQ2_PIN       34
#define RELAY_PIN     26
#define BUZZER_PIN    5
#define GAS_LED_PIN   2

/* ================= OBJECTS ================= */
DHT dht(DHTPIN, DHTTYPE);
Adafruit_SSD1306 display(128, 64, &Wire, -1);

FirebaseData fbdo;
FirebaseAuth auth;
FirebaseConfig config;
BlynkTimer timer;

/* ================= FAN & MODE ==========vvvvvvvvvvvvvvvvvvvvvvvvvv                                                                                          ======= */
bool fanState = false;
int fanMode = 0; // 0 = AUTO, 1 = FORCE ON, 2 = FORCE OFF

/* ================= THRESHOLDS (INCREASED) ================= */
const float dustOnThreshold  = 600.0;  // was 150
const float dustOffThreshold = 150.0;  // was 100
const float dustDanger       = 300.0;  // was 250

// Calibrated MQ2 thresholds in ADC (0-4095)
const int gasThreshold       = 2300;  
const int gasDanger          = 2800;  

/* ================= SENSOR READINGS ================= */
float readDust() {
  float sum = 0;
  for (int i = 0; i < 5; i++) {
    digitalWrite(DUST_LED_PIN, LOW);
    delayMicroseconds(280);
    sum += analogRead(DUST_VO_PIN);
    delayMicroseconds(40);
    digitalWrite(DUST_LED_PIN, HIGH);
    delay(10);
  }
  float avg = sum / 5.0;
  float voltage = avg * (3.3 / 4095.0);
  float dust = (voltage - 0.6) * 1000.0 / 0.5;
  if (dust < 0) dust = 0;
  return dust;
}

int readGasRaw() {
  long sum = 0;
  for (int i = 0; i < 10; i++) {
    sum += analogRead(MQ2_PIN);
    delay(10);
  }
  return sum / 10;
}

/* ================= BLYNK CONTROL ================= */
BLYNK_WRITE(V5) {
  fanMode = param.asInt();
}

/* ================= MAIN UPDATE ================= */
void updateData() {
  float temperature = dht.readTemperature();
  float humidity    = dht.readHumidity();
  float dust        = readDust();
  int gasRaw        = readGasRaw();

  if (isnan(temperature) || isnan(humidity)) return;

  // ===== FAN CONTROL =====
  if (fanMode == 0) {
    if (!fanState) {
      if (dust > dustOnThreshold || gasRaw > gasThreshold)
        fanState = true;
    } else {
      if (dust < dustOffThreshold && gasRaw < gasThreshold)
        fanState = false;
    }
  } else if (fanMode == 1) fanState = true;
  else if (fanMode == 2) fanState = false;

  digitalWrite(RELAY_PIN, fanState ? HIGH : LOW);

  // ===== ALERTS =====
  bool dustAlert = dust > dustDanger;
  bool gasAlert  = gasRaw > gasDanger;

  digitalWrite(BUZZER_PIN, (dustAlert || gasAlert) ? HIGH : LOW);
  digitalWrite(GAS_LED_PIN, gasAlert ? HIGH : LOW);

  // ===== OLED DISPLAY =====
  display.clearDisplay();
  display.setTextSize(1);
  display.setCursor(0,0);
  display.setTextColor(WHITE);

  display.printf(
    "Temp: %.1f C\n"
    "Hum : %.1f %%\n"
    "PM2.5: %.1f\n"
    "Gas : %d\n"
    "Fan : %s",
    temperature, humidity, dust, gasRaw,
    fanState ? "ON" : "OFF"
  );
  display.display();

  // ===== BLYNK =====
  Blynk.virtualWrite(V0, temperature);
  Blynk.virtualWrite(V1, humidity);
  Blynk.virtualWrite(V2, gasRaw);
  Blynk.virtualWrite(V3, dust);
  Blynk.virtualWrite(V4, fanState);

  // ===== FIREBASE =====
  FirebaseJson current;
  current.set("temperature", temperature);
  current.set("humidity", humidity);
  current.set("pm25", dust);
  current.set("gas_raw", gasRaw);
  current.set("fan", fanState);
  current.set("timestamp", millis());

  Firebase.RTDB.setJSON(&fbdo, "/SAARS/current", &current);
  Firebase.RTDB.pushJSON(&fbdo, "/SAARS/history", &current);

  // ===== SERIAL OUTPUT =====
  Serial.printf(
    "T:%.1f H:%.1f PM2.5:%.1f Gas:%d Fan:%d Mode:%d\n",
    temperature, humidity, dust, gasRaw, fanState, fanMode
  );
}

/* ================= SETUP ================= */
void setup() {
  Serial.begin(115200);

  pinMode(DUST_LED_PIN, OUTPUT);
  pinMode(RELAY_PIN, OUTPUT);
  pinMode(BUZZER_PIN, OUTPUT);
  pinMode(GAS_LED_PIN, OUTPUT);
  digitalWrite(RELAY_PIN, LOW);
  digitalWrite(BUZZER_PIN, LOW);
  digitalWrite(GAS_LED_PIN, LOW);

  dht.begin();

  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  display.clearDisplay();
  display.display();

  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);

  config.database_url = DATABASE_URL;
  config.signer.tokens.legacy_token = DATABASE_SECRET;
  Firebase.begin(&config, &auth);
  Firebase.reconnectWiFi(true);

  configTime(0, 0, "pool.ntp.org");

  timer.setInterval(3000L, updateData);
}

/* ================= LOOP ================= */
void loop() {
  Blynk.run();
  timer.run();
}
