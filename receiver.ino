#include <ArduinoMqttClient>

#if defined(ARDUINO_SAMD_MKRWIFI1010) || defined(ARDUINO_SAMD_NANO_33_IOT) || defined(ARDUINO_AVR_UNO_WIFI_REV2)
#include <WiFiNINA.h>
#elif defined(ARDUINO_SAMD_MKR1000)
#include <WiFi101.h>
#elif defined(ARDUINO_ARCH_ESP8266)
#include <ESP8266WiFi.h>
#elif defined(ARDUINO_ARCH_ESP32)
#include <WiFi.h>
#endif

char ssid[] = "Tanmaya's Iphone (2)";
char pass[] = "tanmayaspw";
int light = 13;

WiFiClient wifiClient;
MqttClient mqttClient(wifiClient);
const char broker[] = "mqtt-dashboard.com";
int port = 1883;
const char topic[] = "mywave";

void setup() {
  Serial.begin(9600);
  pinMode(light, OUTPUT);

  while (!Serial) {
    ;
  }

  Serial.print("Connecting to WPA SSID: ");
  Serial.println(ssid);

  while (WiFi.begin(ssid, pass) != WL_CONNECTED) {
    Serial.print(".");
    delay(5000);
  }

  Serial.println("Connected to the network");
  Serial.println();

  Serial.print("Connecting to the MQTT broker: ");
  Serial.println(broker);

  if (!mqttClient.connect(broker, port)) {
    Serial.print("MQTT connection failed! Error code = ");
    Serial.println(mqttClient.connectError());

    while (1);
  }

  Serial.println("Connected to the MQTT broker!");
  Serial.println();
  Serial.print("Subscribing to topic: ");
  Serial.println(topic);
  Serial.println();

  mqttClient.subscribe(topic);
}

void loop() {
  int messageSize = mqttClient.parseMessage();

  if (messageSize) {
    Serial.print("Received a message with topic '");
    Serial.print(mqttClient.messageTopic());
    Serial.print("', length ");
    Serial.print(messageSize);
    Serial.println(" bytes:");

    while (mqttClient.available()) {
      Serial.print((char)mqttClient.read());
    }

    Serial.println();

    digitalWrite(light, HIGH);
    delay(200);
    digitalWrite(light, LOW);
    delay(200);
    digitalWrite(light, HIGH);
    delay(200);
    digitalWrite(light, LOW);
    delay(200);
    digitalWrite(light, HIGH);
    delay(200);
    digitalWrite(light, LOW);
    delay(200);
    Serial.println();
  }
}
