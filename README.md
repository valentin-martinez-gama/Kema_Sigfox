# kema-sigfox
Librería para mandar diversos comandos AT y recibir información de un módulo Wisol de Sigfox en zona RC2.

Al crear la instancia de la libreria declarar el pin de enable del modulo Wisol como argumento

Kema_Sigfox sigfox(7) para tarjeta NXTIoT_DEVKIT

Setting the verboseSerial variable to 1 will enable a Serial output of the library functions usefull for debugging but that should be turned off for production to avoid having to much traffic and interference in the Serial Bus.
libInstance.verboseSerial=1

Funciones disponibles:

setup(Optional enablePin) Inicializa el Serial al modulo Wisol e imprime en consola el DeviceID de la tarjeta

initPayload() Para empezar un nuevo mensaje a Sigfox

addFloat(varfloat)

addInt(varint, intSize) default intSize es 16 bits, funciona para int y uint

addBoolByte(Bxxxxxxxx) Agrega hata ocho valores booleanos en un byte. Funciona con menos de 8 tambien. En la decodificación del payload el bit de hasta la derecha es el 0, el segundo mas signifiativo el 1, etc...

sendMessage() Una vez construido el payload manda el mensaje al backend de Sigfox. Maxpayload = 12 bytes.

requestDownlink() Esta funcion hace un sendMessage + un request de downlink de el backend de Sigfox regresando la respuesta en una variable String con el pyaload del downlink en Hex. Regresa un variable string vacia si no hay respuesta.

getTemperature() Pide la temperatura al modulo Wisol y lo regresa en formato int. Los primeros dos digitos corresponden a la temperatura en Celsius y el tercer al primer decimal. Ej: 326 = 32.6 C

Parte de código desarrollado en el repo https://github.com/NXTIoT/NXTIoT_DEVKIT
