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



 
const char* ntpServer = "129.6.15.28";
const long utcOffsetInSeconds = -3 * 3600;

WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, ntpServer, utcOffsetInSeconds);

int rotate = 3;
String fullDate;
String ssid = "InternetSAX";
String password = "cadebabaca";
String  username, botname;
bool conectadoweb = false;

unsigned long previousMillisCLOCK = 0;  // Armazena o último tempo em que a função foi executada
const long intervalCLOCK = 1000;        // Intervalo de 1 segundo (1000 milissegundos)


void setup() {
  Serial.begin(115200);
  
  tft.init();
  tft.setRotation(rotate);
  tft.fillScreen(TFT_BLACK);
  
  setupWEB();
  delay(500);

  timeClient.begin();
  timeClient.update();
   delay(500);

  UPDATETIME();



  drawDate(fullDate); 
  drawClock(); 
}



void loop() {
  unsigned long currentMillis = millis();  // Captura o tempo atual

  if (currentMillis - previousMillisCLOCK >= intervalCLOCK) {
    previousMillisCLOCK = currentMillis;  // Atualiza o tempo
    drawClock();  // Chama a função para desenhar o relógio a cada 1 segundo
  }

}
