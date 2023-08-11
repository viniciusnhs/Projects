#include <OvO_otahtml.h>
#include <Update.h>
#include <SPIFFS.h>
#include <WebServer.h>
#include <ArduinoJson.h>
#include <WebSocketsServer.h>

WebServer server(80);
WebSocketsServer webSocket = WebSocketsServer(81);

void initSERVER();

void handleRoot();
void handlePage(const String &pageName, const String &contentType);
void handleImage(const String &imageName, const String &contentType);
void webSocketEvent(byte num, WStype_t type, uint8_t *payload, size_t length);

void initSERVER() {
  server.on("/", []() { handlePage("index.html", "text/html"); });
  server.on("/estacao", []() { handlePage("estacao.html", "text/html"); });
  server.on("/login", []() { handlePage("login.html", "text/html"); });
  server.on("/ota", []() { handlePage("ota.html", "text/html"); });

  server.on("/termometro.png", []() { handleImage("termometro.png", "image/png"); });
  server.on("/wifi.png", []() { handleImage("wifi.png", "image/png"); });
  server.on("/wifilevel.png", []() { handleImage("wifilevel.png", "image/png"); });
  server.on("/rain.png", []() { handleImage("rain.png", "image/png"); });
  server.on("/ovologo.jpeg", []() { handleImage("ovologo.jpeg", "image/jpeg"); });

  server.begin();
  webSocket.begin();
  webSocket.onEvent(webSocketEvent);
}

void handlePage(const String &pageName, const String &contentType) {
  File file = SPIFFS.open("/" + pageName, "r");
  if (file) {
    server.streamFile(file, contentType);
    file.close();
  } else {
    server.send(404, "text/plain", "Página não encontrada");
  }
}

void handleImage(const String &imageName, const String &contentType) {
  File file = SPIFFS.open("/" + imageName, "r");
  if (file) {
    server.streamFile(file, contentType);
    file.close();
  } else {
    server.send(404, "text/plain", "Imagem não encontrada");
  }
}

void webSocketEvent(byte num, WStype_t type, uint8_t *payload, size_t length) {
  if (type == WStype_DISCONNECTED) {
    Serial.println("Client " + String(num) + " disconnected");
  } else if (type == WStype_CONNECTED) {
    Serial.println("Client " + String(num) + " connected");
  } else if (type == WStype_TEXT) {
    StaticJsonDocument<200> doc;
    DeserializationError error = deserializeJson(doc, payload);
    if (error) {
      Serial.print(F("deserializeJson() failed: "));
      Serial.println(error.f_str());
      return;
    } else {
      const int restart = doc["status"];
      Serial.println("Status: " + String(restart));
      if (restart) {
        ESP.restart();
      }
    }
    Serial.println("");
  }
}
