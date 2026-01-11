#ifndef LORA_MESH_H
#define LORA_MESH_H

#include <Arduino.h>
#include <LoRa.h>

#define MAX_PAYLOAD_LEN 120
#define DEFAULT_TTL 5
#define MAX_RETRY 3
#define ACK_TIMEOUT 3000

enum PacketType {
  PACKET_DATA = 0,
  PACKET_ACK  = 1
};

struct MeshPacket {
  uint32_t packetId;
  uint8_t sourceId;
  uint8_t ttl;
  uint8_t hops;
  uint8_t type;
  char payload[MAX_PAYLOAD_LEN];
};

void meshInit(uint8_t nodeId);
bool meshSend(const char* message);
void meshReceive();
void meshHandleRetries();

#endif
