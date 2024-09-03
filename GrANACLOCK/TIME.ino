void UPDATETIME(){
  
  
    timeClient.update();

  // Desenha a data uma única vez na inicialização
  unsigned long epochTime = timeClient.getEpochTime();
  struct tm *timeInfo = localtime((time_t *)&epochTime);

  int day = timeInfo->tm_wday;
  int dayOfMonth = timeInfo->tm_mday;
  int month = timeInfo->tm_mon;
  int year = timeInfo->tm_year + 1900;

  String daysOfWeek[7] = {"Dom", "Seg", "Ter", "Qua", "Qui", "Sex", "Sáb"};
  String months[12] = {"Janeiro", "Fevereiro", "Março", "Abril", "Maio", "Junho", 
                       "Julho", "Agosto", "Setembro", "Outubro", "Novembro", "Dezembro"};

  String dayOfWeek = daysOfWeek[day];
  String monthName = months[month];

  fullDate = dayOfWeek + ", " + String(dayOfMonth) + " de " + monthName + " de " + String(year);

  
  
  }
