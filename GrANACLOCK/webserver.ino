#include <WiFi.h>
#include <FS.h>
#include <FFat.h> // Biblioteca para FSFAT
#include "WebServer.h"
#include <ArduinoJson.h> // Biblioteca para manipulação de JSON
#include <WiFiMulti.h>

WiFiMulti wifiMulti;

WebServer server(80);


#define CONFIG_FILE "/wifi_config.txt"

String networksList;
bool loopweb = false;
unsigned long previousMillis = 0;        // Armazena o último tempo em que a verificação foi feita


void scanNetworks() {
  Serial.println("Scanning WiFi networks");
  loopweb = true;
  WiFi.disconnect(true); // Desativa a reconexão automática

  int n = WiFi.scanNetworks();
  DynamicJsonDocument jsonBuffer(1000);
  JsonArray networks = jsonBuffer.createNestedArray("networks");

  for (int i = 0; i < n && i < 5; i++) {
    JsonObject network = networks.createNestedObject();
    network["ssid"] = WiFi.SSID(i);
    network["rssi"] = WiFi.RSSI(i);
  }

  networksList = "";
  serializeJson(jsonBuffer, networksList);
  Serial.println(networksList);
  WiFi.disconnect(true); // Desativa a reconexão automática
}

void setupWEB() {
  Serial.println("INICIANDO CONFIG WiFi");
  loadCredentials();
  WiFi.disconnect(true); // Desativa a reconexão automática
  delay(100);

  int reconnectAttempts = 0;
  const int maxReconnectAttempts = 5;
  while (reconnectAttempts < maxReconnectAttempts) {
    if (ssid.length() > 0) {

      Serial.println("SSID PASS: ");
      Serial.println(ssid.c_str());
      Serial.println(password.c_str());
      
      WiFi.begin(ssid, password);
      unsigned long startAttemptTime = millis();
      const unsigned long wifiTimeout = 800; // Tempo limite para tentar conectar ao WiFi (10 segundos)
      Serial.println("TENTANDO CONECTAR ");
      Serial.println(reconnectAttempts);
     
      tft.fillScreen(TFT_BLACK);
      tft.setTextColor(TFT_RED, TFT_BLACK);
      tft.setTextSize(2); // Tamanho menor
      tft.setFreeFont(&FreeSansBold9pt7b);
      tft.drawString("TENTANDO CONECTAR ...", 10, 10); 
      String reconnectAttemptsStr = String(reconnectAttempts);
      tft.drawString(reconnectAttemptsStr, 10, 50); 

      

      while (WiFi.status() != WL_CONNECTED && (millis() - startAttemptTime) < wifiTimeout) {
        esp_task_wdt_reset();  // Reseta o watchdog timer antes de desativar
        delay(100);
        Serial.print(".");
      }

      if (WiFi.status() == WL_CONNECTED) {
        Serial.println("CONECTADO AO WIFI");
        Serial.print("IP Address: ");
        Serial.println(WiFi.localIP());

        conectadoweb = true;
        loopweb = true;

        return; // Conexão bem-sucedida, sai da função
      } else {
        Serial.println("\nFailed to connect to WiFi");
        conectadoweb = false;
        loopweb = true;
      }
    } else {
      Serial.println("No WiFi credentials saved");
      conectadoweb = false;
      loopweb = true;
    }

    reconnectAttempts++;
  }

  if (reconnectAttempts >= maxReconnectAttempts) {
    Serial.println("Exceeded maximum reconnect attempts.");
    setupAP();
    conectadoweb = false;
    loopweb = true;
  }

  if (!FFat.begin(true)) {
    Serial.println("An error has occurred while mounting FSFAT");
    return;
  }
}

void setupAP() {
  WiFi.mode(WIFI_AP_STA);
  WiFi.disconnect(true); // Desativa a reconexão automática
  delay(200);

  WiFi.softAP("GrANA-AP");
  IPAddress apIP = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Serial.println(apIP);

      tft.fillScreen(TFT_BLACK);
      tft.setTextColor(TFT_CYAN, TFT_BLACK);
      tft.setTextSize(2); // Tamanho menor
      tft.setFreeFont(&FreeSansBold9pt7b);
      tft.println("NAO CONECTADO"); 
      tft.println("ACESSE O IP:"); 
      tft.println(apIP); 
      tft.println("ACESSE A REDE :"); 
      tft.println("GrANA-AP"); 






  setupWebServer();
  loopweb = true;
}

void setupWebServer() {
  server.on("/", HTTP_GET, handleRoot);
  server.on("/save", HTTP_POST, handleSave);
  server.on("/delete", HTTP_GET, handleDeleteCredentials);
  server.on("/restart", HTTP_GET, restartEsp); 
  server.on("/scan", HTTP_GET, scanNetworks);
  server.begin();
}

void loopWEB() {
  if (loopweb) {
    server.handleClient();
  }

  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= 300000) {
    previousMillis = currentMillis;

    if (WiFi.status() == WL_CONNECTED) {
      Serial.println("\nConnected to WiFi");
      Serial.print("IP Address: ");
      Serial.println(WiFi.localIP());
      tft.fillScreen(TFT_BLACK);
      tft.setTextColor(TFT_YELLOW, TFT_BLACK);
      tft.setTextSize(2); // Tamanho menor
      tft.setFreeFont(&FreeSansBold9pt7b);
      tft.println("SSID e PASS:"); 
      tft.println(ssid); 
      tft.println(password); 
      tft.println(WiFi.localIP()); 

      conectadoweb = true;
      loopweb = false;
    } else {
      Serial.println("\nFailed to connect to WiFi");
      conectadoweb = false;
      loopweb = true;
    }
  }
}

void loadCredentials() {
  if (!FFat.begin(true)) {
    Serial.println("An error has occurred while mounting FSFAT");
    return;
  }

  fs::File file = FFat.open(CONFIG_FILE, FILE_READ);
  if (!file) {
    Serial.println("Failed to open config file for reading");
    return;
  }

  ssid = file.readStringUntil('\n');
  password = file.readStringUntil('\n');
  username = file.readStringUntil('\n');
  botname = file.readStringUntil('\n');

  ssid.trim();
  password.trim();
  username.trim();
  botname.trim();

  file.close();
}

void saveCredentials(const char* ssid, const char* password, const char* username, const char* botname) {
  fs::File file = FFat.open(CONFIG_FILE, FILE_WRITE);
  if (!file) {
    Serial.println("Failed to open config file for writing");
    return;
  }

  file.println(ssid);
  file.println(password);
  file.println(username);
  file.println(botname);
  file.close();
}

void deleteCredentials() {
  if (FFat.remove(CONFIG_FILE)) {
    Serial.println("WiFi credentials deleted");
  } else {
    Serial.println("Failed to delete WiFi credentials");
  }
}

void restartEsp() {
  server.send(200, "text/plain", "restarting...");
  delay(1000);
  ESP.restart();
}

void handleRoot() {
  String html = "<!DOCTYPE html>"
                "<html>"
                "<head>"
                "<title>GrANA WiFi Config</title>"
                "<style>"
                "body { font-family: Arial, sans-serif; text-align: center; background-color: black; color: white; }"
                "input[type='checkbox'], input[type='password'], input[type='text'] { width: 50%; padding: 10px; margin: 10px 0; }"
                "button { padding: 5px 10px; font-size: 10px; margin: 10px 5px; }"
                "table { margin: 20px auto; border-collapse: collapse; width: 60%; }"
                "th, td { border: 1px solid white; padding: 10px; }"
                "th { background-color: #333; }"
                "</style>"
                "</head>"
                "<body>"
                "<h1>GrANA WiFi Config</h1>"
                "<table>"
                "<thead>"
                "<tr>"
                "<th>Select</th>"
                "<th>SSID</th>"
                "<th>Signal Strength</th>"
                "</tr>"
                "</thead>"
                "<tbody>";
  html += "<form method='post' action='/save'>";

  // Adicionando as redes WiFi à tabela
  DynamicJsonDocument jsonBuffer(1000);
  deserializeJson(jsonBuffer, networksList);
  JsonArray networks = jsonBuffer["networks"].as<JsonArray>();
  int n = 5; // Limitar a 5 redes

  for (int i = 0; i < n && i < networks.size(); i++) {
    html += "<tr>";
    html += "<td><input type='checkbox' name='ssid' value='" + networks[i]["ssid"].as<String>() + "'></td>";
    html += "<td>" + networks[i]["ssid"].as<String>() + "</td>";
    html += "<td>" + String(networks[i]["rssi"].as<int>()) + " dBm</td>";
    html += "</tr>";
  }
  html += "</tbody>"
          "</table>";
  html += "<label for='password'>Password:</label><input type='text' name='password'><br>";
  html += "<label for='nomedobot'>Nome BOT:</label><input type='text' name='nomedobot'><br>";
  html += "<label for='usuario'>Usuario:</label><input type='text' name='usuario'><br>";
  html += "<input type='submit' value='CONECTAR'>";
  html += "</form>";
  html += "<a href='/delete'><button>Delete WiFi</button></a>"
          "<a href='/restart'><button>Restart</button></a>"
          "<a href='/scan'><button>Scan</button></a>"
          "</body>"
          "</html>";

  server.send(200, "text/html", html);
}

void handleSave() {
  ssid = server.arg("ssid");
  password = server.arg("password");
  username = server.arg("username");
  botname = server.arg("botname");

  ssid.trim();
  password.trim();
  username.trim();
  botname.trim();

  saveCredentials(ssid.c_str(), password.c_str(), username.c_str(), botname.c_str());

  server.send(200, "text/plain", "Credentials saved, restarting...");
  delay(1000);
  ESP.restart();
}

void handleDeleteCredentials() {
  if (FFat.remove(CONFIG_FILE)) {
    server.send(200, "text/plain", "WiFi credentials deleted, restarting...");
    delay(1000);
    ESP.restart();
  } else {
    server.send(500, "text/plain", "Failed to delete WiFi credentials");
  }
}
