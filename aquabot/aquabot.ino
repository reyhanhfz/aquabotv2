#define BLYNK_TEMPLATE_ID "TMPL6_gYDS5u-"
#define BLYNK_TEMPLATE_NAME "project"
#define BLYNK_AUTH_TOKEN "tyTk_KYFxH7EaxxsLsDJwtBaOtWFoVll"

#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <NewPing.h>
#include <Servo.h>

char ssid[] = "Agus Racing";
char pass[] = "siburian";

// Pin setup
#define TRIG_PIN D2
#define ECHO_PIN D1
#define RELAY_PIN D5
#define SERVO_PIN D3 // Ganti jika tidak stabil: D4 / D6

#define TINGGI_TANGKI 20 // cm
#define MAX_SENSOR_DISTANCE 100

#define BATAS_NYALA 4 // Jika air < 4 cm → Pompa nyala
#define BATAS_MATI 15 // Jika air > 15 cm → Pompa mati

NewPing sonar(TRIG_PIN, ECHO_PIN, MAX_SENSOR_DISTANCE);
BlynkTimer timer;
Servo myServo;

bool pompaStatus = false;

void sendDistance()
{
  int distance = sonar.ping_cm();
  if (distance == 0 || distance > TINGGI_TANGKI + 10)
  {
    Serial.println("Sensor error / tidak terbaca");
    return;
  }

  int tinggiAir = TINGGI_TANGKI - distance;
  if (tinggiAir < 0)
    tinggiAir = 0;
  if (tinggiAir > TINGGI_TANGKI)
    tinggiAir = TINGGI_TANGKI;

  int persenKetinggian = map(tinggiAir, 0, TINGGI_TANGKI, 0, 100);

  Serial.print("Ketinggian air: ");
  Serial.print(tinggiAir);
  Serial.print(" cm (");
  Serial.print(persenKetinggian);
  Serial.println("%)");

  Blynk.virtualWrite(V0, distance);
  Blynk.virtualWrite(V1, tinggiAir);
  Blynk.virtualWrite(V4, persenKetinggian);

  if (tinggiAir < BATAS_NYALA && !pompaStatus)
  {
    digitalWrite(RELAY_PIN, LOW); // Pompa ON
    pompaStatus = true;
    Serial.println("Pompa DINYALAKAN (otomatis)");
    Blynk.virtualWrite(V2, "Pompa AKTIF (Otomatis)");
    Blynk.virtualWrite(V3, 1);
  }
  else if (tinggiAir > BATAS_MATI && pompaStatus)
  {
    digitalWrite(RELAY_PIN, HIGH); // Pompa OFF
    pompaStatus = false;
    Serial.println("Pompa DIMATIKAN (otomatis)");
    Blynk.virtualWrite(V2, "Pompa MATI (Otomatis)");
    Blynk.virtualWrite(V3, 0);
  }
}

// Kontrol manual pompa dari Blynk
BLYNK_WRITE(V3)
{
  int state = param.asInt();
  if (state == 1)
  {
    digitalWrite(RELAY_PIN, LOW);
    pompaStatus = true;
    Serial.println("Pompa DINYALAKAN (Manual)");
    Blynk.virtualWrite(V2, "Pompa AKTIF (Manual)");
  }
  else
  {
    digitalWrite(RELAY_PIN, HIGH);
    pompaStatus = false;
    Serial.println("Pompa DIMATIKAN (Manual)");
    Blynk.virtualWrite(V2, "Pompa MATI (Manual)");
  }
}

// Kontrol servo dari Blynk (Slider V5, 0–180)
BLYNK_WRITE(V5)
{
  int sudut = param.asInt();
  sudut = constrain(sudut, 0, 180);
  myServo.attach(SERVO_PIN); // Attach saat akan digunakan
  myServo.write(sudut);
  Serial.print("Servo bergerak ke: ");
  Serial.print(sudut);
  Serial.println(" derajat");
  delay(500); // Opsional untuk memastikan stabil
  // myServo.detach();            // Bisa diaktifkan jika ingin servo tidak bergetar terus
}

void setup()
{
  Serial.begin(115200);
  pinMode(RELAY_PIN, OUTPUT);
  digitalWrite(RELAY_PIN, HIGH); // Pompa mati awal

  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);
  timer.setInterval(1000L, sendDistance);

  myServo.attach(SERVO_PIN); // Posisi awal servo
  myServo.write(0);
}

void loop()
{
  Blynk.run();
  timer.run();
}
