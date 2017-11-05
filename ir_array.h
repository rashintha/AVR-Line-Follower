#ifndef F_CPU
#define F_CPU 16000000UL
#endif

#define IR_LEFT_DDR DDRD
#define IR_LEFT_PIN PIND

#define IR_L_01 PD4
#define IR_L_02 PD5
#define IR_L_03 PD6
#define IR_L_04 PD7

#define IR_RIGHT_DDR DDRB
#define IR_RIGHT_PIN PINB

#define IR_R_05 PB5
#define IR_R_06 PB3
#define IR_R_07 PB4
#define IR_R_08 PB0

#define IR01_VAL() ({ uint8_t retVal = (IR_LEFT_PIN & (1 << IR_L_01)) ? (1 << 0) : 0; retVal;})
#define IR02_VAL() ({uint8_t retVal = (IR_LEFT_PIN & (1 << IR_L_02)) ? (1 << 1) : 0; retVal;})
#define IR03_VAL() ({uint8_t retVal = (IR_LEFT_PIN & (1 << IR_L_03)) ? (1 << 2) : 0; retVal;})
#define IR04_VAL() ({uint8_t retVal = (IR_LEFT_PIN & (1 << IR_L_04)) ? (1 << 3) : 0; retVal;})
#define IR05_VAL() ({uint8_t retVal = (IR_RIGHT_PIN & (1 << IR_R_05)) ? (1 << 4) : 0; retVal;})
#define IR06_VAL() ({uint8_t retVal = (IR_RIGHT_PIN & (1 << IR_R_06)) ? (1 << 5) : 0; retVal;})
#define IR07_VAL() ({uint8_t retVal = (IR_RIGHT_PIN & (1 << IR_R_07)) ? (1 << 6) : 0; retVal;})
#define IR08_VAL() ({uint8_t retVal = (IR_RIGHT_PIN & (1 << IR_R_08)) ? (1 << 7) : 0; retVal;})

#define IR_ARR_VAL IR01_VAL() | IR02_VAL() | IR03_VAL() | IR04_VAL() | IR05_VAL() | IR06_VAL() | IR07_VAL() | IR08_VAL()

#define IR_POSITION IR01_VAL() * 0 | IR02_VAL() * 1000 | IR03_VAL() * 2000 | IR04_VAL() * 3000 | IR05_VAL() * 4000 | IR06_VAL() * 5000 | IR07_VAL() * 6000 | IR08_VAL() * 7000

#define SETUP_IR() {IR_LEFT_DDR &= ~((1 << IR_L_01) | (1 << IR_L_02) | (1 << IR_L_03) | (1 << IR_L_04)); IR_RIGHT_DDR &= ~((1 << IR_R_05) | (1 << IR_R_06) | (1 << IR_R_07) | (1 << IR_R_08));}

#include "serial.h"

uint8_t width = 0;

void initIR(void);
int getPosition(void);
int getError(void);
 
void initIR(void){
	SETUP_IR();
	initUSART();
}

int getPosition(void){
	int position = IR_POSITION / 2;

	if(position > 7000){
		while(position >= 7000){
			position /= 2;
		}
	}

	return position;

}

int getError(void){
	return 3500 - getPosition();
}