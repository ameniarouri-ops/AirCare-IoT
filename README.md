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
shows the data, and a Python script saves it to CSV.

**Current status: simulation and software. Real hardware tests and some
security changes are still to do (see Status).**

---

## Architecture

Sensors (DHT22, PMS7003) → ESP32 → MQTT (HiveMQ Cloud, TLS, port 8883)
→ Web dashboard (WebSockets) + Python CSV logger

---

## Status

- [x] Environment setup (PlatformIO, Wokwi)
- [x] DHT22 firmware and wiring simulated in Wokwi
- [x] MQTT publishing and Python subscriber
- [x] Wi-Fi reconnect logic
- [x] Credentials removed from the code (`secrets.h` is git-ignored)
- [x] MQTT broker users: read-only dashboard user, separate device user
- [ ] TLS with CA certificate, watchdog, PMS7003 reading on every loop
      (build passes; live broker and hardware tests still to do)
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

**Week 4 — Reliability:** Wi-Fi reconnect, sensor accuracy test, system
summary, demo video, GitHub Issues for Month 2.

## Month 2 (weeks 5–8)

- Added the PMS7003 particulate sensor (UART) next to the DHT22.
- Moved from a local Mosquitto broker to HiveMQ Cloud with TLS (port 8883).
- Built a web dashboard (WebSockets) for GitHub Pages.
- Python logger (`mqtt_logger.py`) that saves MQTT data to CSV.
- Reliability and security work on the firmware (in progress, see Status).

---

## Security

- Credentials live in `secrets.h`, which is git-ignored. Only
  `secrets.example.h` is in the repo.
- The MQTT broker has two users: the device (publish and subscribe) and
  a dashboard user that can only subscribe. The dashboard password is
  visible in the browser by design, so that user cannot publish.
- The logger reads its login from environment variables.
- Next: CA certificate check in the firmware (instead of skipping
  verification) and a password for the Wi-Fi setup portal.

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
- Why passwords must never be committed to a public repo, and how to
  rotate them when one leaks.
- Why a read-only user matters when a password is visible in a browser.

---

## Contact
amaniarouri636@gmail.com
