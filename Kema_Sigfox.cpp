/*
 * Kema_Sigfox.h  - Libreria para la comunicación por serial con el modulo Wisol
   de Sigfox.  Valentín Martínez Gama para - Kema Soluciones
 * 29 de Enero 2020
 *	Version 1.0
 */

#include "Arduino.h"
#include "Kema_Sigfox.h"

Kema_Sigfox::Kema_Sigfox(int pin_enable_wisol_module)
{
	Serial.begin(9600);
	pinMode(pin_enable_wisol_module, OUTPUT);   //enable modulo wisol
	_enablePin = pin_enable_wisol_module;
}

void Kema_Sigfox::initPayload()
{
	_ATmessage="AT$SF=";
}

void Kema_Sigfox::sendMessage()
{
  //agregamos el salto de linea "\n"
  _ATmessage+="\n";
  //*******************
  //Habilitamos el modulo Sigfox
  digitalWrite(_enablePin, HIGH);
  delay(1000);
  //Reset del canal para asegurar que manda en la frecuencia correcta
  Serial.print("AT$RC\n");
  //************************
  //Enviamos la informacion por sigfox
  Serial.print(_ATmessage);
  delay(3000);
  //deshabilitamos el modulo Sigfox
  digitalWrite(_enablePin, LOW);
}

void Kema_Sigfox::addFloat(float varFloat) //funcion para agregar flotantes al payload
{
  byte* a1 = (byte*) &varF;    //convertimos el dato a bytes
  String strFloat;
	String hexaF="";
  for(int i=0;i<4;i++)
  {
    strFloat=String(a1[i], HEX);    //convertimos el valor hex a string
    if(strFloat.length()<2)
    {
      hexaF+=0+strFloat;    //si no, se agrega un cero
    }
    else
    {
      hexaF+=strFloat;    //si esta completo, se copia tal cual
    }
  }
	_ATmessage+=hexaF;
}

void Kema_Sigfox::addInt(int varInt, int intSize = 16)    //funcion para agregar enteros al payload (hasta 8 bits)
{
	String hexaInt;
  hexaInt = String(varI, HEX);
  //Comprobar si longitud del string creado es 4 Hexas o le faltan 0s a la izquierda
  while (hexaInt.length()<(intSize/4)) {
    hexaInt = String("0"+hexaInt);
  }
	_ATmessage+=hexaInt;
}

void Kema_Sigfox::addBoolByte(int varBool)    //funcion para agregar hasta 8 valores bool a un byte del payload.

//Pasar argumentos en la forma Bxxxxxxxx Ej: B1101

// En la decodificación del payload el bit de hasta la derecha es el 0, el segundo mas signifiativo el 1, etc...
{
	String hexaBool;
  hexaBool = String(varBool, HEX);
  //Comprobar si longitud del string creado es 4 Hexas o le faltan 0s a la izquierda
  while (hexaBool.length()<4) {
    hexaBool = String("0"+hexaBool);
  }
	_ATmessage+=hexaBool;
}
