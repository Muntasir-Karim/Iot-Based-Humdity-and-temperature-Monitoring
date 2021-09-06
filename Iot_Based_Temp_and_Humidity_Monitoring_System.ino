#include "DHT.h"
#include <SoftwareSerial.h>
#include <LiquidCrystal.h>

const int rs = 12, en = 11, d4 = 5, d5 = 4, d6 = 13, d7 = 10;
LiquidCrystal lcd(rs,en , d4, d5, d6, d7);



int buzz = 9;                            

#define dhtDataPin 7                       // Digital Pin 7

#define dhtModel DHT11                  // We are Using DHT11

String WriteApiKey = "NSVLA0XPANHGPHDY";    // Edit this API key according to your Account

String Wifi_Name = "Sirus";         // Edit Host_Name

String wifiPassword = "55555221";          // Edit Password

SoftwareSerial ser(2, 3);              // RX, TX

DHT dht(dhtDataPin, dhtModel);  // Initialising Pin and Type of DHT


void setup() {  

  
Serial.begin(115200);                  // enable software serial

ser.begin(115200);                     // reset ESP8266

ser.println("AT+RST");               // Resetting ESP8266

dht.begin();                        // Enabling DHT11

char  k='"';

String s1 = "AT+CWJAP";

     s1+= "=";

       s1+= k;

       s1+= Wifi_Name;

       s1+= k;

       s1+= ",";

       s1+= k;

       s1+= wifiPassword;

       s1+= k;

ser.println(s1);                    // Connecting ESP8266 to your WiFi Router



   lcd.begin(16, 2);              
   lcd.print("Temperature:" );
   lcd.setCursor(0,1);
   lcd.print("Humidity:" );       
   
}
  

void loop() {

  int humidity =  dht.readHumidity();             // Reading Humidity Value
  int temperature = dht.readTemperature();        // Reading Temperature Value


  String state1=String(humidity);                 // Converting them to string 
  String state2=String(temperature);              // as to send it through URL

  Serial.print("Temperature = ");
  Serial.println(temperature);
 
  Serial.print("Humidity = ");
  Serial.println(humidity);
  
  
//for lcd
  lcd.setCursor(13, 0);  
  
  if (isnan(humidity) || isnan(temperature)) {
    lcd.print("Failed to Read");
    return;
  }

  lcd.print(temperature);
  lcd.setCursor(10,1);
  lcd.print(humidity);
  
   


//for buzzer

  pinMode(buzz, OUTPUT);       
  
  if(temperature >=32 || humidity >=65)
  {
    int i=0;

    do{
      i++;
    tone(buzz, 1000);  
     delay(200);        
    noTone(buzz);     
     delay(200);        
    }while(i<2);
           
  }



  String s2 = "AT+CIPSTART=\"TCP\",\"";          // Establishing TCP connection

  s2 += "184.106.153.149";                       // api.thingspeak.com

  s2 += "\",80";                                 // port 80

  ser.println(s2);

 if(ser.find((char*)"Error")){

    return;

  }

String s3 = "GET /update?api_key=";           // prepare GET string

  s3 += WriteApiKey;

  s3 +="&field1=";

  s3 += String(state1);                       // Humidity Data

  s3 +="&field2=";

  s3 += String(state2);                       // Temperature Data

  s3 += "\r\n\r\n";
  

  s2 = "AT+CIPSEND=";

  s2 += String(s3.length());                // Total Length of data

  ser.println(s2);

  if(ser.find((char*)">")){

    ser.print(s3);

  }

  else{

    ser.println("AT+CIPCLOSE");                  // closing connection

  }
  

}
