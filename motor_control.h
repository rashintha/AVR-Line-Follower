#define FORWARD 0x00
#define REVERSE	0x01
#define RIGHT	0x02
#define LEFT	0x03
#define STOP 	0x04
#define NEUTRAL	0x05

#define TRUE 	1
#define FALSE 	0

#define MOTOR_DDR	DDRC
#define MOTOR_PORT	PORTC

#define MOTOR_RIGHT_FWD_PIN		PC0
#define MOTOR_RIGHT_REV_PIN		PC1
#define MOTOR_LEFT_FWD_PIN		PC2
#define MOTOR_LEFT_REV_PIN		PC3

#define FORWARD_MODE() { MOTOR_PORT |= (1 << MOTOR_RIGHT_FWD_PIN) | (1 << MOTOR_LEFT_FWD_PIN); MOTOR_PORT &= ~((1 << MOTOR_RIGHT_REV_PIN) | (1 << MOTOR_LEFT_REV_PIN)); }
#define REVERSE_MODE() { MOTOR_PORT |= (1 << MOTOR_RIGHT_REV_PIN) | (1 << MOTOR_LEFT_REV_PIN); MOTOR_PORT &= ~((1 << MOTOR_RIGHT_FWD_PIN) | (1 << MOTOR_LEFT_FWD_PIN)); }
#define RIGHT_MODE()   { MOTOR_PORT |= (1 << MOTOR_RIGHT_REV_PIN) | (1 << MOTOR_LEFT_FWD_PIN); MOTOR_PORT &= ~((1 << MOTOR_RIGHT_FWD_PIN) | (1 << MOTOR_LEFT_REV_PIN)); }
#define LEFT_MODE()    { MOTOR_PORT |= (1 << MOTOR_RIGHT_FWD_PIN) | (1 << MOTOR_LEFT_REV_PIN); MOTOR_PORT &= ~((1 << MOTOR_RIGHT_REV_PIN) | (1 << MOTOR_LEFT_FWD_PIN)); }

#define MAX_PWM_SPEED	20000
#define MIN_PWM_SPEED	2000

#define MAX_SPEED 10000
#define SPEED_INCREASE_RATE		10
#define SPEED_DECREASE_RATE		5
#define BREAK_RATE				30

#include "pwm.h"

uint16_t channelA = 0, channelB = 0;
uint8_t channelA_status = FORWARD, channelB_status = FORWARD, left_status = FALSE, right_status=FALSE;

void controlMotor(uint8_t status);
void setupMotor(void);

void setupMotor(void){
	MOTOR_DDR = 0x0F;

	pwm(CH_A, 0, MAX_PWM_SPEED);
	pwm(CH_B, 0, MAX_PWM_SPEED);

	controlMotor(NEUTRAL);
}

void controlMotor(uint8_t status){
	switch(status){
		case FORWARD:
			if(channelA_status == FORWARD && channelB_status == FORWARD){
				(channelA == MAX_SPEED) ? (channelA = MAX_SPEED) : (channelA += SPEED_INCREASE_RATE);
				channelB = channelA;
			}else if(channelA_status == REVERSE && channelB_status == REVERSE){
				if(channelA > MIN_PWM_SPEED){
					channelA -= BREAK_RATE;
					channelB = channelA;
				}else{
					FORWARD_MODE();
					channelA_status = FORWARD;
					channelB_status = FORWARD;
				}
			}
			break;
		
		case REVERSE:
			if(channelA_status == REVERSE && channelB_status == REVERSE){
				(channelA == MAX_SPEED) ? (channelA = MAX_SPEED) : (channelA += SPEED_INCREASE_RATE);
				channelB = channelA;
			}else if(channelA_status == FORWARD && channelB_status == FORWARD){
				if(channelA > MIN_PWM_SPEED){
					channelA -= BREAK_RATE;
					channelB = channelA;
				}else{
					REVERSE_MODE();
					channelA_status = REVERSE;
					channelB_status = REVERSE;
				}
			}
			break;
		
		case STOP:
			channelA = 0x00;
			channelB = 0x00;
			break;

		case NEUTRAL:
			if(channelA > MIN_PWM_SPEED){
				channelA -= SPEED_DECREASE_RATE;
				channelB = channelA;
			}

			if(channelB_status == FORWARD && channelB_status == FORWARD){
				FORWARD_MODE();
			}
			else{
				REVERSE_MODE();
			}

			if(left_status || right_status){
				channelA = 0x00;
				channelB = 0x00;
				left_status = FALSE;
				right_status = FALSE;
			}
			break;

		case RIGHT:
			RIGHT_MODE();
			if(channelA < MIN_PWM_SPEED){
				left_status = FALSE;
				right_status = TRUE;
				channelA = 15000;
				channelB = 15000;
			}
			break;

		case LEFT:
			LEFT_MODE();
			if(channelA < MIN_PWM_SPEED){
				left_status = TRUE;
				right_status = FALSE;
				channelA = MAX_PWM_SPEED / 2;
				channelB = MAX_PWM_SPEED / 2;
			}
			break;
	}

	pwm(CH_A, channelA, MAX_PWM_SPEED);
	pwm(CH_B, channelB, MAX_PWM_SPEED);
}