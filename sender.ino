#include <ArduinoMqttClient.h>

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

// Define pins for the ultrasonic sensor
const int trigPin = 2;
const int echoPin = 3;

float duration, distance;

WiFiClient wifiClient;
MqttClient mqttClient(wifiClient); 

const char broker[] = "mqtt-dashboard.com"; // MQTT broker server
int port = 1883; // MQTT broker port
const char topic[] = "mywave"; // MQTT topic

const long interval = 1000; // Time interval for measurements
unsigned long previousMillis = 0;

int count = 0;

void setup() {
  Serial.begin(9600);

  while (!Serial) {
    ; // Wait until the computer's serial connection is ready
  }

  Serial.print("Connecting to Wi-Fi network: ");
  Serial.println(ssid);

  while (WiFi.begin(ssid, pass) != WL_CONNECTED) {
    Serial.print("."); // Show dots while trying to connect
    delay(5000); // Wait for Wi-Fi connection
  }

  Serial.println("Connected to Wi-Fi network!");
  Serial.println();

  Serial.print("Connecting to MQTT broker: ");
  Serial.println(broker);

  if (!mqttClient.connect(broker, port)) {
    Serial.print("MQTT connection failed! Error code = ");
    Serial.println(mqttClient.connectError());

    while (1); // Hangs the program if MQTT connection fails
  }

  Serial.println("Connected to MQTT broker!");
  Serial.println();
}

void loop() {
  mqttClient.poll(); // Maintain the MQTT connection

  unsigned long currentMillis = millis();

  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;

    // Measure distance using the ultrasonic sensor
    digitalWrite(trigPin, LOW);
    delayMicroseconds(2);
    digitalWrite(trigPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigPin, LOW);

    duration = pulseIn(echoPin, HIGH);
    distance = (duration * .0343) / 2;
    Serial.print("Distance: ");
    Serial.println(distance);

    // Check if the detected distance is less than 12 units
    if (distance < 12) {
      // Send an MQTT message if a wave is detected
      mqttClient.beginMessage(topic);
      mqttClient.print("Sohil Nagpal : Wave is detected, ");
      mqttClient.print("Distance: ");
      mqttClient.print(distance);
      mqttClient.endMessage();
      delay(1000); // Wait before sending another message
    }

    Serial.println();

    count++;
  }
}
