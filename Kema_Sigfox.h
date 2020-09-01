/*
Kema_Sigfox.h  - Libreria para la comunicación por serial con el modulo Wisol de
							  Sigfox serial.  Valentín Martínez Gama para - Kema Soluciones
  01 de septiembre del 2020
	Version 1.0
*/

#ifndef Kema_Sigfox_h
#define Kema_Sigfox_h

#include "Arduino.h"

class Kema_Sigfox {
	public:

		Kema_Sigfox();

		void setup(int pin_enable_wisol_module = 7);
		void initPayload();
		void sendMessage();
		void addInt(long varInt, int intSize = 16);
		void addFloat(float varFloat);
		void addBoolByte(int varBool);
		int getTemperature();
		String getATData(String ATcmd);
		String requestDownlink();
		bool verboseSerial = 0;

	private:
			int _enablePin;
			String _ATmessage; //variable donde guardaremos nuestro payload
			void _clearSerial();
};

#endif
