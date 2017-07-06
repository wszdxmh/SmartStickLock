
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
#include "oled_library.h"
#include "stdio.h"
#include <stdlib.h>
#include "includes.h"
#include "watchdog.h"
#include "nrf24l01.h"
#include "as608.h"
#include "key.h"
#include "stm32flash.h"
#include "list.h"
#include "malloc.h"
#include "adc.h"
#include "ds18b20.h"

#define LED PAout(15)

typedef struct USER_FLAG {
	uint8_t nrf_flag;
	uint8_t finger_flag;
	uint8_t rand_flag;
	uint8_t mode;
	uint8_t key_mode;
	short temp;
	uint16_t last_randpasswd;
	uint16_t randpaswwd;
	uint8_t passwd[6];
	uint8_t randbuffer[5];
	uint8_t Key_Number[11];
}user_flag;

/**@name ��ʼ����
*@{
*/
#define START_TASK_PRIO	3
#define START_STK_SIZE	64
extern OS_TCB	StartTackTCB;
extern CPU_STK	START_TASK_STK[START_STK_SIZE];
void start_task(volatile void *p_arg);
/**@}
*/

/**@name ��������
*@{
*/
#define MOTOR_TASK_PRIO	5
#define MOTOR_STK_SIZE	64
extern OS_TCB MOTORTCB;
extern CPU_STK	MOTOR_STK[MOTOR_STK_SIZE];
void Motor_task(volatile void *p_arg);
/**@}
*/

/**@name NRF24L01����
*@{
*/
#define NRF24L01_TASK_PRIO 10
#define NRF24L01_STK_SIZE  64
extern OS_TCB NRF24L01TCB;
extern CPU_STK NRF24L01_STK[NRF24L01_STK_SIZE];
void NRF24L01_task(volatile void *p_arg);
/**@}
*/

/**@name AS608����
*@{
*/
#define AS608_TASK_PRIO 8
#define AS608_STK_SIZE  64
extern OS_TCB AS608TCB;
extern CPU_STK AS608_STK[AS608_STK_SIZE];
void Add_FR_task(volatile void *p_arg);
/**@}
*/

/**@name AS608ˢָ������
*@{
*/
#define Press_FR_TASK_PRIO 9
#define Press_FR_STK_SIZE  64
extern OS_TCB Press_FR_TCB;
extern CPU_STK Press_FR_STK[Press_FR_STK_SIZE];
void Press_FR_task(volatile void *p_arg);
/**@}
*/

/**@name DS18B20����
*@{
*/
#define DS18B20_TASK_PRIO 7
#define DS18B20_STK_SIZE  64
extern OS_TCB DS18B20TCB;
extern CPU_STK DS18B20_STK[DS18B20_STK_SIZE];
void DS18B20_task(volatile void *p_arg);
/**@}
*/

/**@name OLED��ʾ����
*@{
*/
#define OLED_Display_TASK_PRIO 4
#define OLED_Display_STK_SIZE  64
extern OS_TCB OLED_Display_TCB;
extern CPU_STK OLED_Display_STK[OLED_Display_STK_SIZE];
void OLED_Display_task(volatile void *p_arg);
/**@}
*/

/**@name ����ɨ������
*@{
*/
#define Key_Scan_TASK_PRIO 6
#define Key_Scan_STK_SIZE  128
extern OS_TCB Key_Scan_TCB;
extern CPU_STK Key_Scan_STK[Key_Scan_STK_SIZE];
void Key_Scan_task(volatile void *p_arg);
/**@}
*/

/**@name ��ʱ������
*@{
*/
/*��ʱ��1�ص�����*/
extern OS_TMR tmr1;
void tmr1_callback_led(void *p_tmr, void *p_arg);

/*��ʱ��2*/
extern OS_TMR tmr2;
void tmr2_Refresh_OLED(void *p_tmr, void *p_arg);

/*��ʱ��3*/
extern OS_TMR tmr3;
void tmr3_matixKey(void *p_tmr, void *p_arg);
/**@}
*/

#endif
