/*
 * Kema_Sigfox.h  - Libreria para la comunicación por serial con el modulo Wisol
   de Sigfox.  Valentín Martínez Gama para - Kema Soluciones
 * 29 de Enero 2020
 *  Version 1.0
 */

#include "Arduino.h"
#include "Kema_Sigfox.h"

int _enablePin;
#define onDelay 300
#define transmissionDelay 700

Kema_Sigfox::Kema_Sigfox()
{}

void Kema_Sigfox::setup(int pin_enable_wisol_module){

  Serial.begin(9600);
  pinMode(pin_enable_wisol_module, OUTPUT);   //enable modulo wisol
  _enablePin = pin_enable_wisol_module;

  if (verboseSerial==1){
    digitalWrite(_enablePin, HIGH);
    _clearSerial();
    Serial.write("AT$I=10\n");
    delay(transmissionDelay);
    digitalWrite(_enablePin, LOW);
    char sigfoxID[9];
    Serial.readBytesUntil('\n', sigfoxID, 8);
    sigfoxID[9] = 0;
    Serial.println("ID de la tarjeta: ");
    Serial.println(sigfoxID);

    digitalWrite(_enablePin, HIGH);
    _clearSerial();
    Serial.write("AT$I=11\n");
    delay(transmissionDelay);
    digitalWrite(_enablePin, LOW);
    char sigfoxPAC[17];
    byte PACsize =Serial.readBytesUntil('\n', sigfoxPAC, 17);
    sigfoxPAC[PACsize] = 0;
    Serial.println("PAC de la tarjeta: ");
    Serial.println(sigfoxPAC);
  }

}


void Kema_Sigfox::initPayload()
{
  _ATmessage="AT$SF=";
}


void Kema_Sigfox::sendMessage()
{
  //agregar el caracter que marca el final de un comando AT al modulo"
  _ATmessage+="\n";
  //Habilitamos el modulo Sigfox
  digitalWrite(_enablePin, HIGH);
  //Limpiar la entrada de Serial del controlador de cualquier ruido generado
  _clearSerial();
  //Reset del canal para asegurar que manda en la frecuencia correcta
  Serial.write("AT$RC\n");
  //Enviamos la informacion por sigfox
  Serial.write(_ATmessage.c_str());
  delay(transmissionDelay);
  //deshabilitamos el modulo Sigfox
  digitalWrite(_enablePin, LOW);
}

String Kema_Sigfox::requestDownlink(){
  //Agregamos al final del payload el comando para hacer un downlink despues de enviar el mensaje
  _ATmessage+=",1";
  //Se manda el mensaje y una vez terminada la transmision se revisa el Serial para guardar el payload de respuesa
  sendMessage();
  String  downlinkPayload = Serial.readStringUntil('\n');

  if (verboseSerial==1){
    Serial.print("Donwlink data : ");
    Serial.println(downlinkPayload);
  }

  return downlinkPayload;
}


void Kema_Sigfox::addFloat(float varFloat) //funcion para agregar flotantes al payload
{
  byte* a1 = (byte*) &varFloat;    //convertimos el dato a bytes
  String strFloat;
  String hexaF="";
  for(int i=0; i<4; i++) {
    strFloat=String(a1[i], HEX);    //convertimos el valor hex a String
    if(strFloat.length() < 2) {
      hexaF+=0+strFloat;    //si no, se agrega un cero
    }
    else {
      hexaF+=strFloat;    //si esta completo, se copia tal cual
    }
  }
  _ATmessage+=hexaF;

  if (verboseSerial==1){
    Serial.print("Float payload = ");
    Serial.println(hexaF);
  }
}


void Kema_Sigfox::addInt(long varInt, int intSize)  //Indicar en la variable intSize el numero de bits deseados (debe ser multiplo de 4 )
{
  String hexaInt;
  hexaInt = String(varInt, HEX);
  //Comprobar si longitud del String creado es 4 Hexas o le faltan 0s a la izquierda
  while (hexaInt.length()<(intSize/4)) {
    hexaInt = String("0"+hexaInt);
  }

  if (hexaInt.length()>(intSize/4)){
    hexaInt = hexaInt.substring(hexaInt.length()-(intSize/4));
    if (verboseSerial==1){
      Serial.println("WARNING: Your int has been truncated to bits = "+ String(intSize));
      Serial.println("Check Readme to see how to increase bit limit");
    }
  }
    _ATmessage+=hexaInt;

    if (verboseSerial==1){
      Serial.print("Int payload = ");
      Serial.println(hexaInt);
    }
}

// !!! Parece haber errores random que cuentan el 2ndo bit = 0 !!!
void Kema_Sigfox::addBoolByte(bool boolArray[]){
//funcion para agregar hasta 8 valores bool a un byte del payload.
// En la decodificación del payload el primer bit es el 0, el segundo el 1, etc...el ultimo el 7.
int varBool = 0;
for(byte b=0; b<=7; b++){
    varBool <<= 1;
    varBool |= boolArray[b];
  }

  String hexaBool;
  hexaBool = String(varBool, HEX);
  //Comprobar si longitud del String creado es 2 Hexas o le faltan 0s a la izquierda
  while (hexaBool.length()<2) {
    hexaBool = String("0"+hexaBool);
  }

    _ATmessage+=hexaBool;

    if (verboseSerial==1){
      Serial.print("BoolByte payload = ");
      Serial.println(hexaBool);
    }
}


int Kema_Sigfox::getTemperature() {
  digitalWrite(_enablePin, HIGH);
  _clearSerial();
  Serial.write("AT$T?\n");
  delay(transmissionDelay);
  digitalWrite(_enablePin, LOW);
  int tempWisol = Serial.parseInt();
  if(verboseSerial==1){
    Serial.println("Temperatura modulo Wisol:");
    Serial.println(tempWisol);
  }
  return tempWisol;
}


String Kema_Sigfox::getATData(String ATcmd) {
  digitalWrite(_enablePin, HIGH);
  _clearSerial();
  ATcmd = ATcmd + '\n';
  Serial.write(ATcmd.c_str());
  delay(transmissionDelay);
  digitalWrite(_enablePin, LOW);
  String dataWisol = Serial.readStringUntil('\n');
  if(verboseSerial == 1){
    Serial.println("Data modulo Wisol:");
    Serial.println(dataWisol);
  }
  return dataWisol;
}


void Kema_Sigfox::_clearSerial(){
  Serial.flush();
  delay(onDelay);
  while(Serial.available()>0){
    Serial.read();
  }
}
