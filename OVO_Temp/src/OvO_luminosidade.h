#include <arduino.h>
#include <BH1750.h>

float luz (BH1750 &lightMeter){
  float lux = lightMeter.readLightLevel(); //VARIÁVEL RECEBE O VALOR DE LUMINOSIDADE MEDIDO
  //Serial.print("Luminosidade: "); //IMPRIME O TEXTO NA SERIAL
  //Serial.print(lux); //IMPRIME NA SERIAL O VALOR DE LUMINOSIDADE MEDIDO
  //Serial.println(" lux"); //IMPRIME O TEXTO NA SERIAL
  return lux;
}