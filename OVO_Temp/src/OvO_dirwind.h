#include <arduino.h>


int windirinp = 34;  //entrada biruta P34

int winddirstate = 0 ; 
int winddir_eu =0 ;
int wd=0;
int wds=0;

float winddir=90;
float windspeed=0;
float windgust= 0;

float DirWind(){

    /*for (int i = 0; i < 20; i++) {
    wd = analogRead(windirinp);
    Serial.println(wd);
    wds=wds+wd; 
    delay(50); 
    }*/

    wd = analogRead(windirinp);
    //Serial.println(wd);
    winddirstate = wd;

    //winddirstate = wds / 20;

  if  (winddirstate >= 3150){
    winddir_eu = 0;
  }
  if ( (winddirstate >= 2550) && (winddirstate <= 3149) ) {
    winddir_eu = 45;  //N 45 I 315
  }
  if ( (winddirstate >= 2050) && (winddirstate <= 2549) ) {
    winddir_eu = 90; //N 90 I 270
  }
  if ( (winddirstate >= 1550) && (winddirstate <= 2049) ) {
    winddir_eu = 135; //N 135 I 225
  }
  if ( (winddirstate >= 1050) && (winddirstate <= 1549) ) {
    winddir_eu = 180;
  }
  if ( (winddirstate >= 550) && (winddirstate <= 1049) ) {
    winddir_eu = 225; //N 225  I 135
  }
  if ( (winddirstate >= 150 ) && (winddirstate <= 549) ) {
    winddir_eu = 270;  // N 270  I 90
  }
  if ( (winddirstate >= 0) && (winddirstate <= 149) ) {
    winddir_eu = 315;  //N 315  I 45
  }

  wd=0;
  wds=0;

  //Serial.print(" -- Pin Status: ");
  //Serial.println(winddirstate);
  //}
  return winddir_eu;

}