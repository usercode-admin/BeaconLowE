# BeaconLowE
**ESP32 Beacon Frame Generator & Wi-Fi Research Tool**

---

## Overview

**BeaconLowE** is an ESP32-based firmware project built with `ESPAsyncWebServer`. It provides a responsive web interface to configure and transmit IEEE 802.11 Beacon frames for wireless network research, educational study, and authorized penetration testing.

### Key Capabilities
* **SSID Prefix Customization:** Define custom base names for broadcasted networks.
* **Channel Switching:** Dynamically select and change Wi-Fi transmission channels.
* **Browser-Based Control:** Start and stop frame broadcasting directly from the web dashboard.
* **Dynamic Beacon Generation:** Continuously broadcast multiple beacon frames with rotating SSIDs.
* **Web-Based Management:** Configure and monitor device parameters in real time.

---

## Features

* **Intuitive Web UI:** Clean, responsive interface accessible via any modern browser.
* **ESP32 SoftAP:** Operates in Access Point mode for standalone configuration without external routers.
* **Asynchronous Web Server:** Powered by `ESPAsyncWebServer` for non-blocking, high-performance handling.
* **802.11 Beacon Frame Generator:** Constructs and injects raw management frames.
* **MAC Address Randomization:** Generates randomized source MAC addresses for each simulated AP.
* **Channel Hopping / Switching:** Supports switching across standard 2.4 GHz Wi-Fi channels.
* **Custom SSID Prefixing:** Allows structured naming conventions during testing.
* **Lightweight & Fast:** Optimized for minimal memory footprint and rapid frame transmission.

---

## Project Structure

```text
.
├── Beacon_fld_n16r8
└── README.md
```

---

## Requirements

### Hardware
* ESP32 Development Board

### Software & Libraries
* Arduino IDE or PlatformIO
* ESP32 Board Package (Espressif Systems)
* `ESPAsyncWebServer`
* `AsyncTCP` (Dependency for `ESPAsyncWebServer` on ESP32)
* Built-in `WiFi` Library

---

## Purpose

This project is designed strictly for technical education and authorized security research:
* Studying the **IEEE 802.11** wireless networking standard.
* Analyzing the structure and behavior of **802.11 Management (Beacon) Frames**.
* Learning low-level network programming on the **ESP32** architecture.
* Practicing embedded Wi-Fi application development.
* Conducting wireless assessments within isolated lab environments or networks you explicitly own and manage.

---

## ⚠️ Warning: Educational Use Only

**Do not** deploy or operate this tool on public networks or third-party infrastructure without explicit written authorization. Unauthorized transmission of raw beacon frames can:
* Cause severe radio frequency (RF) interference and disrupt legitimate Wi-Fi networks.
* Degrade connectivity and user experience for nearby wireless clients.
* Violate institutional or corporate Acceptable Use Policies (AUP).
* Constitute an illegal act under local and international telecommunications laws.

---

## Disclaimer

The author provides this source code strictly for **educational and research purposes**. The end user assumes full legal and operational responsibility for compiling, deploying, and executing this software. 

The author shall not be held liable for any direct, indirect, or consequential damages resulting from the use or misuse of this project, including but not limited to:
* Loss or corruption of data.
* Hardware or equipment damage.
* Network downtime or service disruption.
* Violations of applicable laws, regulations, or network policies.

---

## License

This project is released exclusively for **Education, Research, and Learning**. Please use this software responsibly and in full compliance with all applicable laws.

---

## Acknowledgements

* [Espressif Systems](https://espressif.com)
* [Arduino](https://arduino.cc)
* [ESPAsyncWebServer](https://github.com)
* IEEE 802.11 Standard Documentation

---

⭐ **If you find this project useful for your research, please consider giving it a Star!**
