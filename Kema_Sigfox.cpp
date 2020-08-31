/*
 * Kema_Sigfox.h  - Libreria para la comunicación por serial con el modulo Wisol
   de Sigfox.  Valentín Martínez Gama para - Kema Soluciones
 * 29 de Enero 2020
 *  Version 1.0
 */

#include "Arduino.h"
#include "Kema_Sigfox.h"

Kema_Sigfox::Kema_Sigfox(int pin_enable_wisol_module)
{
  Serial.begin(9600);
  pinMode(pin_enable_wisol_module, OUTPUT);   //enable modulo wisol
  _enablePin = pin_enable_wisol_module;

  digitalWrite(_enablePin, HIGH);
  delay(10);
  Serial.print("AT$I=10\n");
  delay(500);
  String sigfoxID = Serial.readStringUntil("\n");
  Serial.println("\n"+ sigfoxID);
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

String Kema_Sigfox::requestDownlink(){
  //Agregamos al final del payload el comando para hacer un downlink despues de enviar el mensaje
  _ATmessage+=",1";
  //Nos aseguremos de que el incoming buffer del Serial quede vacio
  while (Serial.available() > 0) {
    Serial.read();
  }
  //Se manda el mensaje y una vez terminada la transmision se revisa el Serial para guardar el payload de respuesa
  sendMessage();

  String downlinkPayload = "";

  while (Serial.available() > 0)
    if (Serial.peek() != "\n") {
        downlinkPayload += Serial.read();
    }
    else {
      return downlinkPayload;
    }
}

void Kema_Sigfox::addFloat(float varFloat) //funcion para agregar flotantes al payload
{
  byte* a1 = (byte*) &varFloat;    //convertimos el dato a bytes
  String strFloat;
  String hexaF="";
  for(int i=0;i<4;i++)
  {
    strFloat=String(a1[i], HEX);    //convertimos el valor hex a String
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
  hexaInt = String(varInt, HEX);
  //Comprobar si longitud del String creado es 4 Hexas o le faltan 0s a la izquierda
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
  //Comprobar si longitud del String creado es 4 Hexas o le faltan 0s a la izquierda
  while (hexaBool.length()<4) {
    hexaBool = String("0"+hexaBool);
  }
  _ATmessage+=hexaBool;
}
