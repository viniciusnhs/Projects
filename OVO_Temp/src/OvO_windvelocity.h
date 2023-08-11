# include <arduino.h>

boolean state = false;
int NL=0;
int clicked;
unsigned long lastMillis = 0;

const int Anem = 35; //P 35 in anem  


float RPM = 0;       // Revolutions per minute
float speedwind = 0;// Wind speed (km/h)
float vm=0;
///float vmd=0;
float vmax=0;
///float Vref=0;

float windvelocity() {
    NL++;

    speedwind = 0;

    lastMillis = xTaskGetTickCount();
    while(xTaskGetTickCount() - lastMillis < 3000){
        if(digitalRead(Anem) == HIGH) if(state == false){
            delay(50);
            clicked++;
            state = true;
        }
        if(digitalRead(Anem) == LOW) if(state == true) state = false;
    }
    
    RPM = clicked * 20;
    speedwind = ((30.77 *RPM)/1000) * 3.6;

  vm=vm+speedwind;
  
  if(speedwind > vmax ){
   vmax = speedwind;
   }

   //Serial.print(" - Veloc : ");
   //Serial.print(speedwind);
   //Serial.print(" - Vmd : ");
   //Serial.println(vm/NL);
   
   clicked=0;
   RPM=0;
   
  //esp_task_wdt_reset(); 

  //speedwind = 0;
  return speedwind;
}