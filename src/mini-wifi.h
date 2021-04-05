#ifndef MINI_WIFI_H
#define MINI_WIFI_H

#include <Arduino.h>

class MiniWifi {
 private:
  const char* hostName;
  const char* wifiSsid;
  const char* wifiPass;
  Stream* debugStream;
  uint16_t wifiWaitDelay;
  uint8_t wifiWaitRetries;
  boolean _isEnabled;
 public:
  MiniWifi(const char* hostName, const char* wifiSsid, const char* wifiPass);

  void setWifiWaitDelay(uint16_t delay) { wifiWaitDelay = delay; }
  void setDebugStream(Stream *stream) { debugStream = stream; }
  void setWifiWaitRetries(uint8_t retries) { wifiWaitRetries = retries; }

  void disableWiFi();
  bool isEnabled() { return _isEnabled; }
  bool isConnected() { return WiFi.status() == WL_CONNECTED; }
  void joinWifi();

  void checkWifi();
  int32_t getSignalStrength();
  uint8_t getSignalQuality();

  void createWifi();

  void begin();
  int get(const char* url, char* resultBuf, int resultBufLen);
};



#endif