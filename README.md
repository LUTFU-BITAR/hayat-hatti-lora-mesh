# Hayat Hattı – LoRa Mesh Emergency Communication System

Hayat Hattı is a low-cost, ESP32-based LoRa mesh communication system designed
for post-disaster scenarios where conventional communication infrastructure
is unavailable.

The system enables short text messages to be sent from mobile devices via a
local Wi-Fi access point and forwarded across a LoRa mesh network using a
store-and-forward approach.

---

## System Architecture

- ESP32 microcontroller
- LoRa (SX127x) long-range radio
- Wi-Fi Access Point for user interaction
- Multi-hop mesh forwarding with TTL control
- ACK-based packet reliability mechanism

---

## Key Features

- Decentralized LoRa mesh networking
- TTL-based packet forwarding
- ACK and retry mechanism for reliability
- Store-and-forward message propagation
- Wi-Fi captive portal for end users
- Modular and extensible architecture

---

## Packet Structure

Each packet contains:
- Packet ID
- Source Node ID
- TTL (Time To Live)
- Hop count
- Packet type (DATA / ACK)
- Payload

---

## Current Status

- Core mesh logic implemented
- ACK and retry mechanism implemented
- Wi-Fi captive portal implemented
- Code builds successfully on ESP32 (Arduino framework)

Hardware testing and field validation will be conducted once physical devices
are available.

---

## Development Environment

- PlatformIO
- ESP32 Arduino Framework
- LoRa library

---

## Planned Improvements

- Power management (deep sleep)
- Multi-node field testing
- Packet encryption
- Web interface improvements
