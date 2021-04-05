#ifndef MINI_WIFI_H
#define MINI_WIFI_H

#include <Arduino.h>

class MiniWifi {
 private:
  const char* hostName;
  const char* wifiSsid;
  const char* wifiPass;
  uint16_t wifiWaitDelay;
  Stream* debugStream;
  uint8_t wifiWaitRetries;
  boolean _isEnabled;
 public:
  MiniWifi(const char* hostName, const char* wifiSsid, const char* wifiPass);

  inline void MiniWifi::setWifiWaitDelay(uint16_t delay) { wifiWaitDelay = delay; }
  inline void MiniWifi::setDebugStream(Stream *stream) { debugStream = stream; }
  inline void MiniWifi::setWifiWaitRetries(uint8_t retries) { wifiWaitRetries = retries; }

  void disableWiFi();
  inline bool MiniWifi::isEnabled() { return _isEnabled; }
  inline bool MiniWifi::isConnected() { return WiFi.status() == WL_CONNECTED; }
  void joinWifi();

  void checkWifi();
  int32_t getSignalStrength();
  uint8_t getSignalQuality();

  void createWifi();

  void begin();
  int get(const char* url, char* resultBuf, int resultBufLen);
};



#endif