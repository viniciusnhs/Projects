#include <WiFi.h>
#include <time.h>


struct tm timeinfo;

const char* ntpServer = "b.st1.ntp.br";         // Servidor ntpServer
const long  gmtOffset_sec = -14400;                 // OffSet de Fuso Horário (Brasil)
const int   daylightOffset_sec = 3600;          // OffSet de Hora do Dia


String printData(){
    while(!getLocalTime(&timeinfo)){
    configTime (gmtOffset_sec, daylightOffset_sec, ntpServer);
    Serial.println("Failed to obtain time");
  }

  int dia = timeinfo.tm_mday;
  int mes = timeinfo.tm_mon+1;
  int ano = timeinfo.tm_year+1900;

  String data = String(dia)+"/"+String(mes)+"/"+String(ano);

  return data;

}
int printHora(){
    while(!getLocalTime(&timeinfo)){
    configTime (gmtOffset_sec, daylightOffset_sec, ntpServer);
    Serial.println("Failed to obtain time");
  }

  int hora = timeinfo.tm_hour;

  return hora;

}

int printMinute(){
    while(!getLocalTime(&timeinfo)){
    configTime (gmtOffset_sec, daylightOffset_sec, ntpServer);
    Serial.println("Failed to obtain time");
  }

  int min = timeinfo.tm_min;

  return min;

}


int printSecond(){
    configTime (gmtOffset_sec, daylightOffset_sec, ntpServer);
    while(!getLocalTime(&timeinfo)){
    configTime (gmtOffset_sec, daylightOffset_sec, ntpServer);
    Serial.println("Failed to obtain time");
  }

  int sec = timeinfo.tm_sec;

  return sec;

}

void printLocalTime(){
   while(!getLocalTime(&timeinfo)){
    configTime (gmtOffset_sec, daylightOffset_sec, ntpServer);
    Serial.println("Failed to obtain time");
   }
  int minuto = timeinfo.tm_min;
  int hor = timeinfo.tm_hour;
  int dia = timeinfo.tm_mday;
  int mes = timeinfo.tm_mon+1;
  int ano = timeinfo.tm_year+1900;
  String tempo = String(dia)+"/"+String(mes)+"/"+String(ano)+"\t"+String(hor)+":"+String(minuto);
  Serial.println(tempo);
  return;
}

