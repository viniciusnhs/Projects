#include <Arduino.h>
#include <DHT.h>

float temperatura (DHT &dht){
    float t = dht.readTemperature();
    //Serial.print(F("%  Temperature: "));
    //Serial.print(t);
    //Serial.println(F("°C "));
    return t;
}