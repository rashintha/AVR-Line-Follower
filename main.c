#define F_CPU 16000000UL

//Comment the MCU which you're not using.
#define ATMEGA_328P
//#define ATMEGA_32A

#include <avr/io.h>
#include <util/delay.h>
#include "motor_control.h"

int main(void){

	setupPWM();
	setupMotor();
	initIR();
	init_SRF05();
	SRF05_Start();

	while(1){
		controlMotor();
	}
	
	return 0;
}