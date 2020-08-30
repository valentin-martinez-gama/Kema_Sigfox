/*
Kema_Sigfox.h  - Libreria para la comunicación por serial con el modulo Wisol de
							  Sigfox serial.  Valentín Martínez Gama para - Kema Soluciones
  29 de Enero 2020
	Version 1.0
*/

#ifndef Kema_Sigfox_h
#define Kema_Sigfox_h

#include "Arduino.h"

class Kema_Sigfox {
	public:

		Kema_Sigfox(int pin_enable_wisol_module);

		void initPayload();
		void sendMessage();
		void addInt(int varInt, int intSize = 16);
		void addFloat(float varFloat);
		void addBoolByte(int varBool);

	private:
			int _enablePin;
			string _ATmessage; //variable donde guardaremos nuestro payload
};

#endif
