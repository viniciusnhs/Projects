#include <Wire.h> 
#include <BH1750.h>
#include <OvO_wifi.h>
#include <DHT.h>
#include <OvO_windvelocity.h>
#include <OvO_dirwind.h>
#include <OvO_card.h>
#include <OvO_time.h>
#include <OvO_temp.h>
#include <OvO_luminosidade.h>
#include <OvO_hum.h>
#include <OvO_math.h>
#include <OvO_pluv.h>
#include <EEPROM.h>
#include <Adafruit_BMP085.h>
#include <OvO_pressao.h>
#include "RTClib.h"
#include <OvO_post.h>
#include <OvO_webserver.h>
#include <SPIFFS.h>
#include <OvO_otahtml.h>

void webSocketEvent(byte num, WStype_t type, uint8_t * payload, size_t length);
String wifiST;  

//--------- Estação ID -------------//

String id_ovo = "01";         // ID da Estação 

//-------- Objeto do Sensor de Pressão -----------//

//Adafruit_BMP085 bmp;          

// -------- Pluviometro --------------//

#define PIN_VL 4      // P4 entrada pluviometro
#define RSTp 2        // P2 RST pluviometro
int pulso = 0;        //armazena pulso hora
float chuva =0;

//---------DHT-----------
#define DHTPIN  15 // Pin 33 interfere no biruta 
#define DHTTYPE DHT22   // AM2301 
DHT dht(DHTPIN, DHTTYPE,20);

//--------- Senso de Luminosidade -------------

//BH1750 lightMeter; //CRIA UMA INSTÂNCIA

//---------- Definindo RTC -------------------

//RTC_DS3231 rtc;
int dia;
int mes;
int ano;
int hora;
char tempo_RTC[15];
String hora_ntp;
String data_ntp;

//--------- Variáveis Globas ----------//

int minuto = 0;
int sec = 0;
float vel_vento[10];
float dir_vento[10];
float temp[10];
float ligth[10];
float humidade[10];
float pressure[10];
int counter = 0;
String tempo;

void setup(){

  //------------------ Inicialização de sensores e dispositivos --------------------


  Serial.begin(115200);   //Inicializa serial
  Wire.begin();           //Inicializa porta I2C

  /*if (! rtc.begin()) {
      Serial.println("Não encontrado");
      Serial.flush();
    } else{
      Serial.println("OK");
    }*/

  
  //lightMeter.begin();     //Inicializa medidor de luminosidade
  dht.begin();            //Inicializa medidor de temperatura
  //bmp.begin();            //Inicializa medido de pressão
  cardOVO();              //Inicializa cartão de memória
  //rtc.adjust(DateTime(__DATE__, __TIME__)); // Sincroniza com horário do computador

  //rtc.adjust(DateTime(2023, 5, 15, 17, 9, 0));// Ano,Mes,Dia,Hora,Minuto,Segundo

  pinMode (RSTp,OUTPUT); // reset pluviometro 
  pinMode (PIN_VL,INPUT); // entrada pluvio 
  digitalWrite(RSTp, HIGH);  // ativa pluviom

 initWIFI();             //Inicializa e conecta WiFi



if(!SPIFFS.begin()){
        Serial.println("An Error has occurred while mounting SPIFFS");
        return;
  }
//*****************************************************************
//---------------- Configuração do WebServer ----------------------
//***************************************************************** 

initSERVER();
  

//*****************************************************************
//------------- Configuração do Anemômetro ------------------------
//***************************************************************** 

pinMode(Anem, INPUT);

//*****************************************************************
//------------- Horário de Inicialização --------------------------
//***************************************************************** 

printLocalTime();

}

void loop() {




//delay (1000);
//--------------- Tempo ---------------//
/*DateTime now = rtc.now();
delay(1000);
int temp_min = now.minute();
dia =now.day();
mes = now.month();
ano = now.year();
hora = now.hour();
sprintf(tempo_RTC,"%d/%d/%d \t %d:%d",dia, mes, ano, hora, temp_min);*/

//----------- Chuva -------------

pulso = Pluv() + pulso;
int tempo_min = printMinute();
int tempo_hor = printHora();


if (minuto!=tempo_min){

   hora_ntp = String(tempo_hor)+":"+String(tempo_min);
   data_ntp = printData();

   //String tempo = printLocalTime();           // Horário e Data
   //Serial.println (tempo);

   //-------------- Velocidade do vento ------------------//
   /*vel_vento[counter] = windvelocity();         
   Serial.print("Velocidade do Vento: ");
   Serial.println(vel_vento[counter]);*/

 //-------------- Direção do vento ------------------//
   
   /*dir_vento[counter] = DirWind();              
   Serial.print("Direção do Vento: ");
   Serial.println(dir_vento[counter]);*/
   
  //-------------- Temperatura ------------------//
   
   temp[counter] = temperatura(dht);           
   //Serial.print("Temperatura: ");
   //Serial.println(temp[counter]);

   //-------------- Pressão ------------------//

   /*pressure[counter] = pressao(bmp);           
   //temp[counter] = temp[counter] + counter;            
   Serial.print("Pressão ");
   Serial.println(pressure[counter]);*/
   
    //-------------- Luminosidade------------------//

   /*ligth[counter] = luz(lightMeter);            
   Serial.print("Luminosidade: ");
   Serial.println(ligth[counter]);*/
   
      //-------------- Umidade ------------------//
   
   humidade[counter] = hum( dht);                // Humidity
   //Serial.print("Umidade: ");
   //Serial.println(humidade[counter]);

    //-------------- String com data, hora e parâmetros atmosféricos ------------------//

   String dados = (id_ovo+"\t"+tempo_RTC+"\t"+temp[counter]+"\t"+pressure[counter]+"\t"+humidade[counter]+"\t"+ligth[counter]+"\t"+vel_vento[counter]+"\t"+dir_vento[counter]+"\n");
   Serial.println (dados);

//-------------- String com data, hora e parâmetros atmosféricos ------------------//

   //appendFile(SD,"/OvO_All_min.txt",dados);     
   
   minuto = tempo_min;
   counter++;



}

/* A cada dez minutos as médias, máximas e mínimas do intervalo são calculadas e 
registradas em arquivo. Incluindo dados pluvimétricos */

if (counter == 10){

  /////// Temperatura: Medidade de Velocidade Média, Máxima e Mínima /////////
  float temp_soma = 0;
  float temp_max = temp[0];
  float temp_min = temp[0];
  for (int i = 0; i<=9; i++){
    temp_soma = temp[i]+temp_soma;
    temp_max = max(temp[i],temp_max);
    temp_min = min(temp[i],temp_min);
  }
  float temp_med = temp_soma/10;
  Serial.print("Temperatura Média: ");
  Serial.println(temp_med);
  Serial.print("Temperatura Máxima: ");
  Serial.println(temp_max);
  Serial.print("Temperatura Mínima: ");
  Serial.println(temp_min);

  String dez_dados = (id_ovo+"\t"+tempo_RTC+"\t"+temp_med+"\t"+temp_max+"\t"+temp_min+"\n");
  Serial.println(dez_dados);
  appendFile(SD,"/OvO_temp10.txt",dez_dados);      // Registrando em arquivo os dados da Estação a cada UM minuto


/////// Umidade: Medidade de Velocidade Média, Máxima e Mínima /////////

  float hum_soma = 0;
  float hum_max = humidade[0];
  float hum_min = humidade[0];
  for (int i = 0; i<=9; i++){
    hum_soma = humidade[i]+hum_soma;
    hum_max = max(humidade[i],hum_max);
    hum_min = min(humidade[i],hum_min);
  }
  float hum_med = hum_soma/10;
  Serial.print("Umidade Média: ");
  Serial.println(hum_med);
  Serial.print("Umidade Máxima: ");
  Serial.println(hum_max);
  Serial.print("Umidade Mínima: ");
  Serial.println(hum_min);

  //String tempo_post = printLocalTime();           // Horário e Data

  String dez_dados_hum = (id_ovo+"\t"+tempo_RTC+"\t"+hum_med+"\t"+hum_max+"\t"+hum_min+"\n");
  Serial.println(dez_dados_hum);
  appendFile(SD,"/OvO_hum10.txt",dez_dados_hum);      // Registrando em arquivo os dados da Estação a cada UM minuto


/////// Velocidade: Medidade de Velocidade Média, Máxima e Mínima /////////
/*
  float vel_soma = 0;
  float vel_max = vel_vento[0];
  float vel_min = vel_vento[0];
  for (int i = 0; i<=9; i++){
    vel_soma = vel_vento[i]+vel_soma;
    vel_max = max(vel_vento[i],vel_max);
    vel_min = min(vel_vento[i],vel_min);
  }
  float vel_med = vel_soma/10;
  Serial.print("Velocidade Média: ");
  Serial.println(vel_med);
  Serial.print("Velocidade Máxima: ");
  Serial.println(vel_max);
  Serial.print("Umidade Mínima: ");
  Serial.println(vel_min);

  //String tempo_post = printLocalTime();           // Horário e Data

  String dez_dados_vel = (id_ovo+"\t"+tempo_RTC+"\t"+vel_med+"\t"+vel_max+"\t"+vel_min+"\n");
  Serial.println(dez_dados_vel);
  appendFile(SD,"/OvO_vel10.txt",dez_dados_vel);      // Registrando em arquivo os dados da Estação a cada UM minuto
  */
  /////// Luminosidade: Medidade de Velocidade Média, Máxima e Mínima /////////
  /*
  float ligth_soma = 0;
  float ligth_max = ligth[0];
  float ligth_min = ligth[0];
  for (int i = 0; i<=9; i++){
    ligth_soma = ligth[i]+ligth_soma;
    ligth_max = max(ligth[i],ligth_max);
    ligth_min = min(ligth[i],ligth_min);
  }
  float ligth_med = ligth_soma/10;
  Serial.print("Luminosidade Média: ");
  Serial.println(ligth_med);
  Serial.print("Luminosidade Máxima: ");
  Serial.println(ligth_max);
  Serial.print("Luminosidade Mínima: ");
  Serial.println(ligth_min);

  //String tempo_post = printLocalTime();           // Horário e Data

  String dez_dados_ligth = (id_ovo+"\t"+tempo_RTC+"\t"+ligth_med+"\t"+ligth_max+"\t"+ligth_min+"\n");
  Serial.println(dez_dados_ligth);
  appendFile(SD,"/OvO_luz10.txt",dez_dados_ligth);    
*/
/////// Pressão: Medidade de Velocidade Média, Máxima e Mínima /////////
/*
  float press_soma = 0;
  float press_max = pressure[0];
  float press_min = pressure[0];
  for (int i = 0; i<=9; i++){
    press_soma = pressure[i]+press_soma;
    press_max = max(pressure[i],press_max);
    press_min = min(pressure[i],press_min);
  }
  float press_med = press_soma/10;
  Serial.print("Pressão Média: ");
  Serial.println(press_med);
  Serial.print("Pressão Máxima: ");
  Serial.println(press_max);
  Serial.print("Pressão Mínima: ");
  Serial.println(press_min);

  //String tempo_post = printLocalTime();           // Horário e Data

  String dez_dados_press = (id_ovo+"\t"+tempo_RTC+"\t"+press_med+"\t"+press_max+"\t"+press_min+"\n");
  Serial.println(dez_dados_press);
  appendFile(SD,"/OvO_press10.txt",dez_dados_press);  
*/
  /////// Chuva /////////

  chuva = pulso*0.1;

  Serial.print("Choveu, em mm, nos últimos 10 minutos: ");
  Serial.println(chuva);

  String dez_dados_chuva = (id_ovo+"\t"+tempo_RTC+"\t"+chuva+"\n");
  Serial.println(dez_dados_chuva);
  appendFile(SD,"/OvO_chuva10.txt",dez_dados_chuva);  

//// Postando no Banco de Dados ////

  String dados10;
 dados10 =  "{\"estacao\":\"" + String(1);
 dados10 += "\",\"data\":\"" + data_ntp;
 dados10 += "\",\"hora\":\"" + hora_ntp;
 dados10 += "\",\"temp_min\":\"" + String(temp_min);
 dados10 += "\",\"temp_max\":\"" + String(temp_max);
 dados10 += "\",\"temp_media\":\"" + String(temp_med);
 dados10 += "\",\"umidade_min\":\"" + String(hum_min);
 dados10 += "\",\"umidade_max\":\"" + String(hum_max);
 dados10 += "\",\"umidade_media\":\"" + String(hum_med);
 dados10 += "\",\"mm_ciclo\":\"" + String(chuva) + "\"}";

if ((WiFi.status() == WL_CONNECTED))
{
  wifi_http_post(dados10);
}
else
{
  initWIFI();
}

  pulso = 0;
  counter = 0;
}

if ( WiFi.status() == 3){
  wifiST = "ON";
} else {
  wifiST = "OFF";
}
server.handleClient();                          
webSocket.loop();     
String jsonString = "";
StaticJsonDocument<200> doc;
JsonObject object = doc.to<JsonObject>();
float t = dht.readTemperature();
float power = WiFi.RSSI();
object["temp1"] = t;
object["wifi"] = wifiST;
object["wifilevel"]= power;
object["chuva"]= chuva;
serializeJson(doc, jsonString);
webSocket.broadcastTXT(jsonString);

}