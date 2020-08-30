/*
Kema_Sigfox.h  - Libreria para la comunicación por Sigfox con el Devkit de NXTIoT
								 adaptado por Valentín Martínez para - Kema Soluciones
  21 de Enero 2020
  Para uso de Kema.
*/
#ifndef Kema_Sigfox_h
#define Kema_Sigfox_h

#include "Arduino.h"

#define enable_module 7

class Kema_Sigfox
{
	public:
	Kema_Sigfox();
	void initpayload();
	void sendmessage();
	void addint(int varI);
	void addfloat(float varF);
	String bufer; //variable donde guardaremos nuestro payload
};

#endif
