#include <Arduino.h>
#include <SocketIoClient.h>
#include <ESP8266WiFi.h> //https://github.com/esp8266/Arduino
#include <DNSServer.h>
#include <ESP8266WebServer.h>
#include <WiFiManager.h> //https://github.com/tzapu/WiFiManager
#include <ESP8266HTTPClient.h>
#include <TFT_eSPI.h>
#include <SPI.h>
#include <ArduinoJson.h>

#include "SysVariables.h"
#include "ScreenHandler.h"
#include "CSS.h"
#include "FileHandler.h"
#include "ExecutionHandler.h"
#include "ButtonHandler.h"
#include "SocketHandler.h"
#include "WebPortal.h"


void setup() {
  Serial.begin(115200);
  delay(50);
  WiFi.mode(WIFI_STA);
  wifiManager.setTimeout(180);
  wifiManager.autoConnect("MacroBoardWifiConnect");
  initiateScreen();
  showIpAddress();
  initiateSocket();
  //webserver.enableCORS(true);
  webserver.begin();
  SPIFFS.begin();
  pushAllFilesToJson();
  delay(1000);
  if (!error) {
    getButtonJSON();
    getConfigJSON();
    getButtonConfigJSON();
    configJson = configDoc.as<JsonObject>();
    buttons = doc.as<JsonObject>();
    buttonConfigJson = buttonConfigDoc.as<JsonObject>();
    client.begin(configJson["socketHost"].as<char*>(), configJson["socketPort"].as<int>(), path);
    setCurrentBoard();
    interateOverButtonsOnPage();
  } else {
    tft.println("An error has occured, please fix the error and restart the device.");
    tft.println("The web-interface is available.");
  }
  initiateAPI();
  if (configJson.containsKey("calibrationMode")) {
    if (configJson["calibrationMode"].as<bool>()) {
      clearScreen();
      calibrateButtons(analogRead(AD_PIN));
      showCalibrationComplete();
      delay(2000);
    }
  }
}

void loop() {
  if (configJson.containsKey("development")) {
    if (configJson["development"].as<bool>()) {
      Serial.println(analogRead(AD_PIN));
    }
    else {
      handleButtonPress();

    }
  }
  else {
    handleButtonPress();
  }

  delay(100);
  client.loop();
  webserver.handleClient();
  tft.setTextColor(TFT_BLACK, TFT_WHITE);
}
