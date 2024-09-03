
void drawDate(String fullDate) {
  tft.setTextColor(TFT_CYAN, TFT_BLACK);
  tft.setTextSize(1); // Tamanho menor
  tft.setFreeFont(&FreeSansBold9pt7b);
  tft.drawString(fullDate, 5, 5); 
}

int updateCounter = 0; // Variável global para o contador

void drawClock() {
      // Gerar cor hexadecimal aleatória e converter para RGB565
      uint16_t randomColor = tft.color565(random(256), random(256), random(256));

  if (updateCounter == 0) { // Atualizar a hora e os minutos apenas quando o contador for 0
     tft.fillScreen(TFT_BLACK);
    
    UPDATETIME();

    drawDate(fullDate); 

    String formattedTime = timeClient.getFormattedTime();
    // Gerar cor hexadecimal aleatória e converter para RGB565

    // Separar a hora e os segundos
    String hoursMinutes = formattedTime.substring(0, 5); // "HH:MM"
    String seconds = formattedTime.substring(6); // "SS"

    // Limpar a área da hora e dos minutos
    tft.fillRect(15, 75, 290, 60, TFT_BLACK); // Ajuste a largura e altura conforme necessário

    // Desenhar a hora e os minutos
    tft.setTextColor(randomColor, TFT_BLACK);
    tft.setTextSize(4);
    tft.setFreeFont(&FreeSerifBold24pt7b);
    tft.drawString(hoursMinutes, 15, 75); 

    // Limpar a área dos segundos
    tft.fillRect(380, 280, 60, 40, TFT_BLACK); // Ajuste a largura e altura conforme necessário

    // Gerar cor hexadecimal aleatória e converter para RGB565

    // Desenhar os segundos com a cor aleatória
    tft.setTextColor(randomColor, TFT_BLACK);
    tft.setTextSize(3);
    tft.setFreeFont(&FreeSerifBold12pt7b);
    tft.drawString(seconds, 380, 280);

    // Resetar o contador ao atingir 30
    updateCounter = 30;
  } else {
    // Apenas atualizar os segundos

    String formattedTime = timeClient.getFormattedTime();
    String seconds = formattedTime.substring(6); // Pega os "SS"

    // Limpar a área dos segundos
    tft.fillRect(360, 260, 100, 100, TFT_BLACK); // Ajuste a largura e altura conforme necessário


    // Desenhar os segundos com a cor aleatória
    tft.setTextColor(randomColor, TFT_BLACK);
    tft.setTextSize(3);
    tft.setFreeFont(&FreeSerifBold12pt7b);
    tft.drawString(seconds, 365, 250);

    // Decrementar o contador
    updateCounter--;
  }
}
