#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_MCP23017.h>

Adafruit_MCP23017 mcp;

void setup() {
  Serial.begin(9600);
 
  pinMode(2,INPUT);
  Wire.begin();
  mcp.begin();
  mcp.pinMode(0, OUTPUT);
  mcp.pinMode(1, OUTPUT);
  mcp.pinMode(2, OUTPUT);
  mcp.pinMode(3, OUTPUT);
  pinMode(A0, INPUT);
  mcp.digitalWrite(0, LOW);
  mcp.digitalWrite(1, LOW);
  while (!digitalRead(2));
}

void loop() {
  delay(1000);
  mcp.digitalWrite(1, HIGH);
  while (!digitalRead(2)){
      mcp.digitalWrite(2,LOW);
      mcp.digitalWrite(3,LOW);
      Serial.print("PORT "); Serial.print(1); Serial.print(" "); Serial.print(5.0*analogRead(A0)/1023.0); Serial.print(" V ");
      mcp.digitalWrite(2,HIGH);
      mcp.digitalWrite(3,LOW);
      Serial.print("PORT "); Serial.print(2); Serial.print(" "); Serial.print(5.0*analogRead(A0)/1023.0); Serial.print(" V ");
      mcp.digitalWrite(2,LOW);
      mcp.digitalWrite(3,HIGH);
      Serial.print("PORT "); Serial.print(3); Serial.print(" "); Serial.print(5.0*analogRead(A0)/1023.0); Serial.print(" V ");
      mcp.digitalWrite(2,HIGH);
      mcp.digitalWrite(3,HIGH);
      Serial.print("PORT "); Serial.print(4); Serial.print(" "); Serial.print(5.0*analogRead(A0)/1023.0); Serial.println(" V");
    
  }
  Serial.println("CORREEEEEEEE");
  mcp.digitalWrite(2,LOW);
  mcp.digitalWrite(3,LOW);
  for (int i = 0; i < 10; i++)
  {
    
    mcp.digitalWrite(1, HIGH);
    Serial.print("T-"); Serial.print(10-i);
    delay(1000);
    mcp.digitalWrite(1, LOW);
    delay(150);
    Serial.print("\tCONT "); Serial.print(5.0*analogRead(A0)/1023.0); Serial.println(" V");
  }
  mcp.digitalWrite(2, LOW);
  mcp.digitalWrite(0, HIGH);
  Serial.println("QUEMANDO");
  for (int i = 1; i < 11; i++)
  {
   Serial.print(i); Serial.print("\tCONT "); Serial.print(5000.0*analogRead(A0)/1023.0); Serial.println(" mV");
    delay(1000);
  }
  mcp.digitalWrite(0, LOW);
  mcp.digitalWrite(2, HIGH);
  Serial.println("QUEMADO");
}