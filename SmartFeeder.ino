/********************************************************
   SMART FEEDER CAPSTONE TERBAIK
********************************************************/

// =====================================================
// BLYNK
// =====================================================

#define BLYNK_TEMPLATE_ID "TMPL6IxIkeR3t"
#define BLYNK_TEMPLATE_NAME "SMART FEEDER"
#define BLYNK_AUTH_TOKEN "srMd6Ij24rxYndhwmdurgpOAzXhLnMyo"

#include <WiFi.h>
#include <BlynkSimpleEsp32.h>

// =====================================================
// LIBRARY
// =====================================================

#include <Wire.h>
#include <RTClib.h>
#include <ESP32Servo.h>

// =====================================================
// WIFI
// =====================================================

char ssid[] = "Kartika Wifi";
char pass[] = "orangerti";

// =====================================================
// RTC & SERVO
// =====================================================

RTC_DS3231 rtc;
Servo servoMotor;

BlynkTimer timer;

// =====================================================
// TERMINAL BLYNK
// V2 = LOG SISTEM
// =====================================================

WidgetTerminal terminal(V2);

// =====================================================
// PIN
// =====================================================

#define TRIG_PIN 5
#define ECHO_PIN 18
#define SERVO_PIN 13

// =====================================================
// JADWAL PAKAN
// =====================================================

int feedHour = 7;
int feedMinute = 0;

String jadwalPakan = "07:00";

// Status feed
bool sudahFeed = false;
bool notifikasiStokHabis = false;

// =====================================================
// NAMA HARI
// =====================================================

char daysOfTheWeek[7][12] = {
  "Minggu",
  "Senin",
  "Selasa",
  "Rabu",
  "Kamis",
  "Jumat",
  "Sabtu"
};

// =====================================================
// FUNCTION MEMBERI PAKAN
// =====================================================

void beriPakan(String modeFeed) {

  // =========================
  // AMBIL WAKTU RTC
  // =========================

  DateTime now = rtc.now();

  // =========================
  // FORMAT TANGGAL
  // =========================

  String tanggal =
    String(now.day()) + "/" +
    String(now.month()) + "/" +
    String(now.year());

  // =========================
  // FORMAT JAM
  // =========================

  String jam =
    (now.hour() < 10 ? "0" : "") + String(now.hour());

  String menit =
    (now.minute() < 10 ? "0" : "") + String(now.minute());

  String detik =
    (now.second() < 10 ? "0" : "") + String(now.second());

  String waktu = jam + ":" + menit + ":" + detik;
  String infoWaktu = tanggal + " " + waktu;

  // =========================
  // LOG SERIAL MONITOR
  // =========================

  Serial.println("=======================");
  Serial.print("MODE : ");
  Serial.println(modeFeed);

  Serial.print("TGL  : ");
  Serial.println(tanggal);

  Serial.print("JAM  : ");
  Serial.println(waktu);

  Serial.println("SERVO MEMBUKA");

  // =========================
  // LOG KE TERMINAL BLYNK
  // =========================

  terminal.println("====================");
  terminal.println("MODE : " + modeFeed);
  terminal.println("TGL  : " + tanggal);
  terminal.println("JAM  : " + waktu);
  terminal.println("====================");
  terminal.println();

  terminal.flush();

  // =========================
  // NOTIFIKASI BLYNK
  // =========================

if (modeFeed == "MANUAL FEED") {

  Blynk.logEvent(
    "manual_feed",
    "Manual Feed : " + infoWaktu
  );
}

if (modeFeed == "FEED OTOMATIS") {

  Blynk.logEvent(
    "feed_otomatis",
    "Feed Otomatis : " + infoWaktu
  );
}

  // =========================
  // GERAKKAN SERVO
  // =========================

  servoMotor.write(90);

  delay(1000);

  servoMotor.write(0);

  Serial.println("SERVO MENUTUP");
  Serial.println("=======================");
}

// =====================================================
// MANUAL FEED DARI BLYNK
// V3
// =====================================================

BLYNK_WRITE(V3)
{
  int tombol = param.asInt();

  Serial.print("Nilai Tombol : ");
  Serial.println(tombol);

  if (tombol == 1) {

    Serial.println("MANUAL FEED");

    beriPakan("MANUAL FEED");
  }
}

BLYNK_WRITE(V5)
{
  long totalDetik = param.asLong();

  feedHour = totalDetik / 3600;

  feedMinute = (totalDetik % 3600) / 60;

  char buffer[10];

  sprintf(
    buffer,
    "%02d:%02d",
    feedHour,
    feedMinute
  );

  jadwalPakan = String(buffer);

  Blynk.virtualWrite(V6, jadwalPakan);

  Serial.println("====================");
  Serial.println("JADWAL BARU");
  Serial.println(jadwalPakan);
  Serial.println("====================");

  terminal.println("====================");
  terminal.println("JADWAL BARU");
  terminal.println(jadwalPakan);
  terminal.println("====================");
  terminal.println();

  terminal.flush();
}
// =====================================================
// FUNCTION SENSOR & RTC
// =====================================================

void bacaSensorDanRTC() {

  // =========================
  // RTC
  // =========================

  DateTime now = rtc.now();

  // =========================
  // TAMPILKAN WAKTU
  // =========================

  Serial.print("Hari    : ");
  Serial.println(daysOfTheWeek[now.dayOfTheWeek()]);

  Serial.print("Tanggal : ");
  Serial.print(now.day());
  Serial.print("/");
  Serial.print(now.month());
  Serial.print("/");
  Serial.println(now.year());

  Serial.print("Waktu   : ");

  if (now.hour() < 10) Serial.print("0");
  Serial.print(now.hour());

  Serial.print(":");

  if (now.minute() < 10) Serial.print("0");
  Serial.print(now.minute());

  Serial.print(":");

  if (now.second() < 10) Serial.print("0");
  Serial.println(now.second());

  // =========================
  // SENSOR ULTRASONIK
  // =========================

  long duration;
  float distance;

  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);

  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);

  digitalWrite(TRIG_PIN, LOW);

  duration = pulseIn(ECHO_PIN, HIGH);

  distance = duration * 0.034 / 2;

  // =========================
  // SERIAL MONITOR
  // =========================

  Serial.print("Jarak Pakan : ");
  Serial.print(distance);
  Serial.println(" cm");

  // =========================
  // KIRIM JARAK KE BLYNK
  // V0
  // =========================

  Blynk.virtualWrite(V0, distance);

  // =========================
  // STATUS STOK PAKAN
  // =========================

  if (distance > 14) {

  Serial.println("STOK PAKAN HABIS");

  Blynk.virtualWrite(V1, "STOK HABIS");

  if (!notifikasiStokHabis) {

    String waktuNotif =
      String(now.day()) + "/" +
      String(now.month()) + "/" +
      String(now.year()) + " " +
      String(now.hour()) + ":" +
      String(now.minute());

    Blynk.logEvent(
      "stok_habis",
      "Stok pakan habis pada " + waktuNotif
    );

    terminal.println("⚠ STOK PAKAN HABIS");
    terminal.println("WAKTU : " + waktuNotif);
    terminal.println();

    terminal.flush();

    notifikasiStokHabis = true;
  }
}
else {

  Serial.println("STOK PAKAN TERSEDIA");

  Blynk.virtualWrite(V1, "STOK TERSEDIA");

  notifikasiStokHabis = false;
}

  // =========================
  // JADWAL OTOMATIS
  // =========================

  if (now.hour() == feedHour &&
      now.minute() == feedMinute &&
      !sudahFeed) {

    Serial.println("MEMBERI PAKAN OTOMATIS");

    beriPakan("FEED OTOMATIS");

    sudahFeed = true;
  }

  // =========================
  // RESET STATUS FEED
  // =========================

  if (now.minute() != feedMinute) {

    sudahFeed = false;
  }

  Serial.println("-----------------------");
}

// =====================================================
// SETUP
// =====================================================

void setup() {

  Serial.begin(115200);

  // =========================
  // BLYNK
  // =========================

  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);

  Blynk.syncVirtual(V5);

  Serial.println("BLYNK TERHUBUNG");

  // =========================
  // RTC
  // =========================

  Wire.begin(21, 22);

  if (!rtc.begin()) {

    Serial.println("RTC tidak ditemukan");

    while (1);
  }

  // =====================================================
  // AKTIFKAN SEKALI SAJA UNTUK SET WAKTU RTC
  // Setelah upload pertama -> comment lagi
  // =====================================================

  // rtc.adjust(DateTime(2026, 5, 27, 10, 30, 0));

  // =========================
  // ULTRASONIK
  // =========================

  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);

  // =========================
  // SERVO
  // =========================

  servoMotor.setPeriodHertz(50);

  servoMotor.attach(SERVO_PIN, 500, 2400);

  servoMotor.write(0);

  // =========================
  // TIMER
  // =========================

  timer.setInterval(1000L, bacaSensorDanRTC);

  // =========================
  // LOG AWAL
  // =========================

  terminal.println("SMART FEEDER READY");
  terminal.println("====================");
  terminal.flush();

  Blynk.virtualWrite(V6, jadwalPakan);

  Serial.println("SMART FEEDER READY");

  Blynk.logEvent(
  "device_online",
  "Smart Feeder berhasil terhubung ke Blynk"
);

terminal.println("DEVICE ONLINE");
terminal.println();
terminal.flush();
}

// =====================================================
// LOOP
// =====================================================

void loop() {

  Blynk.run();

  timer.run();
}