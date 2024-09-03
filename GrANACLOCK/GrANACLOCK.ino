//############SantoCyber###############
//###                                 #
//### GRANACLOCK                      #
//###                                 #
//### BY HUGO AB                      #
//### SANTOCYBER                      #
//### http://GITHUB.COM/SANTOCYBER    #
//###                                 #
//#####################################



#include <esp_system.h>  
#include <esp_task_wdt.h> // Inclui a biblioteca para manipulação do watchdog timer 
#include <Arduino.h>
#include <Wire.h>
#include <time.h> 
#include <WiFi.h>
#include <WiFiUdp.h>
#include <NTPClient.h>
#include "User_Setup.h"
#include <TFT_eSPI.h>
#include <SPI.h>

TFT_eSPI tft = TFT_eSPI(); 



 
String ssid = "InternetSAX";
String password = "cadebabaca";
const char* ntpServer = "129.6.15.28";
const long utcOffsetInSeconds = -3 * 3600;

WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, ntpServer, utcOffsetInSeconds);

int rotate = 3;
String fullDate;
String  username, botname;
bool conectadoweb = false;

unsigned long currentMillis = millis();


void setup() {
  Serial.begin(115200);
  
  tft.init();
  tft.setRotation(rotate);
  tft.fillScreen(TFT_BLACK);
  
  WiFi.begin(ssid.c_str(), password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nConectado ao WiFi");

  timeClient.begin();
  timeClient.update();
    delay(500);

  UPDATETIME();
      delay(500);

  drawDate(fullDate); 
  drawClock(); 
}




void loop() {

 // UPDATETIME();

  drawClock();

  delay(1000); 
}
