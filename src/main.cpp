#include <WiFi.h>
#include <WiFiManager.h>
#include <WiFiClientSecure.h>
#include <PubSubClient.h>
#include <DHT.h>
#include "secrets.h"
#include <esp_task_wdt.h>

const char* mqtt_server = "a8984e032441467797687728dde49d14.s1.eu.hivemq.cloud";
const int mqtt_port = 8883;

#define DHTPIN 4
#define DHTTYPE DHT22
DHT dht(DHTPIN, DHTTYPE);

#define PMS_RX 16
#define PMS_TX 17
HardwareSerial pmsSerial(2);

WiFiClientSecure espClient;
PubSubClient client(espClient);
WiFiManager wifiManager;

unsigned long lastMsg = 0;
const long interval = 5000;
unsigned long lastMqttAttempt = 0;
bool mqttAttempted = false;
unsigned long lastWifiAttempt = 0;
bool wifiAttempted = false;
const unsigned long reconnectInterval = 30000;

struct PMS7003Data {
  uint16_t pm1_0;
  uint16_t pm2_5;
  uint16_t pm10;
  bool valid;
};

PMS7003Data latestPms = {0, 0, 0, false};

void setup_wifi() {
  wifiManager.setConfigPortalTimeout(180);

  if (wifiManager.autoConnect("AirCarePlus-Setup", AP_PASSWORD)) {
    Serial.println("WiFi connected");
    Serial.println(WiFi.localIP());
  } else {
    Serial.println("WiFi setup timed out; retrying saved credentials");
    WiFi.mode(WIFI_STA);
    WiFi.begin();
    lastWifiAttempt = millis();
    wifiAttempted = true;
  }
}

PMS7003Data readPMS7003() {
  PMS7003Data data = {0, 0, 0, false};
  while (pmsSerial.available() >= 32) {
    if (pmsSerial.peek() != 0x42) {
      pmsSerial.read();
      continue;
    }

    pmsSerial.read();
    if (pmsSerial.peek() != 0x4D) continue;
    pmsSerial.read();

    byte buf[30];
    if (pmsSerial.readBytes(buf, sizeof(buf)) != sizeof(buf)) break;

    uint16_t frameLen = (buf[0] << 8) | buf[1];
    if (frameLen != 28) continue;

    uint16_t calcSum = 0x42 + 0x4D;
    for (int i = 0; i < 28; i++) calcSum += buf[i];
    uint16_t rxSum = (buf[28] << 8) | buf[29];
    if (calcSum != rxSum) continue;

    data.pm1_0 = (buf[8] << 8) | buf[9];
    data.pm2_5 = (buf[10] << 8) | buf[11];
    data.pm10  = (buf[12] << 8) | buf[13];
    data.valid = true;
  }

  return data;
}

void setup() {
  Serial.begin(115200);
  setup_wifi();
  esp_err_t wdtResult = esp_task_wdt_init(15, true);
  if (wdtResult != ESP_OK) {
    Serial.printf("Task watchdog init failed: %d\n", wdtResult);
  }
  wdtResult = esp_task_wdt_add(NULL);
  if (wdtResult != ESP_OK) {
    Serial.printf("Task watchdog subscribe failed: %d\n", wdtResult);
  }
  espClient.setCACert(HIVEMQ_CA_CERT);
  espClient.setTimeout(5);
  espClient.setHandshakeTimeout(5);
  client.setServer(mqtt_server, mqtt_port);
  client.setSocketTimeout(5);
  dht.begin();
  pmsSerial.begin(9600, SERIAL_8N1, PMS_RX, PMS_TX);
  Serial.println("AirCare+ Week 7 - AP Config Portal init");
}

void loop() {
  esp_task_wdt_reset();
  PMS7003Data newPms = readPMS7003();
  if (newPms.valid) latestPms = newPms;
  unsigned long now = millis();
  if (WiFi.status() == WL_CONNECTED) {
    if (client.connected()) {
      client.loop();
    } else if (!mqttAttempted || now - lastMqttAttempt >= reconnectInterval) {
      mqttAttempted = true;
      lastMqttAttempt = now;
      Serial.print("MQTT connecting...");
      String clientId = "ESP32Client-" + String(random(0xffff), HEX);
      if (client.connect(clientId.c_str(), MQTT_USER, MQTT_PASS)) {
        Serial.println("connected");
      } else {
        Serial.print("failed, rc=");
        Serial.println(client.state());
      }
    }
  } else {
    if (client.connected()) client.disconnect();
    if (!wifiAttempted || now - lastWifiAttempt >= reconnectInterval) {
      wifiAttempted = true;
      lastWifiAttempt = now;
      Serial.println("WiFi disconnected; retrying saved credentials");
      WiFi.mode(WIFI_STA);
      WiFi.begin();
    }
  }

  if (now - lastMsg > interval) {
    lastMsg = now;

    float t = dht.readTemperature();
    float h = dht.readHumidity();
    if (!isnan(t) && !isnan(h)) {
      client.publish("aircare/temperature", String(t, 1).c_str());
      client.publish("aircare/humidity", String(h, 1).c_str());
      Serial.printf("DHT -> Temp: %.1fC  Hum: %.1f%% | ", t, h);
    }

    if (latestPms.valid) {
      client.publish("aircare/pm1", String(latestPms.pm1_0).c_str());
      client.publish("aircare/pm25", String(latestPms.pm2_5).c_str());
      client.publish("aircare/pm10", String(latestPms.pm10).c_str());
      Serial.printf("PMS -> PM1.0:%d PM2.5:%d PM10:%d\n", latestPms.pm1_0, latestPms.pm2_5, latestPms.pm10);
    } else {
      Serial.println("PMS -> no valid frame");
    }
  }
}