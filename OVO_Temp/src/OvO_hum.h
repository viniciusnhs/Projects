#include <Arduino.h>
#include <DHT.h>

float hum (DHT &dht){
    float h = dht.readHumidity();
    //Serial.print(F("%  Temperature: "));
    //Serial.print(t);
    //Serial.println(F("°C "));
    return h;
}