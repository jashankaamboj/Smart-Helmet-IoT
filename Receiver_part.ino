#include <WiFi.h>
#include <esp_now.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);

typedef struct {
  float temperature;
  float humidity;
  int gasValue;
  bool helmetWorn;
  bool objectDetected;
  bool sosTriggered;
} SensorData;

SensorData receivedData;

uint8_t transmitterAddress[] = {0xFC, 0xE8, 0xC0, 0x7C, 0xE0, 0x34};

void OnDataRecv(const esp_now_recv_info *info, const uint8_t *incomingData, int len) {
  memcpy(&receivedData, incomingData, sizeof(receivedData));
  lcd.clear();

  if (receivedData.sosTriggered) {
    lcd.setCursor(0, 0);
    lcd.print("SOS Alert!");
    lcd.setCursor(0, 1);
    lcd.print("Help Needed!");
    return;
  }

  lcd.setCursor(0, 0);
  lcd.print("T:");
  lcd.print(receivedData.temperature, 1);
  lcd.print(" H:");
  lcd.print(receivedData.humidity, 1);

  lcd.setCursor(0, 1);
  if (receivedData.temperature >= 87)
    lcd.print("HIGH TEMP WARN!");
  else if (receivedData.gasValue >= 500)
    lcd.print("GAS DETECTED!");
  else if (!receivedData.helmetWorn)
    lcd.print("NO HELMET!");
  else if (receivedData.objectDetected)
    lcd.print("OBJ DETECTED!");
  else
    lcd.print("ALL CLEAR");
}

void setup() {
  Serial.begin(115200);
  lcd.init();
  lcd.backlight();

  lcd.setCursor(0, 0);
  lcd.print("Smart Helmet");
  lcd.setCursor(0, 1);
  lcd.print("Receiver Ready");
  delay(2000);

  WiFi.mode(WIFI_STA);

  if (esp_now_init() != ESP_OK) {
    lcd.clear();
    lcd.print("ESP-NOW Init Fail");
    return;
  }

  esp_now_register_recv_cb(OnDataRecv);

  esp_now_peer_info_t peerInfo;
  memset(&peerInfo, 0, sizeof(peerInfo));
  memcpy(peerInfo.peer_addr, transmitterAddress, 6);
  peerInfo.channel = 0;
  peerInfo.encrypt = false;

  if (esp_now_add_peer(&peerInfo) != ESP_OK) {
    lcd.clear();
    lcd.print("Peer Add Failed");
    return;
  }

  lcd.clear();
  lcd.print("ESP-NOW Ready");
}

void loop() {
  delay(1000);
}
