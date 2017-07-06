
#include "app.h"

Gozwits_Even_Mutex even_mutex;

extern dataPoint_t currentDataPoint;
extern keysTypedef_t keys;
extern gizwitsProtocol_t gizwitsProtocol;
//extern dataSynchro synchro_data;
extern SysPara 	AS608Para;//指纹模块AS608参数
extern uint16_t 	ValidN;//模块内有效模板个数
char TEXT_Buffer[]={"STM32 FLASH TEST"};
extern uint32_t timerMsCount;
#define SIZE sizeof(TEXT_Buffer)	 	//数组长度

//开始任务控制块
OS_TCB	StartTackTCB;
//开始任务堆栈
CPU_STK	START_TASK_STK[START_STK_SIZE];

//任务MOTOR控制块
OS_TCB MOTORTCB;
//任务MOTOR堆栈
CPU_STK	MOTOR_STK[MOTOR_STK_SIZE];

//任务MOTOR控制块
OS_TCB GizwitsTCB;
//任务MOTOR堆栈
CPU_STK	Gizwits_STK[Gizwits_STK_SIZE];

//NRF24L01任务控制块
OS_TCB NRF24L01TCB;
CPU_STK NRF24L01_STK[NRF24L01_STK_SIZE];

/*定时器1回调函数*/
OS_TMR tmr1;

/*定时器2*/
OS_TMR tmr2;

/*定时器3*/
OS_TMR tmr3;

//任务MOTOR控制函数
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

//任务Gizwits控制函数
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
//NRF24L01任务函数
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

//定时器1回调函数
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

//开始任务控制函数
void start_task(volatile void *p_arg)
{
  OS_ERR err;
  CPU_SR_ALLOC();
  p_arg = p_arg;
  
  CPU_Init();
#if OS_CFG_STAT_TASK_EN > 0u
   OSStatTaskCPUUsageInit(&err);  	//统计任务
#endif

#ifdef CPU_CFG_INT_DIS_MEAS_EN		//如果使能了测量中断关闭时间
    CPU_IntDisMeasMaxCurReset();
#endif

#if	OS_CFG_SCHED_ROUND_ROBIN_EN  //当使用时间片轮转的时候
	 //使能时间片轮转调度功能,时间片长度为1个系统时钟节拍，既1*5=5ms
	OSSchedRoundRobinCfg(DEF_ENABLED,1,&err);
#endif

	OS_CRITICAL_ENTER();//进入临界区代码保护
	
	/*创建任务MOTOR*/
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
	/*创建任务Gizwits*/
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
	/*创建任务NRF24L01*/
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
	/*创建软件定时器1*/
	OSTmrCreate	((OS_TMR         			 *)&tmr1,
							 (CPU_CHAR             *)"tmr1_led",
							 (OS_TICK               )0,//初始延迟为0*10ms
							 (OS_TICK               )1,//定时周期为1*10ms
							 (OS_OPT                )OS_OPT_TMR_PERIODIC,//周期模式
							 (OS_TMR_CALLBACK_PTR   )tmr1_callback_led,
							 (void                 *)0,
							 (OS_ERR               *)&err);
	/*创建软件定时器2*/
	OSTmrCreate	((OS_TMR         			 *)&tmr2,
							 (CPU_CHAR             *)"tmr2_giztime",
							 (OS_TICK               )0,//初始延迟为0*10ms
							 (OS_TICK               )1,//定时周期为1*10ms
							 (OS_OPT                )OS_OPT_TMR_PERIODIC,//周期模式
							 (OS_TMR_CALLBACK_PTR   )tmr2_gizwtime,
							 (void                 *)0,
							 (OS_ERR               *)&err);
	/*创建软件定时器3*/
	OSTmrCreate	((OS_TMR         			 *)&tmr3,
							 (CPU_CHAR             *)"tmr3_matixKey",
							 (OS_TICK               )0,//初始延迟为0*10ms
							 (OS_TICK               )5,//定时周期为1*10ms
							 (OS_OPT                )OS_OPT_TMR_PERIODIC,//周期模式
							 (OS_TMR_CALLBACK_PTR   )tmr3_matixKey,
							 (void                 *)0,
							 (OS_ERR               *)&err);
	OS_CRITICAL_EXIT();//退出临界区代码保护
	OSTaskSuspend((OS_TCB*)&MOTORTCB, &err);
	OSTmrStart(&tmr1,&err);
	OSTmrStart(&tmr2,&err);
	OSTmrStart(&tmr3,&err);
}
