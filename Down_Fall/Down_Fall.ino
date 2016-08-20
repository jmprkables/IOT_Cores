/*
 *  This sketch sends data via HTTP GET requests to data.sparkfun.com service.
 *
 *  You need to get streamId and privateKey at data.sparkfun.com and paste them
 *  below. Or just customize this script to talk to other HTTP servers.
 *
 */

#include <ESP8266WiFi.h>
#define TRESHOLD 180

int OldG,NewG,AllIsWell;

const char* ssid     = "blah";
const char* password = "password";
const char* host = "http://192.168.6.26:8085";
//const char* host = "data.sparkfun.com";
//const char* streamId   = "G2Kpo3JwYGc7bZYpoDGg";
//const char* privateKey = "NWvmaKPJ2lhBEZlKMjWe";

void setup() {
  Serial.begin(115200);
  delay(10);

  // We start by connecting to a WiFi network
  OldG = analogRead(0);
  AllIsWell = 0;
  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  
  WiFi.begin(ssid, password);
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  
 
  Serial.println("");
  Serial.println("WiFi connected");  
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

int value = 0;

void loop() {
    NewG = analogRead(0);
    if ((NewG-OldG > TRESHOLD) || (OldG-NewG > TRESHOLD)){
      for(int i=0;i<10;i++){
        Serial.println("Major Colission");
        SendGet(1);
        delay(100);
      }
      OldG = NewG;
    }
    // if counter mod 100 then all is well
    AllIsWell++;
      if(AllIsWell%100000 == 0 ){
         Serial.println("AllIsWell");
         SendGet(0); // Comment this out for better Sensitivity
         AllIsWell = 0;
      }
    OldG = NewG;
}

void SendGet(bool Fall){
  Serial.print("connecting to ");
  Serial.println(host);
  
  // Use WiFiClient class to create TCP connections
  WiFiClient client;
  const int httpPort = 80;
  if (!client.connect(host, httpPort)) {
    Serial.println("connection failed");
    return;
  }
 // We now create a URI for the request
 // String url = "/input/";
 // url += streamId;
 //url += "?private_key=";
 //  url += privateKey;
String  url = "/&fallen=/fall?fallen=";
  url += Fall;
  
  Serial.print("Requesting URL: ");
  Serial.println(url);
  
  // This will send the request to the server
  client.print(String("GET ") + url + " HTTP/1.1\r\n" +
               "Host: " + host + "\r\n" + 
               "Connection: close\r\n\r\n");

  unsigned long timeout = millis();
  while (client.available() == 0) {
    if (millis() - timeout > 5000) {
      Serial.println(">>> Client Timeout !");
      client.stop();
      return;
    }
  }
  // Read all the lines of the reply from server and print them to Serial
  while(client.available()){
    String line = client.readStringUntil('\r');
    Serial.print(line);
  }
  Serial.println();
  Serial.println("closing connection");  
}

