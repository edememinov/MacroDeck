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
#include <ElegantOTA.h>
#include <PubSubClient.h>

#include "SysVariables.h"
#include "ScreenHandler.h"
#include "CSS.h"
#include "FileHandler.h"
#include "ExecutionHandler.h"
#include "MqttHandler.h"
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
  ElegantOTA.begin(&webserver); 
  webserver.begin();
  SPIFFS.begin();
  readLogTimeAlive();
  if(aliveSince.as<int>() < 7000){
    setErrorToTrue();
    tft.println("Boot loop detected please check all txt files");
  }
  pushAllFilesToJson();
  delay(1000);
  if (!error) {
    writeLogTimeAlive();
    getConfigJSON();
    configJson = configDoc.as<JsonObject>();

    if (configJson.containsKey("webserverOnly")) {
      if (configJson["webserverOnly"].as<bool>()) {
        tft.println("Device is in webserver only mode");
      }
      else{
        
        getButtonJSON();   
        getButtonConfigJSON();
        buttons = doc.as<JsonObject>();
        if(!buttons.isNull()){
          buttonConfigJson = buttonConfigDoc.as<JsonObject>();
          client.begin(configJson["socketHost"].as<char*>(), configJson["socketPort"].as<int>(), path);
          setCurrentBoard();
          interateOverButtonsOnPage();
        }
        
      }
    }
    else{
      tft.println("webserverOnly bool is missing. Please update the configuration through the app");
    }
    
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
  if (configJson.containsKey("development")) {
    if (configJson["development"].as<bool>()) {
      clearScreen();
      tft.println("Device is in development mode");
    }
  }
  else{
    tft.println("calibrationMode bool is missing. Please update the configuration through the app");
  }
}

void loop() {

  if(millis() > 8000){
    if(millis() < 8100){
      writeLogTimeAlive();
    }
  }

  if (configJson.containsKey("development")) {
    if (configJson["development"].as<bool>()) {
      if(analogRead(AD_PIN) > buttonConfigJson["defaultValue"].as<int>() + 20){
        tft.println(analogRead(AD_PIN));
      }
      if(analogRead(AD_PIN) >= buttonConfigJson["buttons"][15]["minValue"].as<int>() && analogRead(AD_PIN) <= buttonConfigJson["buttons"][15]["maxValue"].as<int>()){
        delay(2000);
        clearScreen();
      }
    }
    else {
      if(configJson.containsKey("webserverOnly")){
        if(configJson["webserverOnly"].as<bool>()){
          //Do completely nothing
        }
        else{
           handleButtonPress();
        }
      }
      else{
         tft.println("webserverOnly is missing. Please update the configuration through the app");
      }
      

    }
  }
  else {
    tft.println("development is missing. Please update the configuration through the app");
  }
  delay(100);
  client.loop();
  webserver.handleClient();
  tft.setTextColor(TFT_BLACK, TFT_WHITE);
  
}
