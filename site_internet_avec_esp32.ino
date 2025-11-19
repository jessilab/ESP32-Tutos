#include <WiFi.h>
#include <WebServer.h>

WebServer server(80);
const int LED = 2;

IPAddress local_IP(192, 168, 4, 1);
IPAddress gateway(192, 168, 4, 1);
IPAddress subnet(255, 255, 255, 0);

void handleRoot() {
  String page =
    "<!DOCTYPE html>"
    "<html lang='fr'>"
    "<head>"
      "<meta charset='UTF-8' />"
      "<meta name='viewport' content='width=device-width, initial-scale=1.0' />"
      "<title>ESP32 Control</title>"
      "<style>"
        "body { font-family: Arial; text-align: center; background:#f4f4f4; padding-top:40px; }"
        "h1 { color:#333; margin-bottom:30px; }"
        "button {"
          "background:#ff8ec7;"
          "border:none;"
          "padding:15px 40px;"
          "margin:10px;"
          "font-size:20px;"
          "border-radius:10px;"
          "cursor:pointer;"
          "color:white;"
          "font-weight:bold;"
          "box-shadow:0 4px 8px rgba(0,0,0,0.2);"
          "transition:0.2s;"
        "}"
        "button:hover { transform:scale(1.05); }"
      "</style>"
    "</head>"
    "<body>"
      "<h1>✨ Contrôle la LED ✨</h1>"
      "<button onclick=\"fetch('/on')\">Allumer</button>"
      "<button onclick=\"fetch('/off')\">Éteindre</button>"
      "<p style='margin-top:20px; color:#888;'>Hébergé directement dans l'ESP32</p>"
    "</body>"
    "</html>";

  server.send(200, "text/html", page);
}

void setup() {
  pinMode(LED, OUTPUT);
  digitalWrite(LED, LOW);

  Serial.begin(115200);

  if (!WiFi.softAPConfig(local_IP, gateway, subnet)) {
    Serial.println("AP Config Failed");
  }

  WiFi.softAP("JessiLab", "12345678");

  Serial.print("AP IP address: ");
  Serial.println(WiFi.softAPIP());

  delay(500);

  server.on("/", handleRoot);

  server.on("/on", []() {
    digitalWrite(LED, HIGH);
    server.send(200, "text/plain", "LED ON");
  });

  server.on("/off", []() {
    digitalWrite(LED, LOW);
    server.send(200, "text/plain", "LED OFF");
  });

  server.begin();
  Serial.println("HTTP server started");
}

void loop() {
  server.handleClient();
}
