
#include "app.h"

Gozwits_Even_Mutex even_mutex;

extern dataPoint_t currentDataPoint;
extern keysTypedef_t keys;
extern gizwitsProtocol_t gizwitsProtocol;
//extern dataSynchro synchro_data;
extern SysPara 	AS608Para;//ָ��ģ��AS608����
extern uint16_t 	ValidN;//ģ������Чģ�����
char TEXT_Buffer[]={"STM32 FLASH TEST"};
extern uint32_t timerMsCount;
#define SIZE sizeof(TEXT_Buffer)	 	//���鳤��

//��ʼ������ƿ�
OS_TCB	StartTackTCB;
//��ʼ�����ջ
CPU_STK	START_TASK_STK[START_STK_SIZE];

//����MOTOR���ƿ�
OS_TCB MOTORTCB;
//����MOTOR��ջ
CPU_STK	MOTOR_STK[MOTOR_STK_SIZE];

//����MOTOR���ƿ�
OS_TCB GizwitsTCB;
//����MOTOR��ջ
CPU_STK	Gizwits_STK[Gizwits_STK_SIZE];

//NRF24L01������ƿ�
OS_TCB NRF24L01TCB;
CPU_STK NRF24L01_STK[NRF24L01_STK_SIZE];

/*��ʱ��1�ص�����*/
OS_TMR tmr1;

/*��ʱ��2*/
OS_TMR tmr2;

/*��ʱ��3*/
OS_TMR tmr3;

//����MOTOR���ƺ���
void Motor_task(volatile void *p_arg)
{
	OS_ERR err;
	static uint8_t start_motor = 0;
	static uint8_t start_lack = 0;
	p_arg = p_arg;
	while(1)
	{
		even_mutex.os_flag = 0;
		switch (even_mutex.flag)
		{
			case 3:
				if (start_lack == 0)
				{
					GPIO_SetBits(GPIOB, GPIO_Pin_6);
					GPIO_ResetBits(GPIOB, GPIO_Pin_7);
//					currentDataPoint.valuelatch_lock = 0;
				}
				start_lack++;
				break;
			case 4:
				GPIO_SetBits(GPIOB, GPIO_Pin_7);
				GPIO_ResetBits(GPIOB, GPIO_Pin_6);
//				currentDataPoint.valuelatch_lock = 1;
				start_lack = 0;
				break;
			case 1:
				if (start_motor == 0)
				{
					GPIO_SetBits(GPIOB, GPIO_Pin_8);
					GPIO_ResetBits(GPIOB, GPIO_Pin_9);
					currentDataPoint.valueswitch_door = 1;
				}
				start_motor++;
				break;
			case 2:
				GPIO_SetBits(GPIOB, GPIO_Pin_9);
				GPIO_ResetBits(GPIOB, GPIO_Pin_8);
				currentDataPoint.valueswitch_door = 0;
				start_motor = 0;
				break;
			default:
				break;
		}
		OSTimeDlyHMSM(0,0,(even_mutex.delay_time/1000),(even_mutex.delay_time%1000),OS_OPT_TIME_HMSM_STRICT,&err);
		even_mutex.switchdoor = 0;
		even_mutex.latch_lock = 0;
		GPIO_ResetBits(GPIOB, GPIO_Pin_6 | GPIO_Pin_7 | GPIO_Pin_8 | GPIO_Pin_9);
		even_mutex.os_flag = 1;
		OSTaskSuspend((OS_TCB*)&MOTORTCB, &err);
	}
}

//����Gizwits���ƺ���
void Gizwits_task(volatile void *p_arg)
{
	OS_ERR err;
	p_arg = p_arg;
	while(1)
	{
		OSSchedLock(&err);
//		currentDataPoint.valuedump_energy = 100;
		currentDataPoint.valueFlag_switch = GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_11);
    gizwitsHandle((dataPoint_t *)&currentDataPoint);
		OSSchedUnlock(&err);
		OSTimeDlyHMSM(0,0,0,100,OS_OPT_TIME_HMSM_STRICT,&err);
	}
}

uint8_t rbuf[32] = {0};
//NRF24L01������
void NRF24L01_task(volatile void *p_arg)
{
	OS_ERR err;
	uint16_t randpasswd = 0;
	uint16_t temp = 0;
	p_arg = p_arg;
	while(1)
	{
		OSSchedLock(&err);
		NRF24L01_RX_Mode();
		if (NRF24L01_RxPacket(rbuf) == 0)
		{
			if (rbuf[0] == 0xfe)
			{
				if (rbuf[1] == 0x11)
				{
					currentDataPoint.valueswitch_door = 1;
					LED_Open(1,0);
					if (even_mutex.switchdoor == 0)
					{
						even_mutex.flag = 1;
						even_mutex.switchdoor = 1;
						even_mutex.delay_time = MOTOT_DELAY;
						OSTaskResume((OS_TCB*)&MOTORTCB, &err);
					}
				}
				randpasswd = rbuf[2];
				randpasswd = (randpasswd << 8) | rbuf[3];
				temp = rbuf[4];
				temp = (temp << 8) | rbuf[5];
				currentDataPoint.valuevisitor_passwd = randpasswd;
				currentDataPoint.valueswitch_manID = temp;
				for (uint8_t i=0; i<6; i++)
					rbuf[i] = 0x00;
			}
		}
		OSSchedUnlock(&err);
		OSTimeDlyHMSM(0,0,0,500,OS_OPT_TIME_HMSM_STRICT,&err);
	}
}

//��ʱ��1�ص�����
void tmr1_callback_led(void *p_tmr, void *p_arg)
{
	IWDG_FREE();
	keyHandle((keysTypedef_t *)&keys);
}

void tmr2_gizwtime(void *p_tmr, void *p_arg)
{
//	gizwitsProtocol.timerMsCount += 10;	
	timerMsCount += 10;
}

void tmr3_matixKey(void *p_tmr, void *p_arg)
{
	static uint16_t timedelayms = 0;
	OS_ERR err;
	if (currentDataPoint.valueFlag_switch == 0)
	{
		if (even_mutex.os_flag == 1)
		{
			if (even_mutex.flag == 1)
			{
				even_mutex.flag = 2;
				even_mutex.switchdoor = 1;
				even_mutex.delay_time = MOTOT_DELAY - 100;
				OSTaskResume((OS_TCB*)&MOTORTCB, &err);
			}
		}
	}
	else if (currentDataPoint.valueFlag_switch == 1)
	{
		timedelayms++;
		if (GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_12) == 1 && (currentDataPoint.valuechildren_mode == 0))
		{
			if (timedelayms > 200)
			{
				even_mutex.flag = 1;
				even_mutex.switchdoor = 1;
				even_mutex.delay_time = MOTOT_DELAY;
				OSTaskResume((OS_TCB*)&MOTORTCB, &err);
				timedelayms = 0;
			}
		}
	}
//	LED = ~LED;
//	static uint16_t num = 0;
//	num++;
//	LCD_SPI2_P6x8Num(40,2,num);
}

//��ʼ������ƺ���
void start_task(volatile void *p_arg)
{
  OS_ERR err;
  CPU_SR_ALLOC();
  p_arg = p_arg;
  
  CPU_Init();
#if OS_CFG_STAT_TASK_EN > 0u
   OSStatTaskCPUUsageInit(&err);  	//ͳ������
#endif

#ifdef CPU_CFG_INT_DIS_MEAS_EN		//���ʹ���˲����жϹر�ʱ��
    CPU_IntDisMeasMaxCurReset();
#endif

#if	OS_CFG_SCHED_ROUND_ROBIN_EN  //��ʹ��ʱ��Ƭ��ת��ʱ��
	 //ʹ��ʱ��Ƭ��ת���ȹ���,ʱ��Ƭ����Ϊ1��ϵͳʱ�ӽ��ģ���1*5=5ms
	OSSchedRoundRobinCfg(DEF_ENABLED,1,&err);
#endif

	OS_CRITICAL_ENTER();//�����ٽ������뱣��
	
	/*��������MOTOR*/
	OSTaskCreate ((OS_TCB        *)&MOTORTCB,
								(CPU_CHAR      *)"MOTOR",
								(OS_TASK_PTR    )Motor_task,
								(void          *)0,
								(OS_PRIO        )MOTOR_TASK_PRIO,
								(CPU_STK       *)MOTOR_STK,
								(CPU_STK_SIZE   )MOTOR_STK_SIZE/10,
								(CPU_STK_SIZE   )MOTOR_STK_SIZE,
								(OS_MSG_QTY     )0,
								(OS_TICK        )0,
								(void          *)0,
								(OS_OPT         )OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR,
								(OS_ERR        *)&err);
	/*��������Gizwits*/
	OSTaskCreate ((OS_TCB        *)&GizwitsTCB,
								(CPU_CHAR      *)"Gizwits",
								(OS_TASK_PTR    )Gizwits_task,
								(void          *)0,
								(OS_PRIO        )Gizwits_TASK_PRIO,
								(CPU_STK       *)Gizwits_STK,
								(CPU_STK_SIZE   )Gizwits_STK_SIZE/10,
								(CPU_STK_SIZE   )Gizwits_STK_SIZE,
								(OS_MSG_QTY     )0,
								(OS_TICK        )0,
								(void          *)0,
								(OS_OPT         )OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR,
								(OS_ERR        *)&err);
	/*��������NRF24L01*/
	OSTaskCreate ((OS_TCB        *)&NRF24L01TCB,
								(CPU_CHAR      *)"NRF24L01",
								(OS_TASK_PTR    )NRF24L01_task,
								(void          *)0,
								(OS_PRIO        )NRF24L01_TASK_PRIO,
								(CPU_STK       *)NRF24L01_STK,
								(CPU_STK_SIZE   )NRF24L01_STK_SIZE/10,
								(CPU_STK_SIZE   )NRF24L01_STK_SIZE,
								(OS_MSG_QTY     )0,
								(OS_TICK        )0,
								(void          *)0,
								(OS_OPT         )OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR,
								(OS_ERR        *)&err);
	/*���������ʱ��1*/
	OSTmrCreate	((OS_TMR         			 *)&tmr1,
							 (CPU_CHAR             *)"tmr1_led",
							 (OS_TICK               )0,//��ʼ�ӳ�Ϊ0*10ms
							 (OS_TICK               )1,//��ʱ����Ϊ1*10ms
							 (OS_OPT                )OS_OPT_TMR_PERIODIC,//����ģʽ
							 (OS_TMR_CALLBACK_PTR   )tmr1_callback_led,
							 (void                 *)0,
							 (OS_ERR               *)&err);
	/*���������ʱ��2*/
	OSTmrCreate	((OS_TMR         			 *)&tmr2,
							 (CPU_CHAR             *)"tmr2_giztime",
							 (OS_TICK               )0,//��ʼ�ӳ�Ϊ0*10ms
							 (OS_TICK               )1,//��ʱ����Ϊ1*10ms
							 (OS_OPT                )OS_OPT_TMR_PERIODIC,//����ģʽ
							 (OS_TMR_CALLBACK_PTR   )tmr2_gizwtime,
							 (void                 *)0,
							 (OS_ERR               *)&err);
	/*���������ʱ��3*/
	OSTmrCreate	((OS_TMR         			 *)&tmr3,
							 (CPU_CHAR             *)"tmr3_matixKey",
							 (OS_TICK               )0,//��ʼ�ӳ�Ϊ0*10ms
							 (OS_TICK               )5,//��ʱ����Ϊ1*10ms
							 (OS_OPT                )OS_OPT_TMR_PERIODIC,//����ģʽ
							 (OS_TMR_CALLBACK_PTR   )tmr3_matixKey,
							 (void                 *)0,
							 (OS_ERR               *)&err);
	OS_CRITICAL_EXIT();//�˳��ٽ������뱣��
	OSTaskSuspend((OS_TCB*)&MOTORTCB, &err);
	OSTmrStart(&tmr1,&err);
	OSTmrStart(&tmr2,&err);
	OSTmrStart(&tmr3,&err);
}
