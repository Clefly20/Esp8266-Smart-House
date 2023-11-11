#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESPAsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <SerialSend.h>
#include <ESP8266mDNS.h>
#include <Html.h>

const char* ssid     = "{Your SSID}";
const char* password = "{Your Password}";

String slider_value1 = "0";
String slider_value2 = "0";
String slider_value3 = "0";

const char* input_parameter1 = "value";
const char* input_parameter2 = "value";
const char* input_parameter3 = "value";

IPAddress local_IP(192, 168, 1, 184); //Website Local IP

IPAddress gateway(192, 168, 1, 1);

IPAddress subnet(255, 255, 0, 0);
IPAddress primaryDNS(8, 8, 8, 8);   //optional
IPAddress secondaryDNS(8, 8, 4, 4); //optional

int Relayposition = 4,Led1Brigthness,Led2Brigthness,Led3Brigthness;

AsyncWebServer server(80);


String processor(const String& var){
  if (var == "SLIDERVALUE1"){
    return slider_value1;
  }
   else if (var == "SLIDERVALUE2"){
    return slider_value2;
  }
     else if (var == "SLIDERVALUE3"){
    return slider_value3;
  }
  return String();
}

void sendpacks(int relay,int led1,int led2,int led3){
SendLedControl(relay,led1,led2,led3,0);
}

void activatepacks(){
sendpacks(Relayposition,Led1Brigthness,Led2Brigthness,Led3Brigthness);
}

void WorkshopSpecialFunc1(){
  static bool switcher = 1;
  if(switcher == 1){

Relayposition = 3;
Led2Brigthness = 255;
switcher = 0;
  }
  else{
  
    Led2Brigthness = 0;
    switcher = 1;
  }
}
void WorkshopSpecialFunc2(){
  static bool switcher = 1;
  if(switcher == 1){

Relayposition = 3;
Led1Brigthness = 255;
switcher = 0;
  }
  else{

    Led1Brigthness = 0;
    switcher = 1;
  }
}

void setup(){
  Serial.begin(115200);
  setup1();

  if (WiFi.config(local_IP, gateway, subnet, primaryDNS, secondaryDNS)) {
    Serial.println("STA -Succes to configure");
  }
  else{
    Serial.println("STA -Failed to configure");
  }

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting...");
  }

  MDNS.begin(""); // Optional

  Serial.println(WiFi.localIP());


  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/html", index_html, processor);
  });


    server.on("/button1", HTTP_GET, [](AsyncWebServerRequest *request){
    Serial.println("button1");
    Relayposition = 1;
    activatepacks();
    request->send_P(200, "text/plain", "OK");
  });
      server.on("/button2", HTTP_GET, [](AsyncWebServerRequest *request){
          Serial.println("button2");
          Relayposition = 2;
          activatepacks();
    request->send_P(200, "text/plain", "OK");
  });
      server.on("/button3", HTTP_GET, [](AsyncWebServerRequest *request){
          Serial.println("button3");
          Relayposition = 3;
          activatepacks();
    request->send_P(200, "text/plain", "OK");
  });
      server.on("/button4", HTTP_GET, [](AsyncWebServerRequest *request){
          Serial.println("button4");
          Relayposition = 4;
          activatepacks();
    request->send_P(200, "text/plain", "OK");
  });
        server.on("/button5", HTTP_GET, [](AsyncWebServerRequest *request){
          Serial.println("button5");
          WorkshopSpecialFunc1();
          activatepacks();
    request->send_P(200, "text/plain", "OK");
  });
          server.on("/watchsend", HTTP_GET, [](AsyncWebServerRequest *request){
          Serial.println("watchsend");
          WorkshopSpecialFunc2();
          activatepacks();
    request->send_P(200, "text/plain", "OK");
  });

    server.on("/slider1", HTTP_GET, [] (AsyncWebServerRequest *request) {
    String message;
    if (request->hasParam(input_parameter1)) {
      message = request->getParam(input_parameter1)->value();
      slider_value1 = message;
      Led1Brigthness = slider_value1.toInt();
      activatepacks();
    }
    else {
      message = "No message sent";
    }
    Serial.println(message);
    request->send(200, "text/plain", "OK");
  });

      server.on("/slider2", HTTP_GET, [] (AsyncWebServerRequest *request) {
    String message;
    if (request->hasParam(input_parameter2)) {
      message = request->getParam(input_parameter2)->value();
      slider_value2 = message;
      Led2Brigthness = slider_value2.toInt();
      activatepacks();
    }
    else {
      message = "No message sent";
    }
    Serial.println(message);
    request->send(200, "text/plain", "OK");
  });

      server.on("/slider3", HTTP_GET, [] (AsyncWebServerRequest *request) {
    String message;
    if (request->hasParam(input_parameter3)) {
      message = request->getParam(input_parameter3)->value();
      slider_value3 = message;
      Led3Brigthness = slider_value3.toInt();
      activatepacks();
    }
    else {
      message = "No message sent";
    }
    Serial.println(message);
    request->send(200, "text/plain", "OK");
  });
  
  server.begin();
  MDNS.update();
}
  


void loop() {
 // Empty
}