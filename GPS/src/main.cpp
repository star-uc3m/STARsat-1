#include <Arduino.h>
#include <SoftwareSerial.h>
#include <TinyGPS.h>
#include <VirtualWire.h>
/************************************************************************************************/
//GPS
TinyGPS gps;
#define GPSTX 4
#define GPSRX 3

SoftwareSerial softSerial(GPSTX, GPSRX); //TX RX

float flat, flon;
unsigned long age;
/************************************************************************************************/
//ANTENA
#define DATAPIN 9
char dato[100];
void setup()
{
   Serial.begin(115200);
   softSerial.begin(9600);
   vw_setup(2000);         // velocidad: Bits per segundo
   vw_set_tx_pin(DATAPIN); // RF
   delay(1000);
   Serial.println("INCIALIZANDO: ");
}

void loop()
{
   bool newData = false;

   // Intentar recibir secuencia durante un segundo
   for (unsigned long start = millis(); millis() - start < 1000;)
   {
      while (softSerial.available())
      {
         char c = softSerial.read();
         if (gps.encode(c)) // Nueva secuencia recibida
            newData = true;
      }
   }

   if (newData)
   {
      Serial.print("LAT: ");
      gps.f_get_position(&flat, &flon, &age);
      Serial.print(flat == TinyGPS::GPS_INVALID_F_ANGLE ? 0.0 : flat, 6);
      Serial.print("\tLON: ");
      Serial.print(flon == TinyGPS::GPS_INVALID_F_ANGLE ? 0.0 : flon, 6);
      Serial.print("\tSAT: ");
      Serial.print(gps.satellites() == TinyGPS::GPS_INVALID_SATELLITES ? 0 : gps.satellites());
      Serial.print("\tALT: ");
      Serial.print(gps.f_altitude());
      Serial.print("\tSpeed: ");
      Serial.print(gps.f_speed_mps());
      int year;
      byte month, day, hour, minutes, second;
      gps.crack_datetime(&year, &month, &day, &hour, &minutes, &second);
      char date[100];
      sprintf(date, "\tFecha: %02d/%02d/%02d\tHora (ESPAÑA UTC): %02d:%02d:%02d", day, month, year, hour, minutes, second);
      Serial.println(date);
      Serial.print("https://maps.google.com/?q=");
      Serial.print(flat, 6);
      Serial.print(",");
      Serial.println(flon, 6);
   }
   char buf[VW_MAX_MESSAGE_LEN]; // Cadena para enviar
   String str = "LA NASA TE ESPIA";
   //Serial.println(str);
   str.toCharArray(buf, sizeof(buf));    //convertimos el String en un array
   vw_send((uint8_t *)buf, strlen(buf)); //Enviamos el array
   vw_wait_tx();                         //Esperamos hasta que el mensaje se envie

   delay(200);
}