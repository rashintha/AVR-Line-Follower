#define F_CPU 16000000UL

//Comment the MCU which you're not using.
#define ATMEGA_328P
//#define ATMEGA_32A

#include <avr/io.h>
#include <util/delay.h>

#include "color_sensor.h"
#include "motor_control.h"

uint32_t count3 = 0;

int main(void){

	setupPWM();
	setupMotor();
	initIR();
	init_SRF05();
	SRF05_Start();
	init_ADC();

	while(1){
		count3++;
		controlMotor();
		if(count3 == 50000){
			putCh(6666);
			putCh(6666 >> 8);
			putCh(5555);
			putCh(5555 >> 8);
			count3 = 0;
		}
	}
	
	return 0;
}