#include <FS.h>
#include <FFat.h> // Biblioteca para FSFAT
#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>


const char* serverStatus           = "https://52.54.32.237/granagrow/granagw/estado.php";
const char* serverMac              = "https://52.54.32.237/granagrow/granagw/mac.php";
String macAddress;



void checkLEDStatus(void *pvParameters) {
  while (1) {
  if (conectadoweb) {

  
  HTTPClient http;
  http.setTimeout(20000); // Aumenta o timeout para 20 segundos
  http.begin(serverStatus);
  http.addHeader("Content-Type", "application/x-www-form-urlencoded");

  macAddress = WiFi.macAddress();
  String httpRequestData = "mac_address=" + macAddress;
  int httpResponseCode = http.POST(httpRequestData);

  if (httpResponseCode > 0) {
    String payload = http.getString();
    Serial.println("Payload: " + payload);
    JsonDocument doc;
    deserializeJson(doc, payload);

    message = doc["message"].as<String>();
    estado  = doc["estado"].as<String>();
    saveLedStatusToSPIFFS(estado);
    Serial.println(estado);  
    
        if (estado == "clock") {
      Serial.println("CLOCK PELA API");
    }
        if (estado == "scroll") {
      Serial.println("scroll PELA API");
    }
        if (estado == "crypto") {
      Serial.println("ccrypto PELA API");
    }


  } else {
    Serial.print("Error oao acessar a API: ");
    Serial.println(httpResponseCode);
  }
  http.end();
}else {
    Serial.println("NAO CONECTADO");
          
}

  vTaskDelete(NULL); // Exclui a tarefa


}}






void saveLedStatusToSPIFFS(bool status) {
  fs::File file = FFat.open("/ledstatus.txt", FILE_WRITE);
  if(!file){
    Serial.println("Failed to open file for writing");
    return;
  }
  file.println(status);
  file.close();
}

void readLedStatusFromSPIFFS() {
  fs::File file = FFat.open("/ledstatus.txt", FILE_READ);
  if(!file){
    Serial.println("Failed to open file for reading");
    return;
  }
  String status = file.readStringUntil('\n');
  estado = status;
  Serial.print("LED status read from SPIFFS: ");
 
  file.close();
}











void sendMACAddress(void *pvParameters) {
  while (1) {

  if (conectadoweb) {


  // Criar um cliente HTTP
  HTTPClient http;

  String macAddress = WiFi.macAddress();
  Serial.println("MAC Address: " + macAddress);

  http.begin(serverMac);

  // Definir o tipo de conteúdo
  http.addHeader("Content-Type", "application/x-www-form-urlencoded");

  // Construir os dados a serem enviados
  String httpRequestData = "mac_address=" + macAddress;

  // Enviar a requisição POST
  int httpResponseCode = http.POST(httpRequestData);

  // Checar a resposta da requisição
  if (httpResponseCode > 0) {
    String response = http.getString();
    Serial.println(httpResponseCode);
    Serial.println(response);
  } else {
    Serial.print("Erro na requisição POST: ");
    Serial.println(httpResponseCode);
  }

  // Finalizar a conexão
  http.end();

}else {
    Serial.println("NAO CONECTADO");
          
}

  vTaskDelete(NULL); // Exclui a tarefa

}}
