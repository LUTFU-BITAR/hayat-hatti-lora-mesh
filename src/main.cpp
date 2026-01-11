#include <Arduino.h>
#include "lora_mesh.h"
#include "wifi_portal.h"

unsigned long lastSend = 0;

void setup() {
  Serial.begin(115200);
  delay(1000);

  Serial.println("Hayat Hatti node starting...");

  meshInit(1);
  wifiPortalInit();
}

void loop() {
  meshReceive();
  meshHandleRetries();
  wifiPortalHandle();

  if (millis() - lastSend > 15000) {
    lastSend = millis();
    meshSend("Periodic mesh heartbeat");
  }
}
