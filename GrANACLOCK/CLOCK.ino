
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

    drawPing();

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
    tft.drawString(seconds, 365, 250);

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





void drawPing() {
  int pingResult = Ping.ping("8.8.8.8", 1); // Realiza o ping e verifica se teve sucesso

  // Limpa a área onde o ping será mostrado
  tft.fillRect(5, tft.height() - 30, 100, 20, TFT_BLACK);

  // Exibe o valor do ping
  tft.setTextColor(TFT_GREEN, TFT_BLACK);
  tft.setTextSize(1);
  tft.setFreeFont(&FreeSansBold9pt7b);

  if (pingResult) {
    int pingTime = Ping.averageTime(); // Obtém o tempo médio de ping
    tft.drawString("Ping: " + String(pingTime) + " ms", 5, tft.height() - 30);
  } else {
    tft.drawString("Ping: Error", 5, tft.height() - 30);
  }
}




void scrollText() {

  tft.setRotation(rotate);  // Ajuste a rotação se necessário
  //tft.fillScreen(TFT_BLACK);

  tft.setTextColor(TFT_GREEN, TFT_BLACK);  // Cor do texto
  tft.setFreeFont(&FreeSerifBold24pt7b);  // Fonte
  tft.setTextSize(3);  // Tamanho do texto

  
  tft.fillRect(0, tft.height() - 480, tft.width(), 480, TFT_BLACK);  // Limpa a área onde o texto será desenhado
  tft.setCursor(posX, tft.height() - 70);  // Define a posição do cursor
  tft.print(message);  // Desenha a mensagem

  posX -= 25;  // Move o texto para a esquerda

  // Se o texto tiver saído da tela à esquerda, reinicia a posição à direita
  if (posX < -tft.textWidth(message)) {
    posX = tft.width();
  }

}
