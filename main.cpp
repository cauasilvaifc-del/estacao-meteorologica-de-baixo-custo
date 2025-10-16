
// Estação Meteorológica 3D - IFC Camboriú
// Exemplo básico de envio de dados via Wi-Fi para ThingsBoard

#include <WiFi.h>
#include <HTTPClient.h>
#include <Adafruit_BME280.h>

#define SEALEVELPRESSURE_HPA (1013.25)

Adafruit_BME280 bme;
const char* ssid = "REDE_WIFI";
const char* password = "SENHA_WIFI";
const char* server = "http://thingsboard.server/api/v1/TOKEN/telemetry";

void setup() {
  Serial.begin(115200);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nConectado ao Wi-Fi!");
  if (!bme.begin(0x76)) {
    Serial.println("Erro ao iniciar BME280!");
    while (1);
  }
}

void loop() {
  float temp = bme.readTemperature();
  float hum = bme.readHumidity();
  float pres = bme.readPressure() / 100.0F;

  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;
    http.begin(server);
    http.addHeader("Content-Type", "application/json");
    String jsonData = "{"temperature":" + String(temp) + ","humidity":" + String(hum) + ","pressure":" + String(pres) + "}";
    int httpResponseCode = http.POST(jsonData);
    Serial.println("Dados enviados: " + jsonData);
    http.end();
  }

  delay(60000); // envia a cada 60s
}
