# AirCare+ — IoT Air Quality Monitoring System

**Intern:** Amani Arouri
**Specialty:** Embedded Systems / IoT Engineering
**Platform:** AirCare+ (Innovation Ecosystem — Digimytch)
**Duration:** 2 months (8 weeks)

---

## Project Overview
AirCare+ is an air quality monitoring node built around an ESP32.
It reads temperature and humidity (DHT22) and particulate matter
(PMS7003), then publishes the data over MQTT with TLS. A web dashboard
shows the data live, and a Python script saves it to CSV.

**Current status: simulation and software. Real hardware tests are
still to do.**

---

## Architecture

Sensors (DHT22, PMS7003) → ESP32 → MQTT (HiveMQ Cloud, TLS, port 8883)
→ Web dashboard (WebSockets) + Python CSV logger

---

## Status

- [x] Environment setup (PlatformIO, Wokwi)
- [x] DHT22 firmware and wiring simulated in Wokwi
- [x] MQTT publishing and Python subscriber
- [x] Wi-Fi reconnect and watchdog
- [x] Security changes in the code (see below)
- [ ] Firmware build verified after the security changes
- [ ] Live dashboard tested on GitHub Pages
- [ ] PMS7003 added to the Wokwi diagram
- [ ] Physical ESP32 + sensors connected (issue #1)
- [ ] Real data collected
- [ ] Enclosure / field deployment

---

## Month 1 (weeks 1–4)

**Week 1 — Environment setup:** Arduino IDE and PlatformIO, ESP32 and
DHT22 datasheets, LED blink, component list, GitHub repository.

**Week 2 — Sensor integration:** DHT22 wiring in Wokwi, firmware built
in PlatformIO, Python serial logger to CSV, MQTT research and schematic.

**Week 3 — Connectivity:** Mosquitto broker tested, ESP32 publishes to
MQTT, Python MQTT subscriber, Wi-Fi provisioning compared, data flow
diagram.

**Week 4 — Reliability:** Wi-Fi reconnect and watchdog, sensor accuracy
test, system summary, demo video, GitHub Issues for Month 2.

## Month 2 (weeks 5–8)

- Added the PMS7003 particulate sensor (UART) next to the DHT22.
- Moved from a local Mosquitto broker to HiveMQ Cloud with TLS (port 8883).
- Built a live web dashboard (WebSockets) hosted on GitHub Pages.
- Python logger (`mqtt_logger.py`) that saves MQTT data to CSV.
- Made the firmware more reliable: non-blocking MQTT reconnect, Wi-Fi loss
  handled separately, PMS7003 buffer cleanup with checksum validation.
- Security: credentials in a git-ignored `secrets.h`, CA certificate,
  password-protected setup portal.

---

## Security

- Credentials are in `secrets.h`, which is git-ignored. Only
  `secrets.example.h` is in the repo.
- The ESP32 checks the broker with a CA certificate (ISRG Root X1)
  instead of skipping verification.
- The Wi-Fi setup portal is protected by a password.
- The dashboard uses a separate read-only MQTT user. Its password is
  visible in the browser by design, so that user can only subscribe.

---

## Setup

1. Copy `secrets.example.h` to `secrets.h` and fill in your values.
2. Build with PlatformIO.
3. Logger: set the `AIRCARE_MQTT_USER` and `AIRCARE_MQTT_PASS`
   environment variables, then run `python mqtt_logger.py`.

---

## Tech Stack
ESP32-WROOM-32, DHT22, PMS7003, PlatformIO (Arduino framework),
HiveMQ Cloud (MQTT over TLS), Python + Paho-MQTT, WebSockets,
GitHub Pages, Wokwi

---

## What I learned
- How MQTT works: broker, topics, publish and subscribe.
- Why TLS and a CA certificate matter, and why passwords must never be
  committed to a public repo.
- How to make firmware keep running when Wi-Fi or the broker drops.
- How to read a sensor over UART and check its data with a checksum.

---

## Contact
amaniarouri636@gmail.com
