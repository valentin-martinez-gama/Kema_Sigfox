# kema-sigfox
Librería para mandar diversos comandos AT y recibir información de un módulo Wisol de Sigfox en zona RC2.

Al crear la instancia de la libreria declarar el pin de enable del modulo Wisol como argumento

Kema_Sigfox sigfox(7) para tarjeta NXTIoT_DEVKIT

Funciones disponibles:

initPayload() Para empezar un nuevo mensaje a Sigfox

addFloat(varfloat)

addInt(varint, intSize) default intSize es 16 bits, funciona para int y uint

addBoolByte(Bxxxxxxxx) Agrega hata ocho valores booleanos en un byte. Funciona con menos de 8 tambien. En la decodificación del payload el bit de hasta la derecha es el 0, el segundo mas signifiativo el 1, etc...

sendMessage() Una vez construido el payload manda el mensaje al backend de Sigfox. Maxpayload = 12 bytes.

Parte de código desarrollado en el repo https://github.com/NXTIoT/NXTIoT_DEVKIT
