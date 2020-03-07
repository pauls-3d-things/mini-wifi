
#include "mini-wifi.h"
#include <ESP8266HTTPClient.h>
#include <ESP8266WiFi.h>

MiniWifi::MiniWifi(const char* hostName, const char* wifiSsid, const char* wifiPass) {
  this->hostName = hostName;
  this->wifiSsid = wifiSsid;
  this->wifiPass = wifiPass;
  this->wifiWaitDelay = 4000;
  this->debugStream = NULL;
  this->joinedWifi = false;
}

void MiniWifi::joinWifi() {
  joinedWifi = true;
  WiFi.hostname(hostName);
  WiFi.mode(WIFI_STA);
  do {
    if (debugStream != NULL) {
      debugStream->print("Connecting to WiFi ");
      debugStream->print(wifiSsid);
      debugStream->println("...");
    }
    WiFi.begin(wifiSsid, wifiPass);
    delay(wifiWaitDelay);
  } while (WiFi.status() != WL_CONNECTED);

  if (debugStream != NULL) {
    debugStream->print("Connected to WiFi ");
    debugStream->print("IP: ");
    debugStream->println(WiFi.localIP().toString());
  }
}

void MiniWifi::checkWifi() {
  if (!joinedWifi) {
    return;
  }

  if (debugStream != NULL) {
    debugStream->println("Checking WiFi...");
  }
  if (WiFi.status() != WL_CONNECTED) {
    joinWifi();
  }
  yield();
}

void MiniWifi::createWifi() {
  IPAddress Ip(192, 168, 1, 1);
  IPAddress NMask(255, 255, 255, 0);
  WiFi.mode(WIFI_AP);
  WiFi.softAPConfig(Ip, Ip, NMask);
  WiFi.softAP(wifiSsid, wifiPass);
  delay(wifiWaitDelay);

  if (debugStream != NULL) {
    debugStream->println("Started WIFI");
    debugStream->println(WiFi.softAPIP());
  }
}

void MiniWifi::setWifiWaitDelay(uint16_t delay) { wifiWaitDelay = delay; }
void MiniWifi::setDebugStream(Stream* stream) { debugStream = stream; }

int MiniWifi::get(const char* url, char* resultBuf, int resultBufLen) {
  int bytesRead = 0;

  HTTPClient http;
  http.begin(url);
  int httpCode = http.GET();
  delay(50);

  if (debugStream != NULL) {
    debugStream->print("GET ");
    debugStream->print(url);
    debugStream->print(" ");
    debugStream->print(http.getSize());
    debugStream->print(" ");
  }

  if (http.getSize()) {
    while (!http.getStream().available()) {
      // wait until data is here
      yield();
    }
  }

  if (httpCode > 0) {
    // note: http.getString() crashes when calling http.end(),
    // so I had to implement some stream reading manually.

    char buffer[1];
    while (http.getStream().available() && bytesRead < resultBufLen - 1) {
      http.getStream().readBytes(buffer, 1);
      resultBuf[bytesRead] = buffer[0];
      bytesRead++;
    }
    resultBuf[bytesRead] = 0;

    if (debugStream != NULL) {
      debugStream->print("\"");
      debugStream->print(resultBuf);
      debugStream->println("\"");
    }
  }

  http.end();
  return bytesRead;
}