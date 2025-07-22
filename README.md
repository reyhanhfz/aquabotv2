# 🌊 Aquabot ESP8266 – Smart Aquarium Monitoring with Blynk

Aquabot is an IoT project built using **ESP8266**, **ultrasonic water level sensor**, **relay-controlled pump**, and **servo motor** to monitor and control a water tank automatically. The system is connected to the **Blynk IoT platform**, providing real-time data and control via mobile app.

## 🚀 Features

- 📡 **Real-time water level monitoring** using ultrasonic sensor
- ⚙️ **Automatic & manual pump control** based on tank level
- 🔄 **Servo motor control** from Blynk (e.g., valve or lid)
- 📲 **Blynk dashboard integration** (virtual pins V0–V5)


---

## 🧩 Hardware Required

- 1x ESP8266 NodeMCU
- 1x HC-SR04 Ultrasonic Sensor
- 1x Relay Module
- 1x Mini Water Pump
- 1x Servo Motor (e.g., SG90)
- Jumper wires & Breadboard
- Power Supply (5V USB or adapter)

---

## 📱 Blynk Virtual Pin Mapping

| Function             | Blynk Virtual Pin |
| -------------------- | ----------------- |
| Distance (raw)       | V0                |
| Water height (cm)    | V1                |
| Water level (%)      | V4                |
| Pump status (text)   | V2                |
| Pump manual switch   | V3                |
| Servo angle (slider) | V5                |

---



## 🛠️ How to Flash

1. Install [VS Code](https://code.visualstudio.com/) 
2. Clone this repo:

```bash
git clone https://github.com/reyhanhfz/aquabot.git
cd aquabot
```

3. Plug in your ESP8266
4. Build & Upload via CLI

---

## 🧠 Project Diagram (Wiring)

- **HC-SR04**
  - TRIG → D2
  - ECHO → D1
- **Relay** → D5
- **Servo** → D7
- **GND** semua disambungkan ke GND
- **VCC** ke 3v (via breadboard)

> Note: Use voltage divider or logic level shifter if needed

---


## 👨‍💻 Author

Made with ❤️ by **@aquabotdeveloper**

---

## 📄 License

This project is open-source under the MIT License.
