#include <SdFat.h>
#include <Wire.h>
#include <BH1750.h>
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <WiFiClient.h>

SdFat sd;
BH1750 lightMeter;
ESP8266WebServer server(80);

char* ssid = "Michael";
char* password = "19950523";

String page = "";
double lux;

void setup(){
  Serial.begin(115200);
  Serial.println("Initializing SD card...");
  
  sd.begin(15);
  Wire.begin();
  
  WiFi.begin(ssid, password);
  Serial.print("Connecting to ");
  Serial.print(ssid); 
  Serial.println(" ...");

  delay(10000);

  if (WiFi.status() != WL_CONNECTED) 
  {
     delay(500);
     Serial.print("*");
     Serial.println("");
     Serial.println("Couldn't get a wifi connection");
     // don't do anything else:
     while(false);
  }
  else{
     delay(500);
     Serial.print("*");
     Serial.println("");
     Serial.println("WiFi connection Successful");
     Serial.print("The IP Address of ESP8266 Module is: ");
     Serial.println(WiFi.localIP());// Print the IP address
  }
  
  lightMeter.begin();
  Serial.println(F("BH1750 Test"));

  File dataFile = sd.open("Light_Density.txt", FILE_WRITE);
    if(dataFile){
      dataFile.println(">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>");
      dataFile.close();
    }
      else { 
        Serial.println("error opening test.txt");
      }
  server.on("/",[](){
      page = "<h1>Sensor to Light Test Web Server</h1><h3>Lux:</h3> <h4>"+String(lux)+"</h4>";
      server.send(200,"text/html",page);
  });    
  server.begin();
}

void loop() {
  String dataString = "";
  lux = lightMeter.readLightLevel();
  dataString += String(lux);
  File dataFile = sd.open("Light_Density.txt", FILE_WRITE);
  if(dataFile){
    delay(1000);
    dataFile.print("Light: ");
    dataFile.print(dataString);
    dataFile.println(" lx");
    dataFile.close();
    Serial.print("Light: ");
    Serial.print(dataString);
    Serial.println(" lx");
  } 
    else {
    // if the file didn't open, print an error:
    delay(1000);
    Serial.println("error opening test.txt");
    Serial.print("Light: ");
    Serial.print(dataString);
    Serial.println(" lx");
    }
  
  server.handleClient();
}
