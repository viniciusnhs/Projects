#include <WiFi.h>


#define CONNECTION_TIMEOUT 10
int timeout_counter = 0;

/*
Configurações IP Estático
*/

//IPAddress local_IP(192, 168, 1, 102);
//IPAddress gateway(192, 168, 1, 1);
//IPAddress subnet(255, 255, 255, 0);
//IPAddress primaryDNS(8, 8, 8, 8);
//IPAddress secondaryDNS(8, 8, 4, 4);

/**
 * Credenciais da Rede WIFI
 */

const char* ssid_AP = "OvO_Slave";
char password_AP[] = "ovomaster";
char ssid[] = "3V1S";
char password[] = "viniciussuellen";

void initWIFI(){

  ///Configuração de IP estático
  //if (!WiFi.config(local_IP, gateway, subnet, primaryDNS, secondaryDNS)) {
  //  Serial.println("STA Failed to configure");
  //}

  WiFi.mode(WIFI_AP_STA);
  WiFi.softAP(ssid_AP, password_AP);
  WiFi.begin(ssid,password);
  Serial.print("Connecting to WiFi ..");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(1000);
    timeout_counter++;
        if(timeout_counter >= CONNECTION_TIMEOUT*5){
        ESP.restart();
        }
  }
  Serial.println(WiFi.localIP());
  Serial.print("DNS: ");
  Serial.println(WiFi.dnsIP());
  Serial.print("RRSI: ");
  Serial.println(WiFi.RSSI());
}

void closeWIFI(){
  if(WiFi.disconnect())
  {
    Serial.print("Conexão terminada");
    }
}