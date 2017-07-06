
#ifndef __MOTOR_H_
#define __MOTOR_H_

#include "stm32f10x_gpio.h"
#include "stm32f10x_rcc.h"

//�������Ŷ���
#define MOTOR1_DIRECTION 	GPIO_Pin_3
#define MOTOR2_DIRECTION	GPIO_Pin_14

//���1�����꺯��
#define MOTOR1_DIRECRION(x)		GPIO_WriteBit(GPIOB, MOTOR1_DIRECTION, x);//PB3��DR1
#define MOTOR1_SPEED(x)				TIM_SetCompare1(TIM4,x);//PB6��Ve1
#define MOTOR1_FORWARD				Motor1_Foreward
#define MOTOR1_REVERSE				Motor1_Reversal
//���2�����꺯��
#define MOTOR2_DIRECRION(x)		GPIO_WriteBit(GPIOB, MOTOR2_DIRECTION, x);//PB14��DR2
#define MOTOR2_SPEED(x)				TIM_SetCompare2(TIM4,x);//PB7��Ve2
#define MOTOR2_FORWARD				Motor2_Foreward
#define MOTOR2_REVERSE				Motor2_Reversal

void Motor_MOS_Init(void);

void Motor_Init(uint16_t arr, uint16_t psc);
void Motor1_Foreward(uint16_t pwm);
void Motor1_Reversal(uint16_t pwm);
void Motor2_Foreward(uint16_t pwm);
void Motor2_Reversal(uint16_t pwm);

#endif
