


int touchCount = 0;



void bolsa() {
   Serial.println("EXECUTANDO BOLSA...");
   tft.setRotation(rotate);
   tft.setFreeFont(&FreeSansBold9pt7b);  // Fonte

   tft.fillScreen(TFT_BLACK); // Limpar a tela
   tft.setTextColor(TFT_YELLOW);
   tft.setTextSize(1);
   tft.setCursor(10, 0);
   tft.println("PEGANDO COTACAO...");


  // Exibir todos os valores de uma vez na tela
   tft.fillScreen(TFT_BLACK); // Limpar a tela
   tft.setTextColor(TFT_GREEN);
   tft.setTextSize(3);
   tft.setCursor(20, 0);

   
    String json = getCurrencyPrices();
    Serial.println("Raw JSON Data:");
    Serial.println(json);

    // Deserializar o JSON recebido
    JsonDocument doc;
    DeserializationError error = deserializeJson(doc, json);

    if (error) {
        Serial.print(F("Failed to parse JSON: "));
        Serial.println(error.c_str());
        return;
    }

    // Iterar sobre os valores e imprimir de forma formatada
    Serial.println("Formatted Currency Prices:");
    for (JsonPair kv : doc.as<JsonObject>()) {
        tft.print(kv.key().c_str());
        tft.print(": ");
        tft.println(kv.value().as<String>());
    }
   Serial.println(getCurrencyPrices());

   delay(10000);

 if (tft.getTouchRawZ() > 500) {
      Serial.println("Tela tocada. Saindo do loop.stopFetching");
      //functionExecuted = false;  // Marca a função como executada
      estado = "clock";
      touchCount++;
      delay(800);
    }

}

  



String getCurrencyPrices() {
    HTTPClient http;
    http.begin("https://52.54.32.237/granagrow/granagw/get_prices.php"); // Substitua pela URL do seu script PHP
    int httpCode = http.GET();
    if (httpCode != HTTP_CODE_OK) {
        Serial.println("Failed to get HTTP response.");
        return "ERRO INTERNET";
    }

    String response = http.getString();
    http.end();


    return response;

}
