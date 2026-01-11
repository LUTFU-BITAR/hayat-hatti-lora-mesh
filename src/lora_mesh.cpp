#include "lora_mesh.h"
#include <SPI.h>

static uint8_t NODE_ID = 0;
static uint32_t packetCounter = 0;

static MeshPacket lastPacket;
static bool waitingAck = false;
static uint8_t retryCount = 0;
static unsigned long lastSendTime = 0;

void meshInit(uint8_t nodeId) {
  NODE_ID = nodeId;

  Serial.println("Initializing mesh layer...");

  SPI.begin(18, 19, 23, 5);

  if (!LoRa.begin(433E6)) {
    Serial.println("LoRa init failed");
    return;
  }

  Serial.println("LoRa init OK");
}

bool meshSend(const char* message) {
  MeshPacket pkt;
  pkt.packetId = ++packetCounter;
  pkt.sourceId = NODE_ID;
  pkt.ttl = DEFAULT_TTL;
  pkt.hops = 0;
  pkt.type = PACKET_DATA;

  strncpy(pkt.payload, message, MAX_PAYLOAD_LEN);
  pkt.payload[MAX_PAYLOAD_LEN - 1] = '\0';

  lastPacket = pkt;
  waitingAck = true;
  retryCount = 0;
  lastSendTime = millis();

  Serial.print("Sending DATA packet ");
  Serial.println(pkt.packetId);

  LoRa.beginPacket();
  LoRa.write((uint8_t*)&pkt, sizeof(MeshPacket));
  LoRa.endPacket();

  return true;
}

void sendAck(uint32_t packetId) {
  MeshPacket ack;
  ack.packetId = packetId;
  ack.sourceId = NODE_ID;
  ack.ttl = 1;
  ack.hops = 0;
  ack.type = PACKET_ACK;
  ack.payload[0] = '\0';

  LoRa.beginPacket();
  LoRa.write((uint8_t*)&ack, sizeof(MeshPacket));
  LoRa.endPacket();

  Serial.print("ACK sent for packet ");
  Serial.println(packetId);
}

void meshReceive() {
  int packetSize = LoRa.parsePacket();
  if (!packetSize) return;

  MeshPacket incoming;
  LoRa.readBytes((uint8_t*)&incoming, sizeof(MeshPacket));

  if (incoming.type == PACKET_ACK) {
    if (waitingAck && incoming.packetId == lastPacket.packetId) {
      Serial.print("ACK received for packet ");
      Serial.println(incoming.packetId);
      waitingAck = false;
    }
    return;
  }

  if (incoming.sourceId == NODE_ID) return;

  if (incoming.ttl == 0) {
    Serial.println("Packet dropped (TTL expired)");
    return;
  }

  sendAck(incoming.packetId);

  incoming.ttl--;
  incoming.hops++;

  Serial.print("Forwarding DATA packet ");
  Serial.println(incoming.packetId);

  LoRa.beginPacket();
  LoRa.write((uint8_t*)&incoming, sizeof(MeshPacket));
  LoRa.endPacket();
}

void meshHandleRetries() {
  if (!waitingAck) return;

  if (millis() - lastSendTime < ACK_TIMEOUT) return;

  if (retryCount >= MAX_RETRY) {
    Serial.println("Packet delivery failed");
    waitingAck = false;
    return;
  }

  retryCount++;
  lastSendTime = millis();

  Serial.print("Retrying packet ");
  Serial.println(lastPacket.packetId);

  LoRa.beginPacket();
  LoRa.write((uint8_t*)&lastPacket, sizeof(MeshPacket));
  LoRa.endPacket();
}
