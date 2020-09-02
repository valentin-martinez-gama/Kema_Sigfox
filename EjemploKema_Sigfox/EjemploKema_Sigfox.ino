/* Ejemplo basico de como usar la libreria Kema_Sigfox para mandar mensajes 
   y controlar un modulo Wisol a traves del Serial.

  Por Gerardo Ortiz Cervantes @GeradoOC
  1 Septiembre 2020
*/

#include Kema_Sigfox.h

int downlinkData = 0;
bool pendingDownlink = true;

Kema_Sigfox ejemplo 
void setup() {
  // put your setup code here, to run once:

  //activamos la variable verboseSerial para que se impriman al Serial 
  //valores relevantes. Util para debugg pero no se recomienda para produccion
  ejemplo.verboseSerial = 1;
  ejemplo.setup();  
}

void loop() {
  // put your main code here, to run repeatedly:
  String boolString = "B";
  
  //Se inicializa un mensaje nuevo 
  ejemplo.initPayload();
  
  //Se pida la temperatura del modulo Wisol y se divide entre 10 para tenerla en grados centigrados
  float tempModulo = (ejemplo.getTemperature())/10;
  ejemplo.addFloat(tempModulo);

  //Condiciones para agregar bools a boolString que luego mandaremos como un boolByte
  if (true) {
    boolString += "1";
  }

  if (false)
  {}
  else{
    boolString = "0";
  }

  boolString += random()%2

  
  //checar si ya se recibio info de Downlink o si todavia se necesita pedirla.
  if (pendingDownlink){
    String dataString;
    dataString = ejemplo.requestDownlink();
    downlinkData = dataString.toInt(); 
    pendingDownlink = false;
  }
  
  // Si ya se hizo un downlink ya nadamas se manda un mensaje sin pedir respuesta
  else {
    ejemplo.sendMessage();
  }

  delay(4000);
}
