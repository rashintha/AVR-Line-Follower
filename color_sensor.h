#ifndef F_CPU
#define F_CPU 16000000UL
#endif

#include <avr/io.h>
#include <util/delay.h>

void init_ADC(void);
uint16_t read_ADC(void);

void init_ADC(void){
	//Set Reference to AVcc and select ADC channel 4
	ADMUX |= (1 << REFS0) | (1 << MUX2);

	/*
	 * Enable ADC 
	 * Set prescaler to 128 so input frequency will be 125kHz
	 * Enable ADC Interrupt
	 */
	ADCSRA |= (1 << ADEN) | (1 << ADPS0) | (1 << ADPS1) | (1 << ADPS2) | (1 << ADIE);

	//Enable ADC Noise Reduction Mode
	SMCR |= (1 << SM0);
}

uint16_t read_ADC(void){
	//Start Conversion
	ADCSRA |= (1 << ADSC);

	//Start Sleeping
	SMCR |= (1 << SE);

	return ADC;
}