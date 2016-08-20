#include<SoftwareSerial.h>
void setup() {
  // put your setup code here, to run once:
SoftwareSerial ESPWiFi ( 11 , 12 ); //RX , TX

Serial.begin(115200);
ESPWiFi.begin(115200);

ESPWiFi.print("AT\n\r");
delay(1000);
//Serial.print("AT+RST\n\r");
delay(1000);
ESPWiFi.print("AT+CWJAP=\"GUEST-N\",\"obscure123\"\n\r");
delay(1000);
ESPWiFi.print("AT+CIFSR\n\r");
delay(1000);
Serial.println(ESPWiFi.read());
Serial.println(ESPWiFi.read());
Serial.println(ESPWiFi.read());
Serial.println(ESPWiFi.read());
}

void loop() {
  // put your main code here, to run repeatedly:

}
