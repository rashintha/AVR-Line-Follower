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

#define Kp 27
#define Ki 0
#define Kd 20

#define MAX_SPEED 2000
#define RIGHT_MAX 1500 //Maximum Speed
#define LEFT_MAX 1500 //Maximum Speed
#define RIGHT_NORMAL_SPEED 1300 //Normal Speed
#define LEFT_NORMAL_SPEED 1300 //Normal Speed

#include "pwm.h"
#include "ir_array.h"
#include "srf05.h"

uint32_t count = 0;
uint8_t count2 = 0;
int P = 0, I = 0, D = 0, PID = 0;
int right_motor_pwm = 0, left_motor_pwm = 0;
//uint8_t channelA_status = FORWARD, channelB_status = FORWARD, left_status = FALSE, right_status=FALSE;
int previous_error = 0;

void controlMotor(void);
void setupMotor(void);

void setupMotor(void){
	MOTOR_DDR = 0x0F;

	pwm(CH_A, 0, MAX_SPEED);
	pwm(CH_B, 0, MAX_SPEED);

	//controlMotor(NEUTRAL);
}

void controlMotor(){

	count++;
	uint8_t ir_val = IR_ARR_VAL;

	if(count == 200000){
		putCh(distance);
		count = 0;
	}

	int error = getError();
	P = error;
	I +=  error;
	D = error - previous_error;
	previous_error = error;

	PID = Kp * P + Ki * I + Kd * D;

	right_motor_pwm = RIGHT_NORMAL_SPEED + PID;
	left_motor_pwm = LEFT_NORMAL_SPEED - PID;

	FORWARD_MODE();

	if(ir_val == 0x00){
		if(error > 1000 && count2 <= 5){
			REVERSE_MODE();
			pwm(CH_A, RIGHT_NORMAL_SPEED, MAX_SPEED);
			pwm(CH_B, LEFT_NORMAL_SPEED, MAX_SPEED);
			_delay_ms(100);
			FORWARD_MODE();
			pwm(CH_A, RIGHT_NORMAL_SPEED/2, MAX_SPEED);
			pwm(CH_B, LEFT_NORMAL_SPEED, MAX_SPEED);
			_delay_ms(10);
			count2++;
		}else if(error < -1000 && count2 <= 5){
			REVERSE_MODE();
			pwm(CH_A, RIGHT_NORMAL_SPEED, MAX_SPEED);
			pwm(CH_B, LEFT_NORMAL_SPEED, MAX_SPEED);
			_delay_ms(1000);
			FORWARD_MODE();
			pwm(CH_A, RIGHT_NORMAL_SPEED, MAX_SPEED);
			pwm(CH_B, LEFT_NORMAL_SPEED/2, MAX_SPEED);
			_delay_ms(100);
			count2++;
		}else{
			pwm(CH_A, 0, MAX_SPEED);
			pwm(CH_B, 0, MAX_SPEED);
		}

		return;
	}

	if(right_motor_pwm > RIGHT_MAX)
		right_motor_pwm = RIGHT_MAX;

	if(left_motor_pwm > LEFT_MAX)
		left_motor_pwm = LEFT_MAX;

	if(right_motor_pwm < 0)
		right_motor_pwm = 0;

	if(left_motor_pwm < 0)
		left_motor_pwm = 0;

	count2 = 0;

	pwm(CH_A, right_motor_pwm, MAX_SPEED);
	pwm(CH_B, left_motor_pwm, MAX_SPEED);
	/*switch(status){
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
			MOTOR_PORT = 0x00;
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
			//RIGHT_MODE();
			if(channelA < MIN_PWM_SPEED){
				left_status = FALSE;
				right_status = TRUE;
				channelA = 15000;
				channelB -= 30;
			}
			break;

		case LEFT:
			//LEFT_MODE();
			if(channelA < MIN_PWM_SPEED){
				left_status = TRUE;
				right_status = FALSE;
				channelA -= 30;
				channelB = MAX_PWM_SPEED / 2;
			}
			break;
	}

	pwm(CH_A, channelA, MAX_PWM_SPEED);
	pwm(CH_B, channelB, MAX_PWM_SPEED);
	*/
}