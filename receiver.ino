#include <ArduinoMqttClient>  // Include the Arduino MQTT Client library

// Depending on the board, include the appropriate Wi-Fi library
#if defined(ARDUINO_SAMD_MKRWIFI1010) || defined(ARDUINO_SAMD_NANO_33_IOT) || defined(ARDUINO_AVR_UNO_WIFI_REV2)
#include <WiFiNINA.h>
#elif defined(ARDUINO_SAMD_MKR1000)
#include <WiFi101.h>
#elif defined(ARDUINO_ARCH_ESP8266)
#include <ESP8266WiFi.h>
#elif defined(ARDUINO_ARCH_ESP32)
#include <WiFi.h>
#endif

// Wi-Fi credentials (change these to match your network)
char ssid[] = "Tanmaya's Iphone (2)";
char pass[] = "tanmayaspw";

int light = 13;  // Define the LED pin

WiFiClient wifiClient;  // Create a Wi-Fi client
MqttClient mqttClient(wifiClient);  // Create an MQTT client using the Wi-Fi client
const char broker[] = "mqtt-dashboard.com";  // MQTT broker address
int port = 1883;  // MQTT broker port
const char topic[] = "mywave";  // MQTT topic to subscribe to

void setup() {
  Serial.begin(9600);  // Initialize the serial communication
  pinMode(light, OUTPUT);  // Set the LED pin as an output

  while (!Serial) {
    ;  // Wait for the serial connection to be established
  }

  Serial.print("Connecting to WPA SSID: ");
  Serial.println(ssid);

  while (WiFi.begin(ssid, pass) != WL_CONNECTED) {
    Serial.print(".");  // Wait for the Wi-Fi connection to be established
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

  mqttClient.subscribe(topic);  // Subscribe to the specified MQTT topic
}

void loop() {
  int messageSize = mqttClient.parseMessage();  // Check for incoming MQTT messages

  if (messageSize) {
    Serial.print("Received a message with topic '");
    Serial.print(mqttClient.messageTopic());
    Serial.print("', length ");
    Serial.print(messageSize);
    Serial.println(" bytes:");

    while (mqttClient.available()) {
      Serial.print((char)mqttClient.read());  // Print the received MQTT message
    }

    Serial.println();

    // Blink the LED to indicate message reception
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

    Serial.println();  // Print a newline
  }
}
