/* Ejemplo basico de como usar la libreria Kema_Sigfox para mandar mensajes
   y controlar un modulo Wisol a traves del Serial.

  Por Gerardo Ortiz Cervantes @GeradoOC y Valentin Martinez Gama @valemgama
  1 Septiembre 2020
*/

#include <Kema_Sigfox.h>

int downlinkData = 0;
bool pendingDownlink = true;
int n = 0;

Kema_Sigfox ejemplo;

void setup() {
  // put your setup code here, to run once:

  //activamos la variable verboseSerial para que se impriman al Serial
  //valores relevantes. Util para debugg pero no se recomienda para produccion
  ejemplo.verboseSerial = 1;
  ejemplo.setup();
}

void loop() {
  // put your main code here, to run repeatedly:

  //Se inicializa un mensaje nuevo
  ejemplo.initPayload();

  //Se crea un boolArray que contenga 8 valores booleanos que podemos mandar en un BoolByte
  bool boolArray[8] = {0,0,0,0,0,0,0,0};
  //Se modifican algunos valores del Bool array antes de agregar payload
  //n es la variable de cuenta iteraciones del loop()
  //boolArray[2] = 1;
  boolArray[n%8] = 1;
  //boolArray[7-n%2] = 1;
  //Se agrega el arreglo de booleanos al payload.
  ejemplo.addBoolByte(boolArray);

  //Se pida la temperatura del modulo Wisol y se divide entre 10 para tenerla en grados centigrados
  float tempModulo = (ejemplo.getTemperature())/10;
  ejemplo.addFloat(tempModulo);
  
  String voltajeModulo = ejemplo.getATData("AT$V?");
  ejemplo.addInt(voltajeModulo.toInt());

  ejemplo.addInt(downlinkData);

  //checar si ya se recibio info de Downlink o si todavia se necesita pedirla.
  if (pendingDownlink){
    String dataString;
    dataString = ejemplo.requestDownlink();
    //downlinkData = dataString.toInt();
    pendingDownlink = false;
    Serial.println("Mensaje de Downlink:");
    Serial.println(dataString);
  }
  
  // Si ya se hizo un downlink ya nadamas se manda un mensaje sin pedir respuesta
  else {
    ejemplo.sendMessage();
  }

  n++;
  //Agregar una cuenta al contador de iteraciones y esperar 4s
  delay(4000);
}
