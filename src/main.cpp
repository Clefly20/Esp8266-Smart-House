#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESPAsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <SerialSend.h>
#include <ESP8266mDNS.h>

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

const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE HTML><html>
<head>

    <meta charset="utf-8">
<body background="https://i.ibb.co/9pK99wm/wallpaperflare.jpg">
  <meta name="viewport" content="width=device-width, initial-scale=1">
  <title>Smart House</title>
  <style>
    html {font-family: Times New Roman; display: inline-block; text-align: center;}
    h2 {font-size: 2.3rem;}
    p {font-size: 2.0rem;}
    body {max-width: 400px; margin:0px auto; padding-bottom: 25px;}
    .slider { -webkit-appearance: none; margin: 14px; width: 360px; height: 25px; background: #507680;
      outline: none; -webkit-transition: .2s; transition: opacity .2s;}
    .slider::-webkit-slider-thumb {-webkit-appearance: none; appearance: none; width: 35px; height: 35px; background:#edf1f2; cursor: pointer;}
    .slider::-moz-range-thumb { width: 35px; height: 35px; background: #edf1f2; cursor: pointer; } 
    .fill:hover,
.fill:focus {
  box-shadow: inset 0 0 0 2em var(--hover);
}

.pulse:hover,
.pulse:focus {
  -webkit-animation: pulse 1s;
          animation: pulse 1s;
  box-shadow: 0 0 0 2em transparent;
}

@-webkit-keyframes pulse {
  0% {
    box-shadow: 0 0 0 0 var(--hover);
  }
}

@keyframes pulse {
  0% {
    box-shadow: 0 0 0 0 var(--hover);
  }
}
.close:hover,
.close:focus {
  box-shadow: inset -3.5em 0 0 0 var(--hover), inset 3.5em 0 0 0 var(--hover);
}

.raise:hover,
.raise:focus {
  box-shadow: 0 0.5em 0.5em -0.4em var(--hover);
  transform: translateY(-0.25em);
}

.up:hover,
.up:focus {
  box-shadow: inset 0 -3.25em 0 0 var(--hover);
}

.slide:hover,
.slide:focus {
  box-shadow: inset 6.5em 0 0 0 var(--hover);
}

.offset {
  box-shadow: 0.3em 0.3em 0 0 var(--color), inset 0.3em 0.3em 0 0 var(--color);
}
.offset:hover, .offset:focus {
  box-shadow: 0 0 0 0 var(--hover), inset 6em 3.5em 0 0 var(--hover);
}

.fill {
  --color: #a972cb;
  --hover: #cb72aa;
}

.pulse {
    background-color: rgba(0, 0, 0, 0.767);
  --color: #ef6eae;
  --hover: #ef8f6e;
}

.close {
  --color: #ff7f82;
  --hover: #ffdc7f;
}

.raise {
  --color: #ffa260;
  --hover: #e5ff60;
}

.up {
  --color: #e4cb58;
  --hover: #94e458;
}

.slide {
  --color: #8fc866;
  --hover: #66c887;
}

.offset {
  --color: #19bc8b;
  --hover: #1973bc;
}

button {
  color: var(--color);
  transition: 0.25s;
}
button:hover, button:focus {
  border-color: var(--hover);
  color: #fff;
}

body {
  color: #fff;
  font: 300 1em "Fira Sans", sans-serif;
  justify-content: center;
  align-content: center;
  align-items: center;
  text-align: center;
  min-height: 100vh;
  display: flex;
}

button {
  background: none;
  border: 2px solid;
  font: inherit;
  line-height: 1;
  margin: 0.5em;
  padding: 1em 2em;
}

h1 {
  font-weight: 400;
}

code {
  color: #e4cb58;
  font: inherit;
}


      
      
  </style>
</head>

<body>
   <h2 style="color:white">Led Control</h2>
<center>
       <!-- 1. Buton -->
       <button onclick="button1Click()" class="pulse">Bass Mode</button>

       <!-- 2. Buton -->
       <button onclick="button2Click()" class="pulse">Power Save</button>
   
       <!-- 3. Buton -->
       <button onclick="button3Click()" class="pulse">Normal Mode</button>
   
       <!-- 4. Buton -->
       <button onclick="button4Click()" class="pulse">Power Off</button>

       <!-- 5. Buton -->
       <button onclick="button5Click()" class="pulse">Work Place</button>
    </center>

<center>
  <p style="color: white;"><span id="textslider_value1">%SLIDERVALUE1%</span></p>
  <p><input type="range" onchange="updateSliderPWM1(this)" id="pwmSlider1" min="0" max="255" value="%SLIDERVALUE1%" step="1" class="slider"></p>

  <p style="color: white;"><span id="textslider_value2">%SLIDERVALUE2%</span></p>
  <p><input type="range" onchange="updateSliderPWM2(this)" id="pwmSlider2" min="0" max="255" value="%SLIDERVALUE2%" step="1" class="slider"></p>

  <p style="color: white;"><span id="textslider_value3">%SLIDERVALUE3%</span></p>
  <p><input type="range" onchange="updateSliderPWM3(this)" id="pwmSlider3" min="0" max="255" value="%SLIDERVALUE3%" step="1" class="slider"></p>

</center>

<script>
   function button1Click() {
    console.log("pressed button1");
    var xhr = new XMLHttpRequest();
      xhr.open("GET", "/button1", true);
      xhr.send();
      }

      function button2Click() {
        console.log("pressed button2");
        var xhr = new XMLHttpRequest();
      xhr.open("GET", "/button2", true);
      xhr.send();
      }

      function button3Click() {
        console.log("pressed button3");
        var xhr = new XMLHttpRequest();
      xhr.open("GET", "/button3", true);
      xhr.send();
      }

      function button4Click() {
        console.log("pressed button4");
        var xhr = new XMLHttpRequest();
      xhr.open("GET", "/button4", true);
      xhr.send();
      }

      function button5Click() {
        console.log("pressed button4");
        var xhr = new XMLHttpRequest();
      xhr.open("GET", "/button5", true);
      xhr.send();
      }

function updateSliderPWM1(element) {
  var slider_value1 = document.getElementById("pwmSlider1").value;
  document.getElementById("textslider_value1").innerHTML = slider_value1;
  console.log(slider_value1);
  var xhr = new XMLHttpRequest();
  xhr.open("GET", "/slider1?value="+slider_value1, true);
  xhr.send();
}
function updateSliderPWM2(element) {
  var slider_value2 = document.getElementById("pwmSlider2").value;
  document.getElementById("textslider_value2").innerHTML = slider_value2;
  console.log(slider_value2);
  var xhr = new XMLHttpRequest();
  xhr.open("GET", "/slider2?value="+slider_value2, true);
  xhr.send();
}
function updateSliderPWM3(element) {
  var slider_value3 = document.getElementById("pwmSlider3").value;
  document.getElementById("textslider_value3").innerHTML = slider_value3;
  console.log(slider_value3);
  var xhr = new XMLHttpRequest();
  xhr.open("GET", "/slider3?value="+slider_value3, true);
  xhr.send();
}
</script>
</body>
</html>
)rawliteral";

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