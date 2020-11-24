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
#include "ActionHandler.h"
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
  webserver.begin();
  SPIFFS.begin();
  pushAllFilesToJson();
  delay(1000);
  if(!error){
      getButtonJSON();
      getConfigJSON();
      configJson = configDoc.as<JsonObject>();
      buttons = doc.as<JsonObject>();
      client.begin(configJson["socketHost"].as<char*>(), configJson["socketPort"].as<int>(), path);
      setCurrentBoard();
      interateOverButtonsOnPage();  
  }else{
    tft.setTextColor(TFT_RED, TFT_WHITE);
    tft.println("An error has occured, please fix the error and restart the device.");
    tft.println("The web-interface is available.");
  }
  initiateAPI();
}

void loop() {
   if(configJson.containsKey("development")){
    if(configJson["development"].as<bool>()){
      if(analogRead(AD_PIN) > 20){
        Serial.println(analogRead(AD_PIN));
      }
      
    }
    else{
      getButton();   
    }
  }
  else{
    getButton(); 
  } 
   
  delay(100);
  client.loop();
  webserver.handleClient();
  tft.setTextColor(TFT_BLACK, TFT_WHITE);
}









  
