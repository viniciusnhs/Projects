#include <Arduino.h>
#include <Adafruit_BMP085.h>

float pressao (Adafruit_BMP085 &bmp){
    float p = bmp.readPressure();
    //Serial.print(F("Pressão"));
    //Serial.print(p);
    //Serial.println(F("°C "));
    return p;
}