#include <Arduino.h>
#include <SD.h>
#include <Wire.h>

File myFile;

void receive(char data[]);
char msg[80];
void setup()
{
  Wire.begin(1);
  Serial.begin(9600);
  Serial.print("Iniciando SD ...");
  if (!SD.begin(6))
  {
    Serial.println("No se pudo inicializar");
    return;
  }
  Serial.println("inicializacion exitosa");

  myFile = SD.open("archivo.txt"); //abrimos  el archivo
  if (myFile)
  {
    Serial.println("archivo.txt:");
    while (myFile.available())
    {
      Serial.write(myFile.read());
    }
    myFile.flush(); //cerramos el archivo
  }
  else
  {
    Serial.println("Error al abrir el archivo");
  }
}

void loop()
{
  Wire.onReceive(receive);
  myFile = SD.open("archivo.txt"); //abrimos  el archivo
  myFile.print("Time(ms)=");
  myFile.print(millis());
  myFile.flush();
  Serial.println("archivo.txt:");
  while (myFile.available())
  {
    Serial.write(myFile.read());
  }
}

void receive(char data[])
{
  strcpy(msg, data);
}