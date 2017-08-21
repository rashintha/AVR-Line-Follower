#define IR_LEFT_DDR DDRD
#define IR_LEFT_PIN PIND

#define IR_L_01 PD2
#define IR_L_02 PD3
#define IR_L_03 PD4
#define IR_L_04 PD5

#define IR_RIGHT_DDR DDRB
#define IR_RIGHT_PIN PINB

#define IR_R_05 PB5
#define IR_R_06 PB3
#define IR_R_07 PB4
#define IR_R_08 PB0

#define IR01_VAL IR_LEFT_PIN & (1 << IR_L_01)
#define IR02_VAL IR_LEFT_PIN & (1 << IR_L_02)
#define IR03_VAL IR_LEFT_PIN & (1 << IR_L_03)
#define IR04_VAL IR_LEFT_PIN & (1 << IR_L_04)
#define IR05_VAL IR_RIGHT_PIN & (1 << IR_R_05)
#define IR06_VAL IR_RIGHT_PIN & (1 << IR_R_06)
#define IR07_VAL IR_RIGHT_PIN & (1 << IR_R_07)
#define IR08_VAL IR_RIGHT_PIN & (1 << IR_R_08)

#define IR_ARR_VAL IR01_VAL | IR02_VAL | IR03_VAL | IR04_VAL | IR05_VAL | IR06_VAL | IR07_VAL | IR08_VAL;

#define SETUP_IR() {IR_LEFT_DDR &= ~((1 << IR_L_01) | (1 << IR_L_02) | (1 << IR_L_03) | (1 << IR_L_04)); IR_RIGHT_DDR &= ~((1 << IR_R_05) | (1 << IR_R_06) | (1 << IR_R_07) | (1 << IR_R_08));}

#include "motor_control.h"
#include "serial.h"

uint8_t width = 0;
uint32_t count = 0;

void initIR(void);
void stableCar(void);
uint8_t lineSize(void);

void initIR(void){
	SETUP_IR();
	initUSART();
}

void stableCar(void){
	if(width == 0 || width >= 8){
		controlMotor(STOP);
	}else{
		
		controlMotor(FORWARD);
	}

	uint8_t size = lineSize();

	if(width == 0 && size != 0){
		width = size;
	}else if(width == size){
		count++;
	}

	if(count >= 255){
		width = size;

		if(width % 2 != 0)
			width --;
	}
}

uint8_t lineSize(void){
	uint8_t size = 0;

	if(~IR01_VAL) size++;
	if(~IR02_VAL) size++;
	if(~IR03_VAL) size++;
	if(~IR04_VAL) size++;
	if(~IR05_VAL) size++;
	if(~IR06_VAL) size++;
	if(~IR07_VAL) size++;
	if(~IR08_VAL) size++;

	return size;
}