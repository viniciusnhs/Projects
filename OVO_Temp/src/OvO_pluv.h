#include <Arduino.h>
#include <EEPROM.h>

/*#define ASSINATURA 0
#define mmHe2p 1
#define mmDe2p 3
#define hrAnte2p 5
#define diaAnte2p 6
#define crc_byte 7
#define bootCnte2p 8*/

#define PIN_VL 4  // P4 entrada pluviometro
#define RSTp 2   // P2 RST pluviometro



int ReadWord(int endereco);
void WriteWord(int endereco,int dado);

//int REEDCOUNT = 0;  //conta pulso / hora
//int REEDh = 0;     //conta pulso / 24h
//float rain = 0;       //armazena pulso hora
//float rainh = 0;      //armazena até 24h
int pulso_rain;

int Pluv(){

  if ( digitalRead(PIN_VL) == HIGH ) {

    Serial.println(("Ocorreu um pulso"));
    pulso_rain = 1;
    delay(100);
    digitalWrite(RSTp,LOW ); // reseta pluviom
    //Serial.println(("Reseta hw externo "));
    delay(100);
    digitalWrite(RSTp, HIGH);  // ativa pluviom e finaliza leitura
  }
  else
  {
    pulso_rain = 0;
  }
  return pulso_rain;
}

int ReadWord(int endereco)
{
  EEPROM.begin(512);
  int aux=EEPROM.read(endereco);
  aux=(aux*256)+EEPROM.read(endereco+1);
  EEPROM.end();
  return aux;
}

void WriteWord(int endereco,int dado)
{
  EEPROM.begin(512);
  EEPROM.write(endereco,dado/256);
  delay(10);
  EEPROM.write(endereco+1,dado%256);
  delay(10);
  EEPROM.commit();
  delay(10);
}