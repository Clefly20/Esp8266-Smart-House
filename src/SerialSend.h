#include <Arduino.h>
#include <SoftwareSerial.h>

int Xdata1,Xdata2,Xdata3,Xdata4,Xdata5; // Relay Position - LED1 - LED2 - LED3 - Empty
SoftwareSerial NodeMcu_SoftSerial(D1,D2); // RX - TX
SoftwareSerial PowerSupply_SoftSerial(D3,D4); // RX - TX

void setup1(){
    NodeMcu_SoftSerial.begin(31250); // Baudrate 31250
    PowerSupply_SoftSerial.begin(31250); // Baudrate 31250
    Serial.begin(31250);
}


void SendLedControl(int Xdate1,int Xdate2,int Xdate3,int Xdate4,int Xdate5){
Xdata1 = Xdate1;
Xdata2 = Xdate2;
Xdata3 = Xdate3;
Xdata4 = Xdate4;
Xdata5 = Xdate5;


NodeMcu_SoftSerial.print(Xdata1); NodeMcu_SoftSerial.print("A");
NodeMcu_SoftSerial.print(Xdata2); NodeMcu_SoftSerial.print("B");
NodeMcu_SoftSerial.print(Xdata3); NodeMcu_SoftSerial.print("C");
NodeMcu_SoftSerial.print(Xdata4); NodeMcu_SoftSerial.print("D");
NodeMcu_SoftSerial.print(Xdata5); NodeMcu_SoftSerial.print("E");
NodeMcu_SoftSerial.print("/n");
}

void Opened(){
    Serial.print("ON");
}

void Closed(){
    Serial.print("OFF");
}