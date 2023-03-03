#include <Arduino.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include "env.h"

//need JSON objects to read the JSON coming from server for get
#define LED_PIN0 2 //not sure which pins to use since i cant test physically
#define LED_PIN1 23
#define LED_PIN2 4
 bool b[ 8 ][ 3 ] = { { false, false, false }, { false, false, true } , {false, true, false},{false, true, true}, {true, false, false},{true, false, true},{true, true, false}, {true, true, true }};

void setup() {
  pinMode(LED_PIN0,OUTPUT);
  pinMode(LED_PIN1,OUTPUT);
  pinMode(LED_PIN2,OUTPUT);

  Serial.begin(9600);
  WiFi.begin(WIFI_SSID, WIFI_PASS); // initiates connection
  Serial.println("Connecting");
  while (WiFi.status() != WL_CONNECTED){
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected with IP Address: ");
  Serial.println(WiFi.localIP());
}

void loop() {
if (WiFi.status() != WL_CONNECTED ){
  HTTPClient http;
  http.begin(endpoint);
  http.addHeader("X-API-KEY",username);
  String http_response;
  http.addHeader("Content-Type", "application/json"); // look up what the request needs(if json, html or whatever) to know header type etc
  StaticJsonDocument<76> doc; // Empty JSONDocument
  char * httpRequestData;
  http.addHeader("Host"," https://ecse-three-led-api-v2.onrender.com");  
  http.addHeader("Content-length","76");

  serializeJson(doc, httpRequestData); // serialize to go from string to JSON
  //youd have to hardcode the data if you dont do it this way

  int httpResponseCode = http.PUT(httpRequestData); // takes info from the document dynamically
  //if you want to get different sets of information you need to retrive from different vairables of the doc

  //after this is essentially the same as get request

  if (httpResponseCode>0){
    
    Serial.print("HTTP Response code: ");
    Serial.print(httpResponseCode);
    Serial.print("Response from server:");
    http_response = http.getString();
    Serial.print(http_response);
  }
  else{
    Serial.print("Error code:");
    Serial.print(httpResponseCode);
  }
  http.end();
  StaticJsonDocument<1024> doc;
  DeserializationError error = deserializeJson(doc,http_response);

  if (error){
    Serial.print("Deserialize failed: ");
    Serial.print(error.c_str());
    return;
  }
 


for (int count = 0;count <=8;count++){
     doc["light_switch_1"]= b[count][0];
     doc["light_switch_2"]= b[count][1];
     doc["light_switch_3"]= b[count][2];

    const bool LED0_state= doc["light_switch_1"];
    const bool LED1_state= doc["light_switch_2"];
    const bool LED2_state= doc["light_switch_3"];
    digitalWrite(LED_PIN0,LED0_state);       
    digitalWrite(LED_PIN1,LED1_state);
    digitalWrite(LED_PIN2,LED2_state);
    delay(2000);
  //increment each variable like a truth table
}
}
else return;
  
  
}
