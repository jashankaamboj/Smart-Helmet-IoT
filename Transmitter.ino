#define BLYNK_TEMPLATE_ID "TMPL3LztUHxAH"
#define BLYNK_TEMPLATE_NAME "helmet"
#include <WiFi.h>
#include <esp_now.h>
#include <DHT.h>
#include <BlynkSimpleEsp32.h>

// Define DHT sensor type and pin
#define DHTPIN 21
#define DHTTYPE DHT11
#define BUZZER_PIN 22

// MQ-2 Gas Sensor Pins
#define MQ2_AO_PIN 35
#define MQ2_DO_PIN 15

// IR Sensor Pins
#define IR_SENSOR_PIN1 4
#define IR_SENSOR_PIN2 2

// SOS Button Pin
#define SOS_BUTTON_PIN 14

// Gas sensor threshold
const int gasThreshold = 500;

// Blynk Auth Token (replace with your token from the Blynk app)
char auth[] = "oTO9F4j-DVh8oU7Cw4f3jHkZU1cYSUDx";

// Wi-Fi credentials
char ssid[] = "Himanshu’s Iphone";
char pass[] = "61000000";

// Initialize DHT sensor
DHT dht(DHTPIN, DHTTYPE);

// Global variable to track buzzer state
bool buzzerOn = false;

// Structure to hold sensor data
typedef struct {
  float temperature;
  float humidity;
  int gasValue;
  bool helmetWorn;
  bool objectDetected;
  bool sosTriggered;  // Variable to hold SOS status
} SensorData;

SensorData dataToSend;

// MAC address of the receiver ESP32 (update to match your receiver)
uint8_t receiverAddress[] = {0xfc, 0xe8, 0xc0, 0x7c, 0xe0, 0x34};

// Callback function for ESP-NOW send status
void onDataSent(const uint8_t *macAddr, esp_now_send_status_t status) {
  Serial.print("Send status: ");
  Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Success" : "Fail");
}

void setup() {
  Serial.begin(115200);
  dht.begin();

  pinMode(BUZZER_PIN, OUTPUT);
  pinMode(MQ2_DO_PIN, INPUT);
  pinMode(IR_SENSOR_PIN1, INPUT);
  pinMode(IR_SENSOR_PIN2, INPUT);
  pinMode(SOS_BUTTON_PIN, INPUT_PULLDOWN);

  // Initialize Wi-Fi for ESP-NOW
  WiFi.mode(WIFI_STA);
  Serial.println("ESP32 Transmitter Initialized");

  // Initialize ESP-NOW
  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }

  // Register send callback function
  esp_now_register_send_cb(onDataSent);

  // Set up the receiver as a peer
  esp_now_peer_info_t peerInfo;
  memcpy(peerInfo.peer_addr, receiverAddress, 6);
  peerInfo.channel = 0;
  peerInfo.encrypt = false;

  // Add the receiver as a peer
  if (esp_now_add_peer(&peerInfo) != ESP_OK) {
    Serial.println("Failed to add peer");
    return;
  }

  // Connect to Wi-Fi for Blynk
  WiFi.begin(ssid, pass);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nConnected to Wi-Fi");

  // Initialize Blynk
  Blynk.begin(auth, ssid, pass);
}

void loop() {
  bool shouldBuzzerBeOn = false;

  // --- Read Temperature and Humidity (DHT11) ---
  dataToSend.humidity = dht.readHumidity();
  dataToSend.temperature = dht.readTemperature();

  if (isnan(dataToSend.humidity) || isnan(dataToSend.temperature)) {
    Serial.println("Failed to read from DHT sensor!");
  } else {
    Serial.print("Humidity: ");
    Serial.print(dataToSend.humidity);
    Serial.print(" %, Temperature: ");
    Serial.print(dataToSend.temperature);
    Serial.println(" °C");

    if (dataToSend.temperature >= 45) {
      shouldBuzzerBeOn = true;
      Serial.println("Warning: High temperature!");
    }

    // Send temperature and humidity to Blynk
    Blynk.virtualWrite(V0, dataToSend.temperature); // V0 for Temperature
    Blynk.virtualWrite(V1, dataToSend.humidity);    // V1 for Humidity
  }

  // --- Read Gas Levels (MQ-2) ---
  dataToSend.gasValue = analogRead(MQ2_AO_PIN);
  Serial.print("Gas Sensor Value: ");
  Serial.println(dataToSend.gasValue);

  if (dataToSend.gasValue >= gasThreshold) {
    shouldBuzzerBeOn = true;
    Serial.println("Warning: Gas level above threshold!");
  }

  // Send gas value to Blynk
  Blynk.virtualWrite(V2, dataToSend.gasValue); // V2 for Gas Sensor Value

  // --- Check Helmet Detection (IR Sensor) ---
  int helmetIRValue = digitalRead(IR_SENSOR_PIN2);
  dataToSend.helmetWorn = (helmetIRValue == LOW);

  if (dataToSend.helmetWorn) {
    Serial.println("Helmet is worn.");
  } else {
    shouldBuzzerBeOn = true;
    Serial.println("Helmet not detected. Please wear the helmet.");
  }

  // Send helmet status to Blynk
  Blynk.virtualWrite(V3, dataToSend.helmetWorn); // V3 for Helmet Detection

  // --- Object Detection (IR Sensor 1) ---
  int objectIRValue = digitalRead(IR_SENSOR_PIN1);
  dataToSend.objectDetected = (objectIRValue == LOW);

  if (dataToSend.objectDetected) {
    shouldBuzzerBeOn = true;
    Serial.println("Object detected in front.");
  } else {
    Serial.println("No object detected.");
  }

  // Send object detection status to Blynk
  Blynk.virtualWrite(V4, dataToSend.objectDetected); // V4 for Object Detection

  // --- Check SOS Button ---
  if (digitalRead(SOS_BUTTON_PIN) == HIGH) {
    dataToSend.sosTriggered = true;
    shouldBuzzerBeOn = true;
    Serial.println("SOS Button Pressed! Sending emergency alert.");
  } else {
    dataToSend.sosTriggered = false;
  }

  // Send SOS status to Blynk
  Blynk.virtualWrite(V5, dataToSend.sosTriggered); // V5 for SOS Alert

  // Control the buzzer
  if (shouldBuzzerBeOn && !buzzerOn) {
    digitalWrite(BUZZER_PIN, HIGH);
    buzzerOn = true;
    Serial.println("Buzzer ON");
  } else if (!shouldBuzzerBeOn && buzzerOn) {
    digitalWrite(BUZZER_PIN, LOW);
    buzzerOn = false;
    Serial.println("Buzzer OFF");
  }

  // Send the data using ESP-NOW
  esp_now_send(receiverAddress, (uint8_t *)&dataToSend, sizeof(dataToSend));

  // Run Blynk
  Blynk.run();

  // Delay before the next reading
  delay(2000);
}
