#ifndef F_CPU
#define F_CPU 16000000UL
#endif

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

#define TRIG 		PD3
#define ECHO 		PD2
#define SRF05_DDR 	DDRD
#define SRF05_PORT 	PORTD
#define SRF05_PIN 	PIND

uint16_t distance;

volatile unsigned int mseconds = 0;
volatile uint8_t interrupt = 0;

void init_SRF05(void);
void SRF05_CalcDistance(void);

void init_SRF05(void){
	//CTC mode
	TCCR0A |= (1 << WGM01);
	OCR0A = 19;

	//COmpare match interrupt enable
	TIMSK0 |= (1 << OCIE0A);

	//External interrupt enable
	EIMSK |= (1 << INT0);

	SRF05_DDR |= (1 << TRIG);
	SRF05_DDR &= ~(1 << ECHO);
}

void SRF05_Start(void){
	
	mseconds = 0;
	TCNT0 = 0; //Reset timer0

	//Trigger for 12us
	SRF05_PORT |= (1 << TRIG);
	_delay_us(12);
	SRF05_PORT &= ~(1 << TRIG);

	//Interrupt on rising edge
	EICRA = (1 << ISC01) | (1 << ISC00);

	sei();
}

ISR(INT0_vect){

	if(interrupt == 0){
		//Start Timer
		TCCR0B |= (1 << CS01);

		//Interrupt on falling edge
		EICRA = (1 << ISC01);

		interrupt = 1;
	}else{
		//Stop Timer
		TCCR0B &= ~(1 << CS01);

		interrupt = 0;
		distance = mseconds / 58;
		SRF05_Start();
	}
}

ISR(TIMER0_COMPA_vect){
	mseconds += 10;
}