#include <WiFi.h>
#include <WebServer.h>
#include "wifi_portal.h"
#include "lora_mesh.h"

const char* AP_SSID = "HayatHatti";
const char* AP_PASS = "";

WebServer server(80);

void handleRoot() {
  String page =
    "<html><body>"
    "<h2>Hayat Hatti</h2>"
    "<form action='/send' method='POST'>"
    "<input name='msg' length=64>"
    "<input type='submit' value='Send'>"
    "</form>"
    "</body></html>";

  server.send(200, "text/html", page);
}

void handleSend() {
  if (server.hasArg("msg")) {
    String msg = server.arg("msg");
    meshSend(msg.c_str());
  }

  server.send(200, "text/html",
              "<html><body>Message sent.</body></html>");
}

void wifiPortalInit() {
  WiFi.mode(WIFI_AP);
  WiFi.softAP(AP_SSID, AP_PASS);

  server.on("/", HTTP_GET, handleRoot);
  server.on("/send", HTTP_POST, handleSend);

  server.begin();

  Serial.println("Wi-Fi captive portal started");
}

void wifiPortalHandle() {
  server.handleClient();
}
