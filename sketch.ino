#include <WiFi.h>
#include <PubSubClient.h>
#include <DHT.h>

#define DHTPIN 15
#define SOIL_PIN 34
#define GAS_PIN 35
#define VIB_PIN 32
#define RELAY_PIN 26
#define BUZZER_PIN 27
#define LED_PIN 2

DHT dht(DHTPIN, DHT22);
WiFiClient espClient;
PubSubClient mqtt(espClient);

const char* ssid = "Wokwi-GUEST";
const char* broker = "broker.hivemq.com";
const char* topic = "uppara_sreenath/iot/level3/data";

float vibAvg = 0;

void connectWiFi() {
  WiFi.begin(ssid, "", 6);
  while (WiFi.status() != WL_CONNECTED) { delay(300); Serial.print("."); }
  Serial.println("\nWiFi connected");
}

void connectMQTT() {
  while (!mqtt.connected()) {
    String id = "esp32-" + String(random(0xffff), HEX);
    if (mqtt.connect(id.c_str())) Serial.println("MQTT connected");
    else delay(1000);
  }
}

void setup() {
  Serial.begin(115200);
  pinMode(RELAY_PIN, OUTPUT);
  pinMode(BUZZER_PIN, OUTPUT);
  pinMode(LED_PIN, OUTPUT);
  dht.begin();
  connectWiFi();
  mqtt.setServer(broker, 1883);
}

void loop() {
  if (!mqtt.connected()) connectMQTT();
  mqtt.loop();

  float temp = dht.readTemperature();
  float hum  = dht.readHumidity();
  int soil   = map(analogRead(SOIL_PIN), 0, 4095, 0, 100);   // % moisture
  int gas    = map(analogRead(GAS_PIN), 0, 4095, 0, 100);
  int vib    = analogRead(VIB_PIN);

  // Automation: pump/fan ON if soil is dry or temperature is high
  bool relayOn = (soil < 30) || (temp > 35);
  digitalWrite(RELAY_PIN, relayOn);

  // Predictive maintenance: vibration far above its moving average
  if (vibAvg == 0) vibAvg = vib;
  vibAvg = 0.9 * vibAvg + 0.1 * vib;
  bool anomaly = vib > vibAvg * 1.5 && vib > 1000;

  // Alert
  bool alert = anomaly || gas > 70;
  digitalWrite(BUZZER_PIN, alert);
  digitalWrite(LED_PIN, !alert);

  char payload[200];
  snprintf(payload, sizeof(payload),
    "{\"temp\":%.1f,\"hum\":%.1f,\"soil\":%d,\"gas\":%d,\"vib\":%d,\"relay\":%d,\"alert\":%d}",
    temp, hum, soil, gas, vib, relayOn, alert);

  mqtt.publish(topic, payload);
  Serial.println(payload);
  delay(3000);
}
