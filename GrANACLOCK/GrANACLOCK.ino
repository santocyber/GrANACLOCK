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
#include <ArduinoJson.h> // Biblioteca para manipulação de JSON
#include <Wire.h>
#include <time.h> 
#include <WiFi.h>
#include <WiFiUdp.h>
#include <NTPClient.h>
#include <HTTPClient.h>
#include <ESPping.h>
#include "User_Setup.h"
#include <TFT_eSPI.h>
#include <SPI.h>

TFT_eSPI tft = TFT_eSPI(); 


//####################TASKHANDLES
TaskHandle_t fotobdTASK;

 
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
bool timerfotostatus = false;
String estado = "crypto";

int16_t posX; // Posição horizontal do texto
String message = "obrigado pela preferencia...";


unsigned long previousMillisCLOCK = 0;  // Armazena o último tempo em que a função foi executada
const long intervalCLOCK = 1000;        // Intervalo de 1 segundo (1000 milissegundos)

unsigned long previousMillisSCROLL = 0;  // Armazena o último tempo em que a função foi executada
const long intervalSCROLL = 500;        // Intervalo de 1 segundo (1000 milissegundos)

unsigned long previousMillisCRYPTO = 0;  // Armazena o último tempo em que a função foi executada
const long intervalCRYPTO = 20000;        // Intervalo de 1 segundo (1000 milissegundos)


unsigned long previousMillisFOTO = 0;  // Armazena o último tempo em que a função foi executada
const long intervalFOTO = 600000;        // Intervalo de 1 segundo (1000 milissegundos)


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
   posX = tft.width();  // Inicia o texto fora da tela à direita

}



void loop() {
  unsigned long currentMillis = millis();  // Captura o tempo atual

if (estado == "scroll"){

 if (currentMillis - previousMillisSCROLL >= intervalSCROLL) {
    previousMillisSCROLL = currentMillis;  // Atualiza o tempo
    scrollText();  // Chama a função para desenhar o relógio a cada 1 segundo
  }

}
if (estado == "crypto"){

 if (currentMillis - previousMillisCRYPTO >= intervalCRYPTO) {
    previousMillisCRYPTO = currentMillis;  // Atualiza o tempo
    bolsa();  // Chama a função para desenhar o relógio a cada 1 segundo
  }

}

if (estado == "clock"){

  if (currentMillis - previousMillisCLOCK >= intervalCLOCK) {
    previousMillisCLOCK = currentMillis;  // Atualiza o tempo
    drawClock();  // Chama a função para desenhar o relógio a cada 1 segundo
  }
}
  
  if (currentMillis - previousMillisFOTO >= intervalFOTO) {
    previousMillisFOTO = currentMillis;  // Atualiza o tempo
    capturaimagemenviabd();  // Chama a função para desenhar o relógio a cada 1 segundo
    //xTaskCreatePinnedToCore(capturaimagemenviabd, "capturaimagemenviabd", 6000, NULL, 1, &fotobdTASK, 0);
  }




}
