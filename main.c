#define F_CPU 16000000UL

//Comment the MCU which you're not using.
#define ATMEGA_328P
//#define ATMEGA_32A

#include <avr/io.h>
#include <util/delay.h>
#include "ir_array.h"

int main(void){

	setupPWM();
	setupMotor();
	initIR();

	while(1){
		stableCar();
	}
	
	return 0;
}