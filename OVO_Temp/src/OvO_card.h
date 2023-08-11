#include <SD.h>
#include <FS.h>
#include <OvO_file.h>

/*******************************************************
   * Inicializando Cartão SD
   */

void cardOVO() {
if(!SD.begin()){
  Serial.println("Não montou o cartão! Verifique as conexões");
  return;
}

uint8_t cardType = SD.cardType();

if(cardType == CARD_NONE){
  Serial.println("Quer que funcione sem cartão?");
  return;
}

Serial.print("Tipo de Cartão: ");
  if(cardType == CARD_MMC){
    Serial.println("MMC");
  } else if(cardType == CARD_SD){
    Serial.println("SDSC");
  } else if(cardType == CARD_SDHC){
    Serial.println("SDHC");
  } else {
    Serial.println("UNKNOWN");
  }

uint64_t cardSize = SD.cardSize() / (1024 * 1024);
Serial.printf("Tamanho do Cartão: %lluMB\n", cardSize);
Serial.printf("Espaço Ocupado: %lluMB\n", SD.usedBytes() / (1024 * 1024));
return;
}