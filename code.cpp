 

/*
This sketch connects to http server and simply sends sensor data to it via POST method.
ESP8266 NodeMCU board is required to run the sketch.
*/

#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>
#include "ESP8266HTTPClient.h"
#define myserial Serial
ESP8266WiFiMulti myWiFiMulti;
int SensorPin = 16; // Sensor is connected to pin D0 (16)
boolean  state= 1; // Sensor state variable 

const char ssid[]="YourWiFiName"; 
const char pass[]="YourWifiPassword";
String auth = "YourRestcommAccountAuthenticationToken";
String accid = "YourRestcommAccountID";
String to = "YourMobileNumberwithCountryCode";
String from = "RestcommNumber";
String message = "This+is+a+test+msg";


// Base url according Restcomm SMS API documentation 
String url = "http://" + accid + ":" + auth + "@cloud.restcomm.com/restcomm/2012-04-24/Accounts/" + accid + "/SMS/Messages";


void setup() {
  pinMode(SensorPin, INPUT); 
  myserial.begin(115200); // Open serial port for logging and debugging
  for(uint8_t t = 4; t > 0; t--) {
      myserial.printf("Setup Wait %d...\n", t); //Post �waiting� to serial port
      myserial.flush();
      delay(1000);
  }
  myWiFiMulti.addAP(ssid,pass); // setup Wi-fi credentials
}

void loop() {
  
  if((myWiFiMulti.run() == WL_CONNECTED))    {
  myserial.println("Wi-Fi connected");;
  }
  delay(5000);

  if((myWiFiMulti.run() == WL_CONNECTED) && (digitalRead(SensorPin) == HIGH) && state==false) { // Check is sensor is high,WiFi is connected and previous state was low
      state=true;
      HTTPClient http;
      myserial.print("HTTP begin...\n");
      http.begin(url);
      http.POST("To=%2B" + to + "&From=%2B" + from + "&Body=" + message); // Post
      http.writeToStream(&Serial);
      http.end();
    
  }

if((digitalRead(SensorPin) == LOW)&& (state==true)) {
state=false; // If sensor turned low and previous state was high, update the sensor state 
}
}