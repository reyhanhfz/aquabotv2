
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <NewPing.h>
#include <Servo.h>

#define TRIG_PIN D2
#define ECHO_PIN D1
#define RELAY_PIN D5
#define SERVO_PIN D7
#define TINGGI_TANGKI 20
#define BATAS_NYALA 4
#define BATAS_MATI 15
#define MAX_SENSOR_DISTANCE 100

#define BLYNK_TEMPLATE_ID "TMPL68kaopvDz"
#define BLYNK_TEMPLATE_NAME "aquabot"
#define BLYNK_AUTH_TOKEN "8DalHhkQ7xhPIpR66NBG35mUUlVf-16e"

#define WIFI_SSID "rico"
#define WIFI_PASS "12345678910"

NewPing sonar(TRIG_PIN, ECHO_PIN, MAX_SENSOR_DISTANCE);
BlynkTimer timer;
Servo myServo;
bool pompaStatus = false;

void sendDistance()
{
  int distance = sonar.ping_cm();
  if (distance == 0 || distance > TINGGI_TANGKI + 10)
    return;

  int tinggiAir = TINGGI_TANGKI - distance;
  tinggiAir = constrain(tinggiAir, 0, TINGGI_TANGKI);
  int persenKetinggian = map(tinggiAir, 0, TINGGI_TANGKI, 0, 100);

  Blynk.virtualWrite(V0, distance);
  Blynk.virtualWrite(V1, tinggiAir);
  Blynk.virtualWrite(V4, persenKetinggian);

  if (tinggiAir < BATAS_NYALA && !pompaStatus)
  {
    digitalWrite(RELAY_PIN, LOW);
    pompaStatus = true;
    Blynk.virtualWrite(V2, "Pompa AKTIF (Otomatis)");
    Blynk.virtualWrite(V3, 1);
  }
  else if (tinggiAir > BATAS_MATI && pompaStatus)
  {
    digitalWrite(RELAY_PIN, HIGH);
    pompaStatus = false;
    Blynk.virtualWrite(V2, "Pompa MATI (Otomatis)");
    Blynk.virtualWrite(V3, 0);
  }
}

BLYNK_WRITE(V3)
{
  int state = param.asInt();
  digitalWrite(RELAY_PIN, state ? LOW : HIGH);
  pompaStatus = state;
  Blynk.virtualWrite(V2, state ? "Pompa AKTIF (Manual)" : "Pompa MATI (Manual)");
}

BLYNK_WRITE(V5)
{
  int sudut = constrain(param.asInt(), 0, 180);
  myServo.write(sudut);
}

void setup()
{
  Serial.begin(115200);
  pinMode(RELAY_PIN, OUTPUT);
  digitalWrite(RELAY_PIN, HIGH);
  Blynk.begin(BLYNK_AUTH_TOKEN, WIFI_SSID, WIFI_PASS);
  timer.setInterval(1000L, sendDistance);
  myServo.attach(SERVO_PIN);
  myServo.write(0);
}

void loop()
{
  Blynk.run();
  timer.run();
}
