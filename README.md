# IoT-Based Smart Helmet for Coal Mine Safety

## 📌 Project Overview
This project is an **IoT-enabled smart helmet** designed to **enhance safety for coal mine workers**. It integrates **gas detection (MQ-2), temperature & humidity monitoring (DHT11), helmet compliance check, and obstacle detection (IR sensor)**. The system uses **ESP-NOW** for communication between transmitter and receiver ESP32 modules and connects to **Blynk IoT Cloud** for real-time monitoring. The helmet also includes **SOS alerts and buzzer warnings** to improve worker safety in hazardous environments.

---

## ✅ Features
- **Gas Detection:** Detects harmful gases like methane, carbon monoxide, and smoke (MQ-2 sensor).
- **Temperature & Humidity Monitoring:** Tracks environmental conditions using the DHT11 sensor.
- **Helmet Compliance:** Ensures the helmet is worn using an IR sensor.
- **Obstacle Detection:** Alerts workers about obstructions.
- **Wireless Communication:** Uses **ESP-NOW protocol** for data transfer between ESP32 modules.
- **Cloud Connectivity:** Sends real-time data to **Blynk IoT Cloud** via Wi-Fi.
- **Live Monitoring & Alerts:** Displays real-time sensor readings on the **Blynk mobile app**.
- **Emergency SOS Signal:** Automatically triggers an alert if hazardous conditions are detected.
- **Buzzer Warning System:** Alerts users of dangerous conditions.

---

## 🛠 Tools & Technologies Used
- **Microcontroller:** ESP32
- **Communication Protocol:** ESP-NOW, Wi-Fi (for cloud connectivity)
- **Cloud Platform:** Blynk IoT
- **Sensors:**
  - **MQ-2:** Gas detection
  - **DHT11:** Temperature & humidity monitoring
  - **IR Sensor:** Helmet compliance & obstacle detection
- **Programming Language:** C++ (Arduino IDE)
- **Development Environment:** Arduino IDE

---

## 📡 System Architecture
1. **Transmitter Module (Helmet Side)**
   - Collects sensor data (MQ-2, DHT11, IR).
   - Sends data wirelessly to the receiver via **ESP-NOW**.
   - Triggers SOS alert if thresholds exceed.

2. **Receiver Module**
   - Receives sensor data from the transmitter ESP32.
   - Uploads data to **Blynk Cloud** for real-time monitoring.
   - Activates **buzzer warnings** if unsafe conditions are detected.

3. **Cloud Integration (Blynk IoT)**
   - Stores real-time sensor data.
   - Displays alerts and live monitoring dashboard.

---

## 🔧 Hardware Requirements
| Component      | Description  |
|---------------|-------------|
| **ESP32**      | Microcontroller (Transmitter & Receiver) |
| **MQ-2 Sensor** | Gas detection (methane, CO, smoke) |
| **DHT11 Sensor** | Temperature & Humidity monitoring |
| **IR Sensor** | Helmet compliance check & obstacle detection |
| **Buzzer** | Alerts the worker in case of danger |
| **LiPo Battery** | Power supply for ESP32 module |
| **Jumper Wires** | For circuit connections |

---

## 📲 Software Requirements
- **Arduino IDE** (for programming ESP32)
- **Blynk IoT Cloud** (for remote monitoring)
- **ESP-NOW Protocol** (for wireless communication)
- **Required Libraries:** 
  - `ESP32_Blynk`
  - `DHT.h`
  - `MQ2.h`
  - `IRremote.h`
  - `ESP-NOW.h`
  - `WiFi.h`

---

## ⚙️ Installation & Setup Guide
### **Step 1: Install Arduino Libraries**
1. Open **Arduino IDE**.
2. Go to **Sketch → Include Library → Manage Libraries**.
3. Install the following libraries:
   - **Blynk** for cloud integration.
   - **ESP-NOW** for wireless communication.
   - **DHT Sensor Library** for temperature & humidity monitoring.

### **Step 2: Upload Code to ESP32**
1. Open `Transmitter.ino` in Arduino IDE.
2. Select **ESP32 Dev Module** as the board.
3. Compile and upload the code to the ESP32 **(Transmitter).**
4. Repeat the process for `Receiver_part.ino`.

### **Step 3: Connect to Blynk**
1. Download **Blynk App** on your phone.
2. Create a new project and add **gauge widgets** for gas, temperature, and humidity.
3. Get the **Auth Token** from Blynk and update it in the Arduino code.
4. Upload the code again and monitor real-time data.

---
## ⚠️ Safety Considerations
- Ensure the gas sensor **is calibrated properly** for accurate readings.
- Secure all **circuit connections** inside the helmet to avoid damage.
- Use a **proper power supply** for ESP32 to prevent voltage fluctuations.

---

## 🚀 Future Improvements
- **Edge AI Integration:** Implement ML models for predictive analysis.
- **GPS Tracking:** Add real-time worker location tracking.
- **Long-Range Communication:** Use LoRaWAN for remote mines.
- **Battery Optimization:** Implement low-power sleep modes for longer battery life.


## 👨‍💻 Author
- **Jashan Kamboj**
- GitHub: https://github.com/jashankaamboj
- Email: jashankaamboj@gmail.com

