#include <WiFi.h>

const char* ssid = "massad";
const char* password = "mshmr054";

// Static IP configuration
IPAddress local_IP(192, 168, 43, 50);     // ESP32 fixed IP
IPAddress gateway(192, 168, 43, 1);       // Always 43.1 on hotspot
IPAddress subnet(255, 255, 255, 0);
IPAddress primaryDNS(8, 8, 8, 8);
IPAddress secondaryDNS(8, 8, 4, 4);
void setup() {
  Serial.begin(9600);

  // Set Static IP
  if (!WiFi.config(local_IP, gateway, subnet, primaryDNS, secondaryDNS)) {
    Serial.println("Static IP Failed!");
  }

  WiFi.begin(ssid, password);

  Serial.print("Connecting");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("\nConnected!");
  Serial.print("ESP32 IP: ");
  Serial.println(WiFi.localIP());
}

void loop() {
}
