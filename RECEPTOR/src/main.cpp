#include <Arduino.h>
/*
#include <VirtualWire.h>

void setup()
{
  Serial.begin(9600); // Debugging only
  Serial.println("Receptor: setup");

  // Se inicializa el RF
  vw_setup(2000);   // velocidad: Bits per segundo
  vw_set_rx_pin(9); //Pin como entrada del RF
  vw_rx_start();    // Se inicia como receptor
}
int num = 0;
void loop()
{
  uint8_t buf[VW_MAX_MESSAGE_LEN];
  uint8_t buflen = VW_MAX_MESSAGE_LEN;
  //verificamos si hay un dato valido en el RF
  if (vw_get_message((uint8_t *)buf, &buflen))
  {
    int i;
    String DatoCadena = "";
    for (i = 0; i < buflen; i++)
    {
      DatoCadena.concat((char)buf[i]);
    }

    Serial.println("#: ");
    Serial.println(num);
    Serial.println(DatoCadena);
    num++;
  }
}
*/

#include <VirtualWire.h>

const int dataPin = 8;
#define GPSOK 2
#define RADIOOK 3
#define TEMPOK 4
void setup()
{
  pinMode(2,OUTPUT);
  pinMode(3,OUTPUT);
  pinMode(4,OUTPUT);
  Serial.begin(9600);
  vw_setup(2000);
  vw_set_rx_pin(dataPin);
  vw_rx_start();
  Serial.println("Starting");
}

void loop()
{
  char buf[VW_MAX_MESSAGE_LEN];
  uint8_t buflen = VW_MAX_MESSAGE_LEN;
  if (vw_get_message((uint8_t *)buf, &buflen))
  {
    if(!strcmp(buf,"GPSOK"))
      digitalWrite(2,HIGH);
    if(!strcmp(buf,"RADIOOK"))
      digitalWrite(3,HIGH);
    if(!strcmp(buf,"TEMPOK"))
      digitalWrite(2,HIGH);
    int i;
    Serial.print("Received: ");
    for (i = 0; i < buflen; i++)
    {
      Serial.print(char(buf[i]));
    }
    Serial.println('\r');
  }
}