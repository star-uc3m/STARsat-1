#include <Arduino.h>

#include <SoftwareSerial.h>
#include <TinyGPS.h>
#include <SPI.h>
#include <VirtualWire.h>
#include <Adafruit_Sensor.h>
#include "Adafruit_BMP280.h"
#include <SD.h>
//#include <Wire.h>

//GPS
#define GPSTX 4
#define GPSRX 3

float lattitude, longitude; // create variable for latitude and longitude object
int year;
unsigned long age;
bool gpsOK = false;
byte month, day, hour, minutes, second;

SoftwareSerial gpsSerial(GPSTX, GPSRX); //TX RX
TinyGPS gps;                            // create gps object

void readGPS();

//ANTENA
#define DATAPIN 9
String packetA, packetB;
void sendMSG();
bool anteaOk = false;
//PRESSURE AND TEMP

float presion = 0;     // Almacena la presion atmosferica (Pa)
float temperatura = 0; // Almacena la temperatura (oC)
int altitud = 0;       // Almacena la altitud (m) (se puede usar variable float)
bool bmpPressTemp = false;
Adafruit_BMP280 bmp; // I2C

void tempPress();
//
#define BUTTONPIN 5
//
File logFile;
//

void setup()
{
  vw_setup(2000);         // velocidad: Bits per segundo
  vw_set_tx_pin(DATAPIN); // RF
  char buf[VW_MAX_MESSAGE_LEN];

  pinMode(BUTTONPIN, INPUT);
  delay(1000);
  while (true)
  {
    if (digitalRead(BUTTONPIN))
      break;
  }

  Serial.begin(115200); // connect serial
  Serial.println("Serial Ok: ");

  gpsSerial.begin(9600); // connect gps sensor
  Serial.println("GPS Ok: ");
  gpsOK = true;
  if (gpsOK)
  {
    strcpy(buf, "GPSOK");
    vw_send((uint8_t *)buf, strlen(buf)); //Enviamos el array
    vw_wait_tx();                         //Esperamos hasta que el mensaje se envie
    delay(200);
  }

  Serial.println("Transmitter Ok: ");
  anteaOk = true;
  if (anteaOk)
  {
    strcpy(buf, "RADIOOK");
    vw_send((uint8_t *)buf, strlen(buf)); //Enviamos el array
    vw_wait_tx();                         //Esperamos hasta que el mensaje se envie
    delay(200);
  }

  bmp.begin(); // Inicia el sensor
  Serial.println("Temp Ok: ");
  bmpPressTemp = true;
  if (bmpPressTemp)
  {
    strcpy(buf, "TEMPOK");
    vw_send((uint8_t *)buf, strlen(buf)); //Enviamos el array
    vw_wait_tx();                         //Esperamos hasta que el mensaje se envie
    delay(200);
  }
  /*
  Serial.print(F("Iniciando SD ..."));
  if (!SD.begin(6))
  {
    Serial.println(F("Error al iniciar"));
    return;
  }
  Serial.println(F("Iniciado correctamente"));

  logFile = SD.open("cansatdata.txt", FILE_WRITE);

  delay(10);                                                        // delay previo al funcionamiento !!!!!cambiar!!!
  vw_send((uint8_t *)"INICIANDO: \n\n", strlen("INICIANDO: \n\n")); //Enviamos el array
  vw_wait_tx();                                                     //Esperamos hasta que el mensaje se envie
  delay(200);
  */
  while (true)
  {
    if (digitalRead(BUTTONPIN))
      break;
  }
  //Wire.begin();
}

void loop()
{
  //readGPS();

  tempPress();

  sendMSG();
  /*
  Wire.beginTransmission(1);
  Wire.write("Hola");
  Wire.endTransmission();
  */
  delay(500);
}

void readGPS()
{
  Serial.println("Reading from GPS");
  bool newData = false;
  // Intentar recibir secuencia durante un segundo
  for (unsigned long start = millis(); millis() - start < 1000;)
  {
    while (gpsSerial.available())
    {
      char c = gpsSerial.read();
      if (gps.encode(c)) // Nueva secuencia recibida
        newData = true;
    }
  }
  Serial.println(newData);
  Serial.println("NEW DATA");
  if (newData)
  {
    gps.f_get_position(&lattitude, &longitude, &age);
    Serial.println("lat ok");
    gps.crack_datetime(&year, &month, &day, &hour, &minutes, &second);
    Serial.println("year ok");
  }
}

void tempPress()
{
  presion = bmp.readPressure() / 100;
  temperatura = bmp.readTemperature();
  altitud = bmp.readAltitude(1010); // Ajustar con el valor local
}

void sendMSG()
{
  char buf[VW_MAX_MESSAGE_LEN]; // Cadena para enviar
  //PAQUETES DE DATOS
  packetA = "Day " + String(day) + " Hour " + String(hour) + "z Lat " + String(lattitude, 6) + " Long " + String(longitude, 6) + String(year) + " " + String(day) + " " + String(month) + " " + String(hour + 2);
  //String maps = "https://maps.google.com/?q=" + String(lattitude, 6) + "," + String(longitude, 6);
  packetB = "Press " + String(presion, 5) + " hPa Temp " + String(temperatura, 2) + " ºC Alt " + String(altitud) + " m";

  packetA.toCharArray(buf, sizeof(buf)); //convertimos el String en un array
  vw_send((uint8_t *)buf, strlen(buf));  //Enviamos el array
  Serial.println(packetA);
  vw_wait_tx(); //Esperamos hasta que el mensaje se envie
  delay(200);
  /*
  maps.toCharArray(buf, sizeof(buf));   //convertimos el String en un array
  vw_send((uint8_t *)buf, strlen(buf)); //Enviamos el array
  Serial.println(maps);
  vw_wait_tx(); //Esperamos hasta que el mensaje se envie
  delay(200);
  */
  packetB.toCharArray(buf, sizeof(buf)); //convertimos el String en un array
  vw_send((uint8_t *)buf, strlen(buf));  //Enviamos el array
  Serial.println(packetB);
  vw_wait_tx(); //Esperamos hasta que el mensaje se envie
  delay(200);
}