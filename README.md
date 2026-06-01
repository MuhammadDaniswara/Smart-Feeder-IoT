# 🐦 Smart Feeder Burung Berbasis IoT

Smart Feeder adalah sistem pemberi pakan burung otomatis berbasis Internet of Things (IoT) menggunakan ESP32. Sistem ini dapat memberikan pakan secara otomatis berdasarkan jadwal yang ditentukan melalui aplikasi Blynk, serta memungkinkan pemberian pakan secara manual dari smartphone.

## 📌 Fitur Utama

- ✅ Monitoring stok pakan secara real-time menggunakan sensor ultrasonik HC-SR04
- ✅ Pemberian pakan otomatis berdasarkan jadwal RTC DS3231
- ✅ Pengaturan jadwal pakan melalui aplikasi Blynk
- ✅ Pemberian pakan manual melalui aplikasi Blynk
- ✅ Monitoring status stok pakan
- ✅ Log aktivitas sistem lengkap dengan tanggal dan waktu
- ✅ Notifikasi saat:
  - Stok pakan habis
  - Pakan diberikan secara otomatis
  - Pakan diberikan secara manual
  - Perangkat kembali online
- ✅ Monitoring jarak stok pakan dalam satuan cm
- ✅ Dashboard Web dan Mobile Blynk

---

# 📷 Sistem

## Diagram Blok

```text
                +------------------+
                |    Blynk IoT     |
                | Web & Mobile App |
                +---------+--------+
                          |
                          |
                     WiFi |
                          |
                +---------v--------+
                |      ESP32       |
                +---------+--------+
                          |
      +-------------------+-------------------+
      |                   |                   |
      |                   |                   |
+-----v-----+      +------v------+      +-----v-----+
| HC-SR04   |      | DS3231 RTC  |      | Servo SG90|
| Ultrasonik|      | Real Time   |      | Dispenser |
+-----------+      +-------------+      +-----------+
```

---

# 🔧 Hardware yang Digunakan

| No | Komponen                     |
|----|------------------------------|
| 1  | ESP32 DevKit V1              |
| 2  | Sensor Ultrasonik HC-SR04    |
| 3  | RTC DS3231                   |
| 4  | Servo Motor SG90/MG90S       |
| 5  | Breadboard                   |
| 6  | Kabel Jumper                 |
| 7  | Wadah Pakan Burung           |
| 8  | Smartphone dengan Blynk IoT  |

---

# 🔌 Wiring

## RTC DS3231

| DS3231 | ESP32 |
|---------|--------|
| VCC | 3.3V    |
| GND | GND     |
| SDA | GPIO 21 |
| SCL | GPIO 22 |

---

## HC-SR04

| HC-SR04 | ESP32 |
|----------|--------|
| VCC | 5V |
| GND | GND |
| TRIG | GPIO 5 |
| ECHO | GPIO 18 |

---

## Servo

| Servo | ESP32 |
|--------|--------|
| VCC | 5V |
| GND | GND |
| Signal | GPIO 13 |

---

# 📱 Blynk Dashboard

## Datastream

| Virtual Pin | Fungsi |
|-------------|---------|
| V0 | Jarak stok pakan |
| V1 | Status stok pakan |
| V2 | Log sistem |
| V3 | Tombol manual feed |
| V5 | Input jadwal pakan |
| V6 | Tampilan jadwal aktif |

---

## Widget Mobile

| Widget | Datastream |
|----------|-----------|
| Gauge | V0 |
| Labeled Value | V1 |
| Button | V3 |
| Time Input | V5 |
| Labeled Value | V6 |
| Terminal | V2 |

---

# 🔔 Event Notification

Buat event berikut pada Blynk:

| Event Name | Event Code |
|------------|------------|
| Device Online | device_online |
| Manual Feed | manual_feed |
| Feed Otomatis | feed_otomatis |
| Stok Habis | stok_habis |

---

# ⚙️ Cara Kerja Sistem

1. ESP32 membaca waktu dari RTC DS3231.
2. Pengguna menentukan jadwal pakan melalui Blynk.
3. Jadwal disimpan pada variabel:
   - feedHour
   - feedMinute
4. Saat waktu RTC sama dengan jadwal:
   - Servo membuka dispenser pakan.
   - Log sistem dicatat.
   - Notifikasi dikirim ke pengguna.
5. Sensor ultrasonik memantau stok pakan.
6. Jika stok habis:
   - Status berubah menjadi "STOK HABIS".
   - Notifikasi dikirim ke smartphone.

---

# 📊 Monitoring

## Status Tersedia

```text
Jarak Pakan : 5 cm
STOK TERSEDIA
```

## Status Habis

```text
Jarak Pakan : 18 cm
STOK HABIS
```

---

# 📝 Contoh Log Sistem

```text
====================
MODE : MANUAL FEED
TGL  : 27/05/2026
JAM  : 14:25:31
====================

====================
MODE : FEED OTOMATIS
TGL  : 27/05/2026
JAM  : 17:00:00
====================
```

---

# 🚀 Instalasi

## Library Arduino IDE

Install library berikut:

```text
Blynk
RTClib
ESP32Servo
Wire
WiFi
```

---

## Konfigurasi WiFi

```cpp
char ssid[] = "Nama_Wifi";
char pass[] = "Password_Wifi";
```

---

## Konfigurasi Blynk

```cpp
#define BLYNK_TEMPLATE_ID ""
#define BLYNK_TEMPLATE_NAME ""
#define BLYNK_AUTH_TOKEN ""
```

---

# 📚 Teknologi yang Digunakan

- ESP32
- Arduino IDE
- Blynk IoT
- RTC DS3231
- HC-SR04
- Servo Motor
- WiFi Communication

---

# 👨‍💻 Pengembang

**Muhammad Bahari Layana Daniswara**

Program Studi Teknik Elektro

Proyek Capstone Design

---

# 📄 Lisensi

Proyek ini dibuat untuk tujuan pembelajaran, penelitian, dan pengembangan sistem Smart Feeder berbasis IoT.
