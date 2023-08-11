#include <HTTPClient.h>

const char *serverName = "http://django-env.eba-pdtwmpte.us-east-1.elasticbeanstalk.com/datalogsensor/?sensor=01";
const char *username = "ovomaster";
const char *passwordHTTP = "ovomaster";

String wifi_http_post(String Dados_Sensor)// O parametro id sensor é o identificador da estação hardware cadastrada na API- REST
{
  HTTPClient http;
  http.begin(serverName);
  http.addHeader("Content-Type", "application/json");
  http.setAuthorization(username,passwordHTTP);
  String httpRequestData =  Dados_Sensor;
  int httpResponseCode = http.POST(httpRequestData);
  String payload = "{}";

  if (httpResponseCode > 0)
  {
    Serial.print("HTTP Response code: ");
    Serial.println(httpResponseCode);
    payload = http.getString();
  }
  else
  {
    Serial.print("Error code: ");
    Serial.println(httpResponseCode);
  }
  
  http.end();
  return payload;
}

String httpGETRequest()
{
  HTTPClient http;
  http.begin(serverName);
  http.addHeader("Content-Type", "application/x-www-form-urlencoded");
  http.setAuthorization(username,passwordHTTP);
  int httpResponseCode = http.GET();
  String payload = "{}";

  if (httpResponseCode > 0)
  {
    Serial.print("HTTP Response code: ");
    Serial.println(httpResponseCode);
    payload = http.getString();
  }
  else
  {
    Serial.print("Error code: ");
    Serial.println(httpResponseCode);
  }
  
  http.end();

  return payload;
}