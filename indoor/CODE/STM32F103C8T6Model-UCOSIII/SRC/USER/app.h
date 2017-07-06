
#ifndef __APP_H
#define __APP_H

#include <string.h>
#include "stm32f10x.h"
#include "led.h"
#include "tim.h"
#include "usart.h"
#include "delay.h"
#include "sys.h"
#include "spi.h"
#include "stdio.h"
#include "PC_Link.h"
#include <stdlib.h>
#include "includes.h"
#include "infrared.h"
#include "hal_uart.h"
#include "hal_key.h"
#include "gizwits_product.h"
#include "gizwits_protocol.h"
#include "watchdog.h"
#include "hal_watchdog.h"
#include "nrf24l01.h"
#include "as608.h"
#include "key.h"
#include "stm32flash.h"
#include "list.h"
#include "malloc.h"
#include "adc.h"
#include "motor.h"

#define LED PBout(14)

typedef struct EVEN_MUTEX{
	uint8_t switchdoor;
	uint8_t latch_lock;
	uint8_t flag;
	uint16_t delay_time;
	uint8_t os_flag;
}Gozwits_Even_Mutex;

#define MOTOT_DELAY		7900
#define MOTOR_LOCK		1500

/**@name 开始任务
*@{
*/
#define START_TASK_PRIO	3
#define START_STK_SIZE	128
extern OS_TCB	StartTackTCB;
extern CPU_STK	START_TASK_STK[START_STK_SIZE];
void start_task(volatile void *p_arg);
/**@}
*/

/*任务4*/
//任务MOTOR优先级
#define MOTOR_TASK_PRIO	4
//任务MOTOR堆栈大小
#define MOTOR_STK_SIZE	128
//任务MOTOR控制块
extern OS_TCB MOTORTCB;
//任务MOTOR堆栈
extern CPU_STK	MOTOR_STK[MOTOR_STK_SIZE];
//任务控制函数
void Motor_task(volatile void *p_arg);

/**@name Gizwits任务
*@{
*/
#define Gizwits_TASK_PRIO	5
#define Gizwits_STK_SIZE	256
extern OS_TCB GizwitsTCB;
extern CPU_STK	Gizwits_STK[Gizwits_STK_SIZE];
void Gizwits_task(volatile void *p_arg);
/**@}
*/

/**@name NRF24L01任务
*@{
*/
#define NRF24L01_TASK_PRIO 6
#define NRF24L01_STK_SIZE  128
extern OS_TCB NRF24L01TCB;
extern CPU_STK NRF24L01_STK[NRF24L01_STK_SIZE];
void NRF24L01_task(volatile void *p_arg);
/**@}
*/

/**@name 定时器任务
*@{
*/
/*定时器1回调函数*/
extern OS_TMR tmr1;
void tmr1_callback_led(void *p_tmr, void *p_arg);

/*定时器2*/
extern OS_TMR tmr2;
void tmr2_gizwtime(void *p_tmr, void *p_arg);

/*定时器3*/
extern OS_TMR tmr3;
void tmr3_matixKey(void *p_tmr, void *p_arg);
/**@}
*/

#endif
